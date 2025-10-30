//
//  HMDPAGNetworkInjector.h
//  HeimdallrPAG
//
//  Created by fengyadong on 2021/5/18.
//

#import <Foundation/Foundation.h>

typedef NSData *_Nullable (^HMDPAGNetEncryptBlock)(NSData *_Nullable);

@interface HMDPAGNetworkInjector : NSObject

+ (nonnull instancetype)sharedInstance;

- (void)configEncryptBlock:(HMDPAGNetEncryptBlock _Nullable)encryptBlock;
- (HMDPAGNetEncryptBlock _Nullable)encryptBlock;

@end
