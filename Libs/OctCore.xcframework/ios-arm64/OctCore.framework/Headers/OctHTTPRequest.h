//
//  OctHTTPRequest.h
//  OctCoreSDK
//
//  Created by guantou on 2019/12/9.
//

#import <Foundation/Foundation.h>
#import "OctHttpRequestType.h"

NS_ASSUME_NONNULL_BEGIN

/**
 请求管理类
 */
@interface OctHTTPRequest : NSObject

@property (nonatomic, assign) NSTimeInterval timeoutInterval;

+ (OctHTTPRequest *)shareInstance;

/**
 POST请求，传入加密data
 */
+ (OctURLSessionDataTask *)HTTPSerializerPOSTWithUrlString:(NSString *)urlString
                                                  parameters:(id)parameters
                                               backQueueName:(dispatch_queue_t)backQueueName
                                                successBlock:(OctResponseSuccess)successBlock
                                                failureBlock:(OctResponseFail)failureBlock;
+ (OctURLSessionDataTask *)HTTPSerializerPOSTWithUrlString:(NSString *)urlString
                                                  parameters:(id)parameters
                                               backQueueName:(dispatch_queue_t)backQueueName
                                                   timeOut: (double)timeInterval
                                                successBlock:(OctResponseSuccess)successBlock
                                              failureBlock:(OctResponseFail)failureBlock;

/**
 正常GET请求
 */
+ (OctURLSessionDataTask *)requestGETWithUrlString:(NSString *)urlString
                                       backQueueName:(dispatch_queue_t)backQueueName
                                        successBlock:(OctResponseSuccess __nullable)successBlock
                                        failureBlock:(OctResponseFail __nullable)failureBlock;

/**
 修改userAgent的GET请求
 */
+ (OctURLSessionDataTask *)requestGETWithUrlString:(NSString *)urlString
                                           userAgent:(NSString *)userAgent
                                       backQueueName:(dispatch_queue_t)backQueueName
                                        successBlock:(OctResponseSuccess)successBlock
                                        failureBlock:(OctResponseFail)failureBlock;

///**
// 正常POST请求
// */
//+ (OctURLSessionDataTask *)requestPOSTWithUrlString:(NSString *)urlString
//                                           parameters:(id)parameters
//                                        backQueueName:(dispatch_queue_t)backQueueName
//                                         successBlock:(OctResponseSuccess)successBlock
//                                         failureBlock:(OctResponseFail)failureBlock;

/**
 正常图片下载
 */
+ (OctURLSessionDataTask *)downLoadImage:(NSString *)imageURL
                              successBlock:(OctResponseSuccess)successBlock
                              failureBlock:(OctResponseFail)failureBlock;

/**
 上传单文件
*/
+ (OctURLSessionDataTask *)uploadTaskUrl:(NSString *__nullable)uploadPage
                                    params:(NSDictionary *__nullable)params
                                  filePath:(NSString *__nullable)filePath
                                  fileName:(NSString *__nullable)fileName
                              successBlock:(OctResponseSuccess __nullable)successBlock
                              failureBlock:(OctResponseFail __nullable)failureBlock;

///**
// Crash收集
// */
//+ (OctURLSessionDataTask *)requestWithCrash;

@end

NS_ASSUME_NONNULL_END
