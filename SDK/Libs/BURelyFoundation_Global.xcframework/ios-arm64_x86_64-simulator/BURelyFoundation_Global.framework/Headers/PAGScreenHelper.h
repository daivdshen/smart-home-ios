//
//  BDUScreenHelp.h
//  BUAdSDK
//
//  Created by bytedance_yuanhuan on 2018/11/28.
//  Copyright © 2018年 bytedance. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//设备类型
typedef NS_ENUM(NSInteger, PAGDeviceMode) {
    //iPad
    PAGDeviceModePad,
    //iPhone6plus,iPhone6Splus
    PAGDeviceMode736,
    //iPhone6,iPhone6S
    PAGDeviceMode667,
    //iPhone5,iPhone5C,iPhone5S,iPhoneSE
    PAGDeviceMode568,
    //iPhone4,iPhone4s
    PAGDeviceMode480,
    //iPhoneX,iphoneXS
    PAGDeviceMode812,
    //iphoneXR,iphoneRS Max
    PAGDeviceMode896
};

NS_ASSUME_NONNULL_BEGIN

@interface PAGScreenHelper: NSObject

/**
 *  判断设备是iPhoneX,iphoneXS
 *
 *  @return Yes or No
 */
+ (BOOL)is812Screen;

/**
 *  判断设备是iPad
 *
 *  @return Yes or No
 */
+ (BOOL)isPadDevice;

/**
 *  获取设备类型
 *
 *  @return BUDeviceType类型
 */
+ (PAGDeviceMode)getDeviceType;

/**
 *  分辨率，区分横竖屏，形如@"414*736"
 *  @return 横竖屏返回样式，横屏样式@"736*414"，竖屏样式@"414*736"
 */
+ (nullable NSString *)resolutionString;

/**
 *  分辨率，区分横竖屏，形如@"414x736"
 *  @return 横竖屏返回样式，横屏样式@"736x414"，竖屏样式@"414x736"
 */
+ (NSString *)displayDensity;

@end

NS_ASSUME_NONNULL_END
