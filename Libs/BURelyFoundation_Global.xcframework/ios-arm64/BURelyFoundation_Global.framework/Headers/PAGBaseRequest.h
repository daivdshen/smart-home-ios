//
//  PAGBaseRequest.h
//  BUAdSDK
//
//  Created by 李盛 on 2018/4/2.
//  Copyright © 2018年 bytedance. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

///  HTTP Request method.
typedef NS_ENUM(NSInteger, PAGRequestMethod) {
    PAGRequestMethodGET = 0,
    PAGRequestMethodPOST,
    PAGRequestMethodHEAD,
    PAGRequestMethodPUT,
    PAGRequestMethodDELETE,
    PAGRequestMethodPATCH,
};

///  Request serializer type.
typedef NS_ENUM(NSInteger, PAGRequestSerializerType) {
    PAGRequestSerializerTypeHTTP = 0,
    PAGRequestSerializerTypeJSON,
    PAGRequestSerializerTypeProtobuf
};

///  Response serializer type, which determines response serialization process and
///  the type of `responseObject`.
typedef NS_ENUM(NSInteger, PAGResponseSerializerType) {
    /// NSData type
    PAGResponseSerializerTypeHTTP,
    /// JSON object type
    PAGResponseSerializerTypeJSON,
    /// Protobuf object type
    PAGResponseSerializerTypeProtobuf
};

///  Request priority
typedef NS_ENUM(NSInteger, PAGRequestPriority) {
    PAGRequestPriorityLow = -4L,
    PAGRequestPriorityDefault = 0,
    PAGRequestPriorityHigh = 4,
};

@protocol PAG_AFMultipartFormData;

typedef void (^PAGAFConstructingBlock)(id<PAG_AFMultipartFormData> formData);
typedef void (^PAGAFURLSessionTaskProgressBlock)(NSProgress *);

@class PAGBaseRequest;

typedef void(^PAGRequestCompletionBlock)(__kindof PAGBaseRequest *request);

@interface PAGBaseRequest : NSObject

///  The underlying NSURLSessionTask.
///
///  @warning This value is actually nil and should not be accessed before the request starts.
@property (nonatomic, strong) NSURLSessionTask *requestTask;
@property (nonatomic, strong) NSData *responseData;
@property (nonatomic, strong) id responseJSONObject;
@property (nonatomic, strong) id responseObject;
@property (nonatomic, strong) NSString *responseString;
@property (nonatomic, strong) NSError *error;
@property (nonatomic, assign) PAGRequestMethod requestMethod;
/// For post method, when httpbody can not be Serialized from NSDictionary json. if httpBody exists, please use httpBody directively and ignore 'requestArgument'
@property (nonatomic, strong) NSData *httpBody;

/// Whether gzip compression of httpBody is supported
@property (nonatomic, assign) BOOL isSupportGzip;

///  Shortcut for `requestTask.currentRequest`.当前活跃的request
@property (nonatomic, strong, readonly) NSURLRequest *currentRequest;

///  Shortcut for `requestTask.originalRequest`.在task创建的时候传入的request(有可能会重定向)
@property (nonatomic, strong, readonly) NSURLRequest *originalRequest;

///  Shortcut for `requestTask.response`.
@property (nonatomic, strong, readonly) NSHTTPURLResponse *response;

///  The response status code.
@property (nonatomic, readonly) NSInteger responseStatusCode;

///  The success callback. Note if this value is not nil and `requestFinished` delegate method is
///  also implemented, both will be executed but delegate method is first called. This block
///  will be called on the main queue.
@property (nonatomic, copy, nullable) PAGRequestCompletionBlock successCompletionBlock;

///  The failure callback. Note if this value is not nil and `requestFailed` delegate method is
///  also implemented, both will be executed but delegate method is first called. This block
///  will be called on the main queue.
@property (nonatomic, copy, nullable) PAGRequestCompletionBlock failureCompletionBlock;

///  Additional HTTP request header field.
- (nullable NSDictionary<NSString *, NSString *> *)requestHeaderFieldValueDictionary;

///  Request serializer type.
- (PAGRequestSerializerType)requestSerializerType;

///  Response serializer type. See also `responseObject`.
- (PAGResponseSerializerType)responseSerializerType;

///  Request cache policy.
- (NSURLRequestCachePolicy)bu_requestCachePolicy;

//constructingBodyWithBlock:在此block种可以为上传的参数添加(拼接)新的需要的上传的数据,适用于上传给服务器的数据流比较大的时候
@property (nonatomic, copy, nullable) PAGAFConstructingBlock constructingBodyBlock;

- (NSString *)requestUrl;
- (NSString *)cdnUrl;
- (NSString *)baseUrl;
- (NSTimeInterval)requestTimeoutInterval;
- (nullable id)requestArgument;
///  Whether the request is allowed to use the cellular radio (if present). Default is YES.
- (BOOL)allowsCellularAccess;
///  Nil out both success and failure callback blocks.
- (void)clearCompletionBlock;

@property (nonatomic) PAGRequestPriority requestPriority;

///  Should use CDN when sending request.
- (BOOL)useCDN;

#pragma mark - Request Action
///=============================================================================
/// @name Request Action
///=============================================================================

///  Append self to request queue and start the request.
- (void)start;

///  Remove self from request queue and cancel the request.
- (void)stop;

///  Convenience method to start the request with block callbacks.
- (void)startWithCompletionBlockWithSuccess:(nullable PAGRequestCompletionBlock)success
                                    failure:(nullable PAGRequestCompletionBlock)failure;

///  Return cancelled state of request task.
@property (nonatomic, readonly, getter=isCancelled) BOOL cancelled;

///  Executing state of request task.
@property (nonatomic, readonly, getter=isExecuting) BOOL executing;

///  Time the request starts.
@property (nonatomic, assign) NSTimeInterval startTime;

///  Time the request ends.
@property (nonatomic, assign) NSTimeInterval endTime;

+ (void)test;

@end

NS_ASSUME_NONNULL_END
