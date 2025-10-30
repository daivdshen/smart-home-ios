//
//  OctAdViewAttribute.h
//  OctCoreSDK
//
//  Created by guantou on 2019/12/10.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface OctAdViewAttribute : NSObject

/**
 视图透明度
 */
+ (CGFloat)oct_getSuperViewsAlpha:(UIView *)view;

/**
 视图是否可见
 */
+ (BOOL)oct_getSuperViewsVisibility:(UIView *)view;

/**
 视图是否被遮挡
 */
+ (BOOL)oct_getSuperViewsShade:(UIView *)view;

/**
 视图状态压缩后的字符串
 */
+ (NSString *)octGetStatusWithView:(UIView *)judgeView;

@end

NS_ASSUME_NONNULL_END
