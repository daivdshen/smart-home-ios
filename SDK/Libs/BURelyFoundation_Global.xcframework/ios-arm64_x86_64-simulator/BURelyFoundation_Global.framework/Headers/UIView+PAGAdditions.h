//
//  UIView+Additions.h
//  BUAdSDK
//
//  Created by bytedance_yuanhuan on 2018/3/15.
//  Copyright © 2018年 bytedance. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef enum {
    UIViewBorderOptionTop = 0,
    UIViewBorderOptionRight,
    UIViewBorderOptionBottom,
    UIViewBorderOptionLeft,
    UIViewBorderOptionAll
} UIViewBorderOption;


typedef struct {
  CGFloat topLeft;
  CGFloat topRight;
  CGFloat bottomLeft;
  CGFloat bottomRight;
} BUFCornerRadii;


@interface UIView (PAG_Border)

- (void)pag_setBorder:(UIViewBorderOption)option width:(CGFloat)width color:(UIColor *)color;
- (void)pag_setDashBorder:(UIViewBorderOption)option width:(CGFloat)width color:(UIColor *)color;
- (void)pag_updateClippingForLayer:(CALayer *)layer cornerRadii:(BUFCornerRadii)cornerRadii maxRadius:(CGFloat)maxRadius;
@end


@interface UIView (PAG_FrameAdditions)
@property (nonatomic) float pag_x;
@property (nonatomic) float pag_y;
@property (nonatomic) float pag_width;
@property (nonatomic) float pag_height;
@property (nonatomic, getter = pag_y,setter = setPag_y:) float pag_top;    // 增加bu前缀，防止与外部开发者的分类属性名冲突：https://jira.bytedance.com/browse/UNION-4447 fixed in 3300 by chaors
@property (nonatomic, getter = pag_x,setter = setPag_x:) float pag_left;
@property (nonatomic) float pag_bottom;
@property (nonatomic) float pag_right;
@property (nonatomic) CGSize pag_size;
@property (nonatomic) CGPoint pag_origin;
@property (nonatomic) CGFloat pag_centerX;
@property (nonatomic) CGFloat pag_centerY;

- (UIViewController *)pag_viewController;
@end

@interface UIView (PAG_Gesture)

@property (nonatomic, strong, nullable) UITapGestureRecognizer *pag_tgr;
// 会移除旧的手势
- (void)pag_addGestureRecognizerWithTarget:(id)target action:(SEL)action;
- (void)pag_removeGestureRecognizer;
@end

@interface UIView (PAG_FindFirstResponder)
- (UIView *)pag_findViewThatIsFirstResponder;
@end

@interface UIView (PAG_SafeArea)
- (UIEdgeInsets)pag_safeAreaInsets;

+ (UIEdgeInsets)pag_defaultAreaInsets;

@end
NS_ASSUME_NONNULL_END
