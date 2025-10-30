//
//  NSMutableArray+Utilities.h
//  BUAdSDK
//
//  Created by 李盛 on 2018/2/28.
//  Copyright © 2018年 bytedance. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSMutableArray (PAG_Utilities)

- (void)pag_safeAddObject:(id)object;

@end

@interface NSArray(PAG_JSONValue)
- (nullable NSString *)pag_JSONRepresentation:(NSError **)error;

- (void)pag_forEachWithBlock:(void(^)(id item))block;
- (void)pag_forEachWithIndexBlock:(void(^)(id item, NSInteger index))indexBlock;

- (NSArray *)pag_mapWithBlock:(id(^)(id item))block;
- (NSArray *)pag_mapWithIndexBlock:(id(^)(id item, NSInteger index))indexBlock;

- (NSArray *)pag_filterWithConditionBlock:(BOOL(^)(id item))conditionBlock;
- (NSArray *)pag_filterWithIndexConditionBlock:(BOOL(^)(id item, NSInteger index))indexConditionBlock;

- (BOOL)pag_someWithConditionBlock:(BOOL(^)(id item))conditionBlock;
- (BOOL)pag_someWithIndexConditionBlock:(BOOL(^)(id item, NSInteger index))indexConditionBlock;

- (BOOL)pag_everyWithConditionBlock:(BOOL(^)(id item))conditionBlock;
- (BOOL)pag_everyWithIndexConditionBlock:(BOOL(^)(id item, NSInteger index))indexConditionBlock;

- (id)pag_reduceWithBlock:(id(^)(id item1, id item2))block initial:(id)initial;



- (id)pag_findWithConditionBlock:(id(^)(id item))conditionBlock;
- (id)pag_findWithIndexConditionBlock:(id(^)(id item, NSInteger index))indexConditionBlock;
@end

NS_ASSUME_NONNULL_END
