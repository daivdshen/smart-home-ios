//
//  PAG_IESGurdCacheCleaner.h
//  IESGeckoKit
//
//  Created by chenyuchuan on 2019/6/6.
//

#ifndef PAG_IESGurdCacheCleaner_h
#define PAG_IESGurdCacheCleaner_h

#import "PAG_IESGurdCacheConfiguration.h"

@protocol PAG_IESGurdCacheCleaner <NSObject>

@property (nonatomic, readonly, copy) NSString *accessKey;

@required
/**
 初始化方法
 */
+ (instancetype)cleanerWithAccessKey:(NSString *)accessKey
                       channelsArray:(NSArray<NSString *> *)channelsArray
                       configuration:(PAG_IESGurdCacheConfiguration *)configuration;

@optional

/**
 返回需要清理的channel
 */
- (NSArray<NSString *> *)channelsToBeCleaned;

/**
 gurd解压channel对应的资源
 */
- (void)gurdDidApplyPackageForChannel:(NSString *)channel;

/**
 gurd获取channel对应的资源
 */
- (void)gurdDidGetCachePackageForChannel:(NSString *)channel;

/**
 gurd删除channel对应的资源
 */
- (void)gurdDidCleanPackageForChannel:(NSString *)channel;

/**
 gurd新增channel白名单
 */
- (void)gurdDidAddChannelWhitelist:(NSArray<NSString *> *)channelWhitelist;

@end

#endif /* PAG_IESGurdCacheCleaner_h */
