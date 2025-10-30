//
//  PAGDeviceHelper.h
//  BUSDKProject
//
//  Created by ranny_90 on 2017/5/20.
//  Copyright © 2017年 ranny_90. All rights reserved.
//
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface PAGDeviceHelper : NSObject

/// 判断设备是否越狱
+ (BOOL)bu_isJailBroken;

/// 获取idfv
+ (NSString *)bu_idfvString;

/// 获取uuid
+ (NSString *)bu_uuid;

/// 获取系统版本号
+ (float)bu_OSVersionNumber;

 /// 获取当前语言种类
+ (nullable NSString *)currentLanguage;

///是否锁屏
+ (BOOL)is_screen_off;

///电池状态
+ (UIDeviceBatteryState)batteryState;

///电池电量
+ (float)battery_remaining_pct;

///ATT状态
+ (NSUInteger)trackingAuthorizationStatus;

/// iOS 系统版本
+ (NSString *)systemVersion;

/// 设备的运行环境
+ (NSString *)pagDeviceState;

@end


@interface PAGDeviceHelper (Hardware)

/// 返回设备平台信息
+ (NSString *)platform;

/// 返回设备Model
+ (NSString *)model;

/// 返回设备type：iphone／ipad／ipod／apple tv等
+ (NSString *)bu_platformTypeString;

/// 具体到型号，如iPhone1,1
+ (NSString *)bu_platformString;

/// 返回硬盘空闲空间
+ (NSNumber *)freeDiskSpace;

/// 设备的总内存 单位MB
+ (NSInteger)totolDeviceMemory;

/// APP已使用的内存 单位MB
+ (NSInteger)usedAPPMemory;

/// 获取系统编译版本号
+ (NSString *)bu_buildVersionNumber;

@end

NS_ASSUME_NONNULL_END
