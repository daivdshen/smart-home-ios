//
//  UIView+OctAddition.h
//  OCTOPUS
//
//  Created by zwf on 2020/12/8.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface UIView (OctAddition)

@property (nonatomic, assign) CGPoint oct_origin;
@property (nonatomic, assign) CGSize oct_size;

@property (nonatomic, assign) CGFloat oct_height;
@property (nonatomic, assign) CGFloat oct_width;

@property (nonatomic, assign) CGFloat oct_top;
@property (nonatomic, assign) CGFloat oct_left;
@property (nonatomic, assign) CGFloat oct_bottom;
@property (nonatomic, assign) CGFloat oct_right;

@property (nonatomic, readonly) CGPoint oct_bottomLeft;
@property (nonatomic, readonly) CGPoint oct_bottomRight;
@property (nonatomic, readonly) CGPoint oct_topRight;

- (void)oct_setupCornerRadius:(CGFloat)cornerRadius;
- (void)oct_setupCornerRadius:(CGFloat)cornerRadius withBorderWidth:(CGFloat)width borderColor:(UIColor *)color;
- (void)oct_setupCornerRadius:(CGFloat)cornerRadius withType:(UIRectCorner)cornerType;

- (void)oct_fitWidth;
- (void)oct_fitHeight;

+ (UIWindow *)oct_lastWindow;

- (void)addTopShadowLineWithColor:(UIColor *)theColor;

@end

NS_ASSUME_NONNULL_END
