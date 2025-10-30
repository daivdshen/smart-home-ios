//
//  NSMutableDictionary+Utilities.h
//  BUAdSDK
//
//  Created by 李盛 on 2018/2/28.
//  Copyright © 2018年 bytedance. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSDictionary (PAG_Helper)

#pragma mark - Safe Value Type From Key
- (nullable NSString *)pag_stringForKey:(NSString *)key defaultValue:(nullable NSString *)defalutValue;
- (BOOL)pag_boolForKey:(NSString *)key defaultValue:(BOOL)defalutValue;
- (NSInteger)pag_integerForKey:(NSString *)key defaultValue:(NSInteger)defalutValue;
- (long)pag_longForKey:(NSString *)key defaultValue:(long)defalutValue;
- (long long)pag_longLongForKey:(NSString *)key defaultValue:(long long)defalutValue;
- (NSTimeInterval)pag_timeIntervalForKey:(NSString *)key defaultValue:(NSTimeInterval)defalutValue;
- (float)pag_floatForKey:(NSString *)key defaultValue:(float)defalutValue;
- (nullable NSArray *)pag_arrayForKey:(NSString *)key defaultValue:(nullable NSArray *)defalutValue;
- (nullable NSDictionary *)pag_dictionaryForKey:(NSString *)key defaultValue:(nullable NSDictionary *)defalutValue;

@end

@interface NSDictionary(PAG_JSONValue)

- (nullable NSString *)pag_JSONRepresentation:(NSError **)error;
+ (nullable NSDictionary *)pag_dictionaryWithJSONData:(nullable NSData *)inData error:(NSError **)outError;
+ (nullable NSDictionary *)pag_dictionaryWithJSONString:(NSString *)inJSON error:(NSError **)outError;




- (void)pag_forEachWithBlock:(void(^)(id key, id value))block;





@end

@interface NSDictionary(PAG_Formate)

- (void)pag_parserWithKey:(NSString *)key stringValue:(NSString *_Nullable*_Nullable)stringValue dictValue:(NSDictionary *_Nullable*_Nullable)dictValue aryValue:(NSArray *_Nullable*_Nullable)aryValue;

@end

@interface NSMutableDictionary (PAG_Helper)

#pragma mark - Safe Set Object For Key
- (void)pag_setObject:(nullable id)object forKey:(nullable id<NSCopying>)key;

@end

NS_ASSUME_NONNULL_END
