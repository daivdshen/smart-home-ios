//
//  BUAdNetworkRequest.h
//  BUFoundation
//
//  Created by bytedance on 2020/12/15.
//

#if __has_include(<BURelyFoundation/PAGBaseRequest.h>)
#import <BURelyFoundation/PAGBaseRequest.h>
#elif __has_include(<BURelyFoundation_Global/PAGBaseRequest.h>)
#import <BURelyFoundation_Global/PAGBaseRequest.h>
#else
#import "PAGBaseRequest.h"
#endif


NS_ASSUME_NONNULL_BEGIN

@interface PAGNetworkRequest : PAGBaseRequest

@property (nonatomic,copy  ) NSString *buRequestUrl;
@property (nonatomic,strong) NSDictionary *parameter;
@property (nonatomic,assign) PAGRequestMethod method;
@property (nonatomic,assign) PAGRequestSerializerType buRequestSerializerType;
@property (nonatomic,assign) PAGResponseSerializerType buResponseSerializerType;

- (instancetype)initWithUrl:(NSString * _Nullable)url
                     method:(PAGRequestMethod)method
                  parameter:(NSDictionary * _Nullable)parameter;

+ (instancetype)requestWithURL:(NSString *)url
                        method:(PAGRequestMethod)method
                     parameter:(NSDictionary *)paraDic
                 isSupportGzip:(BOOL)isSupportGzip
         completionWithSuccess:(PAGRequestCompletionBlock)success
                       failure:(PAGRequestCompletionBlock)failure;

+ (instancetype)requestWithURL:(NSString *)url
                        method:(PAGRequestMethod)method
                     parameter:(NSDictionary *)paraDic
         completionWithSuccess:(PAGRequestCompletionBlock  _Nullable)success
                       failure:(PAGRequestCompletionBlock _Nullable)failure;

@end

@interface PAGNetworkRequest (TNC)
// 使用TNC服务的标识，自定义，唯一即可
- (NSString *)TNCServiceKey;

// 请求失败回调，如果需要TNC，请使用`failureCompletionBlockWithTNC`替换`failureCompletionBlock`的实现
- (PAGRequestCompletionBlock)failureCompletionBlockWithTNC;

// 请求成功回调，如果需要TNC，请使用`successCompletionBlockWithTNC`替换`successCompletionBlock`的实现
- (PAGRequestCompletionBlock)successCompletionBlockWithTNC;

@end

NS_ASSUME_NONNULL_END
