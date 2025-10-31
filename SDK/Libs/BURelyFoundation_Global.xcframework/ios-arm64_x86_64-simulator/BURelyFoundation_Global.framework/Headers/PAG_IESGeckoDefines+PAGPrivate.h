//
//  BU_IESGeckoDefines.h
//  IESGeckoKit
//
//  Created by willorfang on 2017/8/4.
//
//

#import <Foundation/Foundation.h>

////////////////////////////////////////////////////////////////////

NS_ASSUME_NONNULL_BEGIN

#ifndef IES_isEmptyString
#define IES_isEmptyString(param)        ( !(param) ? YES : ([(param) isKindOfClass:[NSString class]] ? (param).length == 0 : NO) )
#endif

#ifndef IES_isEmptyArray
#define IES_isEmptyArray(param)         ( !(param) ? YES : ([(param) isKindOfClass:[NSArray class]] ? (param).count == 0 : NO) )
#endif

#ifndef IES_isEmptyDictionary
#define IES_isEmptyDictionary(param)    ( !(param) ? YES : ([(param) isKindOfClass:[NSDictionary class]] ? (param).count == 0 : NO) )
#endif

#if DEBUG

__unused static NSString *currentTimeString()
{
    NSDateFormatter *dateFormatter = [NSDateFormatter new];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss.SSSSSS"];
    
    return [dateFormatter stringFromDate:[NSDate date]];
}

#define IES_Log(s, ...) \
fprintf(stderr, "%s <%s:%d> %s\n", [currentTimeString() UTF8String], [[[NSString stringWithUTF8String:__FILE__] lastPathComponent] UTF8String], __LINE__, [[NSString stringWithFormat:(s), ##__VA_ARGS__] UTF8String])

#else

#define IES_Log(s, ...)

#endif

#pragma mark - Network param keys

__unused static NSString *kIESGurdRequestConfigLocalInfoKey = @"local";
__unused static NSString *kIESGurdRequestConfigDeploymentsInfoKey = @"deployments";
__unused static NSString *kIESGurdRequestConfigCustomInfoKey = @"custom";
__unused static NSString *kIESGurdNetworkCommonKey = @"common";

#pragma mark - NSSecureCoding - Decode

#define IES_DECODE_STRING(__decoder, __key)     \
self.__key = [__decoder decodeObjectOfClass:[NSString class] forKey:NSStringFromSelector(@selector(__key))]                     \

#define IES_DECODE_INT64(__decoder, __key)      \
self.__key = [[__decoder decodeObjectOfClass:[NSNumber class] forKey:NSStringFromSelector(@selector(__key))] longLongValue]     \

#define IES_DECODE_INTEGER(__decoder, __key)    \
self.__key = [[__decoder decodeObjectOfClass:[NSNumber class] forKey:NSStringFromSelector(@selector(__key))] integerValue]      \

#define IES_DECODE_BOOL(__decoder, __key)       \
self.__key = [[__decoder decodeObjectOfClass:[NSNumber class] forKey:NSStringFromSelector(@selector(__key))] boolValue]         \

#pragma mark - NSSecureCoding - Encode

#define IES_ENCODE_OBJECT(__coder, __key)    \
[__coder encodeObject:self.__key forKey:NSStringFromSelector(@selector(__key))];        \

#define IES_ENCODE_NUMBER(__coder, __key)    \
[__coder encodeObject:@(self.__key) forKey:NSStringFromSelector(@selector(__key))];     \

NS_ASSUME_NONNULL_END


