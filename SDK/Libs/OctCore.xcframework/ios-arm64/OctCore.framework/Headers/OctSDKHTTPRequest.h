//
//  OctCoreSDKHTTPRequest.h
//  OctCoreSDK
//
//  Created by guantou on 2019/12/9.
//

#import <Foundation/Foundation.h>
#import "OctHttpRequestType.h"

NS_ASSUME_NONNULL_BEGIN

typedef void(^NormalGetDataBlock)(NSDictionary * __nullable);
/**
 请求广告worker
*/
@interface OctSDKHTTPRequest : NSObject

+ (OctSDKHTTPRequest *)shareInstance;

/**
 发起广告请求
 */
- (void)beginAdvertisingHTTPRequestWithData:(NSData *)data
                                 requestURL:(NSString *)requestURL
                               successBlock:(OctResponseSuccessWithModel)successBlock
                               failureBlock:(OctResponseFailWithError)failureBlock;
- (void)beginAdvertisingHTTPRequestWithData:(NSData *)data
                                 requestURL:(NSString *)requestURL
                                    timeOut: (double)timeInterval
                               successBlock:(OctResponseSuccessWithModel)successBlock
                               failureBlock:(OctResponseFailWithError)failureBlock;

//发起其它本公司的服务请求
- (void)beginCommonHTTPRequestWithData:(NSData *)data
                                 modelClass:(Class)modelClass
                                 requestURL:(NSString *)requestURL
                               successBlock:(OctResponseSuccessWithModel)successBlock
                          failureBlock:(OctResponseFailWithError)failureBlock;

//请求外部数据
- (void)octGetDataWithURL:(NSString *)viewURL handler:(NormalGetDataBlock)handler;

@end

NS_ASSUME_NONNULL_END
