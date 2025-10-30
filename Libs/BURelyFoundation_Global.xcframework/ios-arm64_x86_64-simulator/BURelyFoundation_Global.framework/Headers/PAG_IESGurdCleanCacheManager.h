//
//  IESGurdCleanCacheManager.h
//  BDAssert
//
//  Created by 陈煜钏 on 2020/7/20.
//

#import <Foundation/Foundation.h>
#import "PAG_IESGurdCacheCleanerManager.h"
NS_ASSUME_NONNULL_BEGIN

@protocol PAG_IESGurdCacheCleanerManagerDelegate;

@interface PAG_IESGurdCleanCacheManager : NSObject <PAG_IESGurdCacheCleanerManagerDelegate>

+ (instancetype)sharedManager;

/**
 * @brief 清除cache
 */
+ (void)cleanCache;

/**
 根据accessKey和channel清理对应的缓存;
 */
+ (void)cleanCacheForAccessKey:(NSString *)accessKey
                       channel:(NSString *)channel;

/**
根据accessKey和channel清理对应的缓存;
*/
+ (void)cleanCacheForAccessKey:(NSString *)accessKey
                       channel:(NSString *)channel
                    completion:(void (^ _Nullable)(BOOL succeed, NSDictionary *info, NSError *error))completion;

@end

NS_ASSUME_NONNULL_END
