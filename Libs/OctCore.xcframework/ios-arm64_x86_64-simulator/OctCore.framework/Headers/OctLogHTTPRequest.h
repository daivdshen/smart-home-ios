//
//  OctLogHTTPRequest.h
//  OctCoreSDK
//
//  Created by guantou on 2019/12/9.
//

#import <Foundation/Foundation.h>
#import "OctHttpRequestType.h"

NS_ASSUME_NONNULL_BEGIN

/**
 上传日志worker
*/
@interface OctLogHTTPRequest : NSObject

+ (OctLogHTTPRequest *)shareInstance;

/**
 发送时时Log请求
 */
- (void)beginLogHTTPRequestWithURL:(NSString *)url
                      successBlock:(OctResponseSuccessWithModel)successBlock
                      failureBlock:(OctResponseFailWithError)failureBlock
                            iSSpin:(BOOL)spin;

- (void)beginOfflineLogHTTPRequestWithURL:(NSString *)url
                                 filePath:(NSString *)filePath
                                 fileName:(NSString *)fileName
                             successBlock:(OctResponseSuccessWithModel)successBlock
                             failureBlock:(OctResponseFailWithError)failureBlock;

@end

NS_ASSUME_NONNULL_END
