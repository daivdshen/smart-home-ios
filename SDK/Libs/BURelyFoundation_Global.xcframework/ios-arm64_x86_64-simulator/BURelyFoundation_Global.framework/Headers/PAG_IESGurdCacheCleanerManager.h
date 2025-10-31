//
//  PAG_IESGurdCacheCleanerManager.h
//  IESGeckoKit
//
//  Created by chenyuchuan on 2019/6/6.
//

#import <Foundation/Foundation.h>

#import "PAG_IESGurdCacheCleaner.h"
#import "PAG_IESGurdCacheConfiguration.h"

NS_ASSUME_NONNULL_BEGIN

@class PAG_IESGurdCacheCleanerManager;

@protocol PAG_IESGurdCacheCleanerManagerDelegate <NSObject>

- (void)cacheCleanerManager:(PAG_IESGurdCacheCleanerManager *)manager
     cleanCacheForAccessKey:(NSString *)accessKey
        channelsToBeCleaned:(NSArray<NSString *> *)channelsToBeCleaned;

@end

/**
 缓存策略管理
 note : 只在BU_PAG_IESGurdCacheManager处理cleaner相关事件
 */
@interface PAG_IESGurdCacheCleanerManager : NSObject

@property (nonatomic, weak) id<PAG_IESGurdCacheCleanerManagerDelegate> delegate;

+ (instancetype)sharedManager;

- (void)registerCacheCleanerForAccessKey:(NSString *)accessKey
                           configuration:(PAG_IESGurdCacheConfiguration *)configuration;

- (id<PAG_IESGurdCacheCleaner>)cleanerForAccessKey:(NSString *)accessKey;

- (void)addChannelsWhitelist:(NSArray<NSString *> *)channels
                forAccessKey:(NSString *)accessKey;

- (BOOL)isChannelInWhitelist:(NSString *)channel
                   accessKey:(NSString *)accessKey;

@end

NS_ASSUME_NONNULL_END
