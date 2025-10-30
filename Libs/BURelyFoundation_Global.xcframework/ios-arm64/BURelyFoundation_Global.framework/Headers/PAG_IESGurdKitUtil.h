//
//  BU_PAG_IESGurdKitUtil.h
//  IESGeckoKit
//
//  Created by 陈煜钏 on 2020/3/5.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

#pragma mark - Base

/**
 当前网络状态
 */
extern NSString *PAG_IESGurdCurrentConnectionString (void);

/**
 返回客户端通用参数
 */
extern NSDictionary *PAG_IESGurdClientCommonParams (void);

#pragma mark - Hook

extern void PAG_IESGurdKitHookInstanceMethod(Class targetClass, SEL originalSEL, SEL swizzledSEL);

extern void PAG_IESGurdKitHookClassMethod(Class targetClass, SEL originalSEL, SEL swizzledSEL);

#pragma mark - Queue

extern dispatch_queue_t PAG_IESGurdKitCreateSerialQueue(const char *_Nullable label);

extern dispatch_queue_t PAG_IESGurdKitCreateConcurrentQueue(const char *_Nullable label);

NS_ASSUME_NONNULL_END
