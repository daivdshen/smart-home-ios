//
//  PAGNetworkAgent.h
//  BUAdSDK
//
//  Created by 李盛 on 2018/4/2.
//  Copyright © 2018年 bytedance. All rights reserved.
//

#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, PAGSSLPinningMode) {
    BU_SSLPinningModeNone,
    BU_SSLPinningModePublicKey,
    BU_SSLPinningModeCertificate
};

typedef NSURLSessionAuthChallengeDisposition (^PAGURLSessionDidReceiveAuthenticationChallengeBlock)(NSURLSession *session, NSURLAuthenticationChallenge *challenge, NSURLCredential * _Nullable __autoreleasing * _Nullable credential);

@class PAGBaseRequest;


@interface PAGNetworkAgent : NSObject

- (instancetype)init;
+ (instancetype)new NS_UNAVAILABLE;

///  Get the shared agent.
+ (PAGNetworkAgent *)sharedAgent;

/// network is DEBUG
+ (void)openNetWorkDebug;

///  Add request to session and start it.
- (void)addRequest:(PAGBaseRequest *)request;

///  Cancel a request that was previously added.
- (void)cancelRequest:(PAGBaseRequest *)request;

///  Return the constructed URL of request.
///
///  @param request The request to parse. Should not be nil.
///
///  @return The result URL.
- (NSString *)buildRequestUrl:(PAGBaseRequest *)request;

/// set custom AuthenticationChanllengeBlock, do not set it when using shareAgent, you can use it when using another instance of PAGNetworkAgent
- (void)setSessionDidReceiveAuthenticationChanllengeBlock:(PAGURLSessionDidReceiveAuthenticationChallengeBlock)challengeBlock;

- (BOOL)evaluateServerTrust:(SecTrustRef)serverTrust
                  forDomain:(nullable NSString *)domain;

@end

NS_ASSUME_NONNULL_END
