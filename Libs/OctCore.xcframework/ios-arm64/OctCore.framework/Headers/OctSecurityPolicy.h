//
//  OctSecurityPolicy.h
//  OctCoreSDK
//
//  Created by guantou on 2023/10/10.
//

#import <Foundation/Foundation.h>
#import <Security/Security.h>

typedef NS_ENUM(NSUInteger, OctSSLPinningMode) {
    OctSSLPinningModeNone,
    OctSSLPinningModePublicKey,
    OctSSLPinningModeCertificate,
};

NS_ASSUME_NONNULL_BEGIN

@interface OctSecurityPolicy : NSObject <NSSecureCoding, NSCopying>

@property (readonly, nonatomic, assign) OctSSLPinningMode SSLPinningMode;

@property (nonatomic, strong, nullable) NSSet <NSData *> *pinnedCertificates;

@property (nonatomic, assign) BOOL allowInvalidCertificates;

@property (nonatomic, assign) BOOL validatesDomainName;

+ (NSSet <NSData *> *)certificatesInBundle:(NSBundle *)bundle;

+ (instancetype)defaultPolicy;

+ (instancetype)policyWithPinningMode:(OctSSLPinningMode)pinningMode;

+ (instancetype)policyWithPinningMode:(OctSSLPinningMode)pinningMode withPinnedCertificates:(NSSet <NSData *> *)pinnedCertificates;

- (BOOL)evaluateServerTrust:(SecTrustRef)serverTrust
                  forDomain:(nullable NSString *)domain;

@end

NS_ASSUME_NONNULL_END


