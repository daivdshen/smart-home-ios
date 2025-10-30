//
//  NSString+URLEncoding.h
//  BUAdSDK
//
//  Created by carl on 2017/10/26.
//  Copyright © 2017年 bytedance. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, PAGLanguageType) {//"万"转换规则
    PAGLanguageType_showWan     = 0,         //直接拼接万,默认
    PAGLanguageType_showNum     = 1,         //直接展示数字
};

@interface NSString (PAG_URLCoding)
- (nullable NSString *)pag_URLEncodedString;
- (nullable NSString *)pag_URLDecodedString;
- (nullable NSString *)pag_URLEncodedStringWithCustomSet;
@end

@interface NSString (PAG_Encryption)
/**
 * @brief 返回自身的md5
 * @return 返回自身的md5的16进制字串
 */
- (nullable NSString *)pag_MD5HashString;
@end

@interface NSString (PAG_NumberToWan)

/// 大于1w就用k表示，不大于就直接展示多少个评分
+ (NSString *)pag_numberToThousand:(NSInteger)target;


/// 0-9999 = 实际数值
/// 1w - 9999w = (实际数值 / 10000) w+
/// 1亿+ = 9999w+
/// @param target 实际数值
/// @param name 单位 如w/万
+ (NSString *)pag_numberShortToW:(NSInteger)target name:(nonnull NSString *)name;

@end

@interface NSString (PAG_URLStringAppend)
/*
 *
 string转URL
 */
+ (nullable NSURL *)pag_URLWithURLString:(NSString *)str;
/*
 *
 string转字典
 */
+ (NSDictionary*)pag_parametersOfURLQueryString:(NSString*)urlString;

/*
 *
 url字符串拼接参数,需要判断是否是第一个
 */
+ (NSString *)pag_urlStringWithOriginUrlString:(NSString *)originUrlString appendParameters:(NSDictionary *)parameters;

+ (NSString *)pag_urlStringWithBaseUrlString:(NSString *)baseUrlString requestURLString:(NSString *)requestURLString;

+ (NSString *)pag_urlStringWithUrlString:(NSString *)urlString parameterString:(NSString *)parameterString;

/*
 * 格式转化
   222222 -> 222,222
 */
+ (NSString *)pag_convertNumberFormatter:(NSString *)str;



/// 正则匹配字符
/// @param pattern 正则表达式
/// @param withTemplate 返回样式: "$1 + $2"
/// @param error 错误信息
- (nullable NSString *)pag_replacingWithPattern:(NSString * _Nonnull)pattern withTemplate:(NSString * _Nonnull)withTemplate error:(NSError **)error;
@end


@interface NSString (PAG_Sandbox)
/**
 *  获取缓存路径
 *
 *  @return path where to cache
 */
- (NSString *)pag_CachePath;

@end


@interface NSString(PAG_JSONValue)

- (nullable id)pag_JSONValue:(NSError **)error;

@end


@interface NSString(PAG_Time)

+ (NSString*)pag_dateNowString;

+ (NSNumber*)pag_currentInterval;

@end


@interface NSString(PAG_Random)

/*  @deccription生成随机字符
 *  @param count 多少位随机数
 */
+ (NSString *)pag_randomStringWithCount:(NSUInteger)count;
@end

@interface NSString (PAG_Size)

- (CGSize)pag_boundingRectWithSize:(CGSize)size font:(UIFont *)font;

@end

@interface NSString (PAG_CheckingResult)

- (nullable NSString *)pag_substringWithPattern:(NSString *)pattern;

@end

NS_ASSUME_NONNULL_END
