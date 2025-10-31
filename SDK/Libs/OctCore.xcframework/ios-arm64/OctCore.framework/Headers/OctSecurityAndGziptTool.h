//
//  OctSecurityAndGziptTool.h
//  OctCoreSDK
//
//  Created by guantou on 2020/1/10.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/*
 *  解压缩，加密、解密。
 */
@interface OctSecurityAndGziptTool : NSObject

//  Gzipt压缩
+ (NSData *)AdGzipData:(NSData*)uncompressedData;
//  Gzipt解压
+ (NSData *)AdUngzipData:(NSData *)compressedData;

/// oct加密
/// @param encryptString 需要加密的字符串
+ (NSData *)OctEncryptWithString:(NSString *)encryptString aesKey:(NSString*)key aesIV:(NSString*)aesIV;

/// oct解密
/// @param decryptString 需要解密的字符串
+ (NSData *)OctDecryptWithString:(NSString *)decryptString aesKey:(NSString*)key aesIV:(NSString*)aesIV;

+ (NSData *)OctAESEncryptWithString:(NSString *)encryptString aesKey:(NSString*)key aesIV:(NSString*)aesIV;
+ (NSData *)OctAESDecryptWithString:(NSString *)decryptString aesKey:(NSString*)key aesIV:(NSString*)aesIV;

//可选加解密
+ (NSData *)OctOptionEncryptWithString:(NSString *)encryptString aesKey:(NSString*)key aesIV:(NSString*)aesIV;
+ (NSData *)OctOptionDecryptWithString:(NSString *)decryptString aesKey:(NSString*)key aesIV:(NSString*)aesIV;

+ (NSData *)OctOptionAESEncryptWithString:(NSString *)encryptString aesKey:(NSString*)key aesIV:(NSString*)aesIV;
+ (NSData *)OctOptionAESDecryptWithString:(NSString *)decryptString aesKey:(NSString*)key aesIV:(NSString*)aesIV;


@end

NS_ASSUME_NONNULL_END
