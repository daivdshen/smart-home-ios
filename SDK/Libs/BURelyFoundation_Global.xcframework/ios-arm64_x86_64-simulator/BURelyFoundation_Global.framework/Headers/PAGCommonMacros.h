//
//  BUCommonMacros.h
//  BUAdSDK
//
//  Created by 崔亚楠 on 2018/10/23.
//  Copyright © 2018年 bytedance. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <libkern/OSAtomic.h>
#import <os/lock.h>

#import "PAGLogMacros.h"

FOUNDATION_EXPORT NSString * const PAGSDKRelyVersion;

/** String **/
#define PAGEmptyString                                 (@"");
#define PAGSafeString(__string)                        ((__string && [__string isKindOfClass:[NSString class]]) ? __string :@"")
#define PAGSafeDictionary(__aDictionary)               ((__aDictionary && [__aDictionary isKindOfClass:[NSDictionary class]]) ? __aDictionary :@{})

/** VALID CHECKING**/
#define PAGCheckValidString(__string)                (__string && [__string isKindOfClass:[NSString class]] && [__string length])
#define PAGCheckValidNumber(__aNumber)               (__aNumber && [__aNumber isKindOfClass:[NSNumber class]])
#define PAGCheckValidArray(__aArray)                 (__aArray && [__aArray isKindOfClass:[NSArray class]] && [__aArray count])
#define PAGCheckValidDictionary(__aDictionary)       (__aDictionary && [__aDictionary isKindOfClass:[NSDictionary class]] && [__aDictionary count])
#define PAGCheckValidDate(__aDate)                   (__aDate && [__aDate isKindOfClass:[NSDate class]])
#define PAGCheckValidData(__aData)                   (__aData && [__aData isKindOfClass:[NSData class]])

/** Color String**/
#define PAGColorString(__string)    [UIColor bu_colorWithHexString:(__string)]

/*********************************************************************************************************/
//强弱引用转换，用于解决代码块（block）与强引用对象之间的循环引用问题
#ifndef bu_weakify
#if __has_feature(objc_arc)
#define bu_weakify(object) __weak __typeof__(object) weak##object = object;
#else
#define bu_weakify(object) __block __typeof__(object) block##object = object;
#endif
#endif
#ifndef bu_strongify
#if __has_feature(objc_arc)
#define bu_strongify(object) __typeof__(object) object = weak##object;
#else
#define bu_strongify(object) __typeof__(object) object = block##object;
#endif
#endif
/*********************************************************************************************************/

#ifndef PAGisEmptyString
#define PAGisEmptyString(str) (!str || ![str isKindOfClass:[NSString class]] || str.length == 0)
#endif

#ifndef PAGIsEmptyArray
#define PAGIsEmptyArray(array) (!array || ![array isKindOfClass:[NSArray class]] || array.count == 0)
#endif

#ifndef PAGIsEmptyDictionary
#define PAGIsEmptyDictionary(dict) (!dict || ![dict isKindOfClass:[NSDictionary class]] || ((NSDictionary *)dict).count == 0)
#endif


#ifndef PAGMinX
#define PAGMinX(view) CGRectGetMinX(view.frame)
#endif

#ifndef PAGMinY
#define PAGMinY(view) CGRectGetMinY(view.frame)
#endif

#ifndef PAGMaxX
#define PAGMaxX(view) CGRectGetMaxX(view.frame)
#endif

#ifndef PAGMaxY
#define PAGMaxY(view) CGRectGetMaxY(view.frame)
#endif

#ifndef PAGWidth
#define PAGWidth(view) view.frame.size.width
#endif

#ifndef PAGHeight
#define PAGHeight(view) view.frame.size.height
#endif

#ifndef PAGScreenWidth
#define PAGScreenWidth [[UIScreen mainScreen] bounds].size.width
#endif

#ifndef PAGScreenHeight
#define PAGScreenHeight [[UIScreen mainScreen] bounds].size.height
#endif

#ifndef PAGMINScreenSide
#define PAGMINScreenSide                    MIN([UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height)
#endif

#ifndef PAGMAXScreenSide
#define PAGMAXScreenSide                   MAX([UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height)
#endif

