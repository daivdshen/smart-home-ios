//
//  NSString+OctAddition.h
//  OCTOPUS
//
//  Created by zwf on 2020/12/8.
//

#import <UIKit/UIKit.h>


NS_ASSUME_NONNULL_BEGIN

@interface NSString (OctAddition)

/// 字符串的MD5值 以十六进制标识
/// @param string 字符串
+ (NSString *)oct_md5HexFromString:(NSString *)string;

+ (NSString *)oct_hexStringFromString:(NSString *)string;

+ (NSString *)oct_currentTimeIntervalString;

- (CGFloat)oct_floatFromString;

- (UIColor *)oct_colorWithHex:(NSString *)hexColor alpha:(float)alpha;

- (CGSize)caleSizeByWidth:(CGFloat)width fontSize:(CGFloat)fontSize;

@end

NS_ASSUME_NONNULL_END
