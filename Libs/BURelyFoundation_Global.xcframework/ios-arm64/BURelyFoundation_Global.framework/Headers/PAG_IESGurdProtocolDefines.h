//
//  BU_IESGurdProtocolDefines.h
//  Pods
//
//  Created by chenyuchuan on 2019/6/5.
//

#ifndef PAG_IESGurdProtocolDefines_h
#define PAG_IESGurdProtocolDefines_h

#import "PAG_IESGeckoDefines.h"
#import "PAG_IESGurdDownloadPackageInfo.h"
#import "PAG_IESGurdUnzipPackageInfo.h"
#import "PAG_IESGurdNetworkResponse.h"

NS_ASSUME_NONNULL_BEGIN

typedef void(^PAG_IESGurdNetworkDelegateDownloadCompletion)(NSURL * _Nullable pathURL, NSError * _Nullable error);

@protocol PAG_IESGurdNetworkDelegate <NSObject>

- (void)downloadPackageWithURLString:(NSString *)packageURLString
                          completion:(PAG_IESGurdNetworkDelegateDownloadCompletion)completion;

- (void)requestWithMethod:(NSString *)method
                URLString:(NSString *)URLString
                   params:(NSDictionary *)params
               completion:(void (^)(PAG_IESGurdNetworkResponse *response))completion;

@end

@protocol PAG_IESGurdDownloaderDelegate <NSObject>

- (void)downloadPackageWithIdentity:(NSString *)identity
                         URLStrings:(NSArray <NSString *> *)URLStrings
                         completion:(IESGurdDownloadResourceCompletion)completion;

// 用于清理任务和下载缓存
- (void)cancelDownloadWithIdentity:(NSString *)identity;

@end

@protocol PAG_IESGurdEventDelegate <NSObject>

@optional
/**
 请求资源包的状态
 
 @param accessKey accessKey
 @param configsDictionary @{ channelName : @(IESGurdRequestChannelConfigStatus) }
 */
- (void)gurdDidRequestConfigForAccessKey:(NSString *)accessKey
                       configsDictionary:(NSDictionary<NSString *, NSNumber *> *)configsDictionary;

/**
 即将下载资源包
 
 @param accessKey accessKey
 @param channel channel
 @param isPatch 是否增量包
 */
- (void)gurdWillDownloadPackageForAccessKey:(NSString *)accessKey
                                    channel:(NSString *)channel
                                    isPatch:(BOOL)isPatch;

/**
 下载资源包完成；包括成功和失败
 
 @param accessKey accessKey
 @param channel channel
 @param packageInfo 下载资源包信息
 */
- (void)gurdDidFinishDownloadingPackageForAccessKey:(NSString *)accessKey
                                            channel:(NSString *)channel
                                        packageInfo:(PAG_IESGurdDownloadPackageInfo *)packageInfo;

/**
 解压资源包完成
 
 @param accessKey accessKey
 @param channel channel
 @param packageInfo 解压资源包信息
 */
- (void)gurdDidFinishUnzippingPackageForAccessKey:(NSString *)accessKey
                                          channel:(NSString *)channel
                                      packageInfo:(PAG_IESGurdUnzipPackageInfo *)packageInfo;

/**
 激活资源包完成；包括成功和失败

 @param accessKey accessKey
 @param channel channel
 @param succeed 是否成功
 @param error 失败的错误信息
 */
- (void)gurdDidFinishApplyingPackageForAccessKey:(NSString *)accessKey
                                         channel:(NSString *)channel
                                         succeed:(BOOL)succeed
                                           error:(NSError * _Nullable)error;

/**
 清理资源包
 
 @param accessKey accessKey
 @param channel 被清理的channel
 */
- (void)gurdDidCleanCachePackageForAccessKey:(NSString *)accessKey
                                     channel:(NSString *)channel;

@end


/// 数据上报
@protocol PAG_IESGurdAppLogDelegate <NSObject>

- (void)trackEvent:(NSString *)event params:(NSDictionary *)params;

@end

NS_ASSUME_NONNULL_END

#endif /* PAG_IESGurdProtocolDefines_h */
