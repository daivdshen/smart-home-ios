//
//  NSArray+OctSafe.h
//  OctCore
//
//  Created by ZY on 2018/12/18.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSArray (OctSafe)

- (id)oct_safe_objectAtIndex:(NSUInteger)index;
+ (BOOL)oct_isEmptyArray:(NSMutableArray *)array;

@end

@interface NSMutableArray (OctSafe)

- (id)oct_safe_objectAtIndex:(NSUInteger)index;
+ (BOOL)oct_isEmptyArray:(NSMutableArray *)array;

@end

NS_ASSUME_NONNULL_END
