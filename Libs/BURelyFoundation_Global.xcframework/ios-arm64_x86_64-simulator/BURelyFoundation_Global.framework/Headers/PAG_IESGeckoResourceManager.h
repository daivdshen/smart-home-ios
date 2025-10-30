//
//  PAG_IESGurdResourceManager.h
//  PAG_IESGurdKit
//
//  Created by 01 on 17/6/30.
//

#import "PAG_IESGeckoDefines.h"

NS_ASSUME_NONNULL_BEGIN

extern NSString *PAG_IESGurdDownloadInfoDurationKey;    //下载时长key
extern NSString *PAG_IESGurdDownloadInfoURLKey;         //下载地址key

@interface PAG_IESGurdResourceManager : NSObject

/**
 * 下载包, 失败后重试
 */
+ (void)downloadPackageWithURLStrings:(NSArray <NSString *> *)URLStrings
                           completion:(IESGurdDownloadResourceCompletion)completion;

/**
 发送GET请求
 */
//+ (void)GETWithURLString:(NSString * _Nonnull)URLString
//                  params:(NSDictionary * _Nullable)params
//              completion:(IESGurdHTTPRequestCompletion)completion;

/**
 发送POST请求
 */
+ (void)POSTWithURLString:(NSString * _Nonnull)URLString
                   params:(NSDictionary * _Nullable)params
               completion:(nullable IESGurdHTTPRequestCompletion)completion;

@end

NS_ASSUME_NONNULL_END
