//
//  NSObject+OctModel.h
//  TestView
//
//  Created by guantou on 2020/1/6.
//  Copyright © 2020 北京市吕俊学. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSObject (OctModel)

+ (nullable instancetype)oct_modelWithJSON:(id)json;

+ (nullable instancetype)oct_modelWithDictionary:(NSDictionary *)dictionary;

- (BOOL)oct_modelSetWithJSON:(id)json;

- (BOOL)oct_modelSetWithDictionary:(NSDictionary *)dic;

- (nullable id)oct_modelToJSONObject;

- (nullable NSData *)oct_modelToJSONData;

- (nullable NSString *)oct_modelToJSONString;

- (nullable id)oct_modelCopy;

- (void)oct_modelEncodeWithCoder:(NSCoder *)aCoder;

- (id)oct_modelInitWithCoder:(NSCoder *)aDecoder;

- (NSUInteger)oct_modelHash;

- (BOOL)oct_modelIsEqual:(id)model;

- (NSString *)oct_modelDescription;

@end

@interface NSArray (OctModel)

+ (nullable NSArray *)oct_modelArrayWithClass:(Class)cls json:(id)json;

@end

@interface NSDictionary (OctModel)

+ (nullable NSDictionary *)oct_modelDictionaryWithClass:(Class)cls json:(id)json;

@end

@protocol OctModel <NSObject>
@optional

+ (nullable NSDictionary<NSString *, id> *)oct_modelCustomPropertyMapper;

+ (nullable NSDictionary<NSString *, id> *)oct_modelContainerPropertyGenericClass;

+ (nullable Class)oct_modelCustomClassForDictionary:(NSDictionary *)dictionary;

+ (nullable NSArray<NSString *> *)oct_modelPropertyBlacklist;

+ (nullable NSArray<NSString *> *)oct_modelPropertyWhitelist;

- (NSDictionary *)oct_modelCustomWillTransformFromDictionary:(NSDictionary *)dic;

- (BOOL)oct_modelCustomTransformFromDictionary:(NSDictionary *)dic;

- (BOOL)oct_modelCustomTransformToDictionary:(NSMutableDictionary *)dic;

@end

NS_ASSUME_NONNULL_END