#define PAGIsNotchScreen pag_is_notch_screen()
#define PAGiPhoneX PAGIsNotchScreen
#define kPAGDefaultNavigationBarHeight  (PAGiPhoneX?88:64)      // 导航条高度
#define kPAGSafeTopMargin (PAGiPhoneX?24:0)
#define kPAGDefaultStautsBarHeight  (PAGiPhoneX?44:20)      // 状态栏高度

#define PAGOnePixel (1.0f/[[UIScreen mainScreen] scale])

///全局队列
#ifndef PAGDispatchGetGlobalQueue
#define PAGDispatchGetGlobalQueue dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)
#endif

#ifndef PAGDispatchGetHighQueue
#define PAGDispatchGetHighQueue dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0)
#endif

//单例
#define PAG_SINGLETION(...)                                          \
+ (instancetype)sharedInstance NS_SWIFT_NAME(shared());

#define PAG_DEF_SINGLETION(...)                                      \
+ (instancetype)sharedInstance                                      \
{                                                                   \
static dispatch_once_t once;                                        \
static id __singletion;                                             \
dispatch_once(&once,^{__singletion = [[self alloc] init];});        \
return __singletion;                                                \
}

FOUNDATION_EXPORT void pag_safe_dispatch_sync_main_queue(void (^block)(void));
FOUNDATION_EXPORT void pag_safe_dispatch_async_main_queue(void (^block)(void));

FOUNDATION_EXPORT id PAG_JSONObjectByRemovingKeysWithNullValues(id JSONObject);

FOUNDATION_EXPORT BOOL pag_is_notch_screen(void);

FOUNDATION_EXPORT UIEdgeInsets pag_portraitAdSafeInsets(void);


#pragma mark - 锁

#define PAG_USE_OS_UNFAIR_LOCK (__IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_10_0)

#ifndef PAG_LOCK_DECLARE
#if PAG_USE_OS_UNFAIR_LOCK
#define PAG_LOCK_DECLARE(lock) os_unfair_lock lock
#else
#define PAG_LOCK_DECLARE(lock) os_unfair_lock lock API_AVAILABLE(ios(10.0), tvos(10), watchos(3), macos(10.12)); \
OSSpinLock lock##_deprecated;
#endif
#endif

#ifndef PAG_LOCK_INIT
#if PAG_USE_OS_UNFAIR_LOCK
#define PAG_LOCK_INIT(lock) lock = OS_UNFAIR_LOCK_INIT
#else
#define PAG_LOCK_INIT(lock) if (@available(iOS 10, tvOS 10, watchOS 3, macOS 10.12, *)) lock = OS_UNFAIR_LOCK_INIT; \
else lock##_deprecated = OS_SPINLOCK_INIT;
#endif
#endif

#ifndef PAG_LOCK
#if PAG_USE_OS_UNFAIR_LOCK
#define PAG_LOCK(lock) os_unfair_lock_lock(&lock)
#else
#define PAG_LOCK(lock) if (@available(iOS 10, tvOS 10, watchOS 3, macOS 10.12, *)) os_unfair_lock_lock(&lock); \
else OSSpinLockLock(&lock##_deprecated);
#endif
#endif

#ifndef PAG_UNLOCK
#if PAG_USE_OS_UNFAIR_LOCK
#define PAG_UNLOCK(lock) os_unfair_lock_unlock(&lock)
#else
#define PAG_UNLOCK(lock) if (@available(iOS 10, tvOS 10, watchOS 3, macOS 10.12, *)) os_unfair_lock_unlock(&lock); \
else OSSpinLockUnlock(&lock##_deprecated);
#endif
#endif

@protocol PAGDictionarify <NSObject>
@required
- (NSDictionary *)toDictionary;
@end



// 对枚举值进行日志字符串转换， 例如对于一个枚举值   1表示激励视频广告的意思， 将返回：   激励视频广告(value:1)
FOUNDATION_EXPORT NSString *NSStringLogFromPAGAdEnumItem(NSInteger enumItem, NSDictionary *dic, NSString *defaultValue);
// 对枚举值进行字符串转换   例如对于一个枚举值   1表示rewarded_ad的字符串， 将返回：  rewarded_ad
FOUNDATION_EXPORT NSString *NSStringFromPAGAdEnumItem(NSInteger enumItem, NSDictionary *dic, NSString *defaultValue);
