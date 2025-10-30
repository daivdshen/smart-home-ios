//
//  UIView+OctFrame.h
//  OctCore
//
//  Created by ZY on 2019/6/6.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface UIView (OctFrame)

@property (nonatomic ,assign) CGFloat oct_x;
@property (nonatomic ,assign) CGFloat oct_y;
@property (nonatomic ,assign) CGFloat oct_width;
@property (nonatomic ,assign) CGFloat oct_height;
@property (nonatomic ,assign) CGFloat oct_centerX;
@property (nonatomic ,assign) CGFloat oct_centerY;
@property (nonatomic ,assign) CGSize oct_size;
@property (nonatomic, assign) CGFloat oct_right;
@property (nonatomic, assign) CGFloat oct_bottom;

@end

NS_ASSUME_NONNULL_END
