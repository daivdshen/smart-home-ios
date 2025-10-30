//
//  UIView+OctAttribute.h
//  OctCore
//
//  Created by ZY on 2019/1/16.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface UIView (OctAttribute)

/**
 视图透明度
 */
- (CGFloat)oct_getSuperViewsAlpha:(UIView *)view;

/**
 视图是否可见
 */
- (BOOL)oct_getSuperViewsVisibility:(UIView *)view;

/**
 视图是否被遮挡
 */
- (BOOL)oct_getSuperViewsShade:(UIView *)view;

/**
 视图是否被添加到页面
 */
- (BOOL)oct_isDisplayedInScreen;

@end

NS_ASSUME_NONNULL_END
