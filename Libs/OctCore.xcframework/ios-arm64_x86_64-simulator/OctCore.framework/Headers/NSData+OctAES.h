//
//  NSData+OctAES.h
//  OctCoreSDK
//
//  Created by guantou on 2019/12/30.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSData (OctAES)

//加密
- (NSData *)OctAES128EncryptWithKey:(NSString *)key iv:(NSString *)iv;


//解密
- (NSData *)OctAES128DecryptWithKey:(NSString *)key iv:(NSString *)iv;

@end

NS_ASSUME_NONNULL_END
