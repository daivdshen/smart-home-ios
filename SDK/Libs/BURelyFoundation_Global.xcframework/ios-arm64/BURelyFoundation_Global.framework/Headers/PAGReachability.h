/*
 File: PAGReachability.h
 Abstract: Basic demonstration of how to use the SystemConfiguration Reachablity APIs.
 Version: 3.5
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
 Inc. ("Apple") in consideration of your agreement to the following
 terms, and your use, installation, modification or redistribution of
 this Apple software constitutes acceptance of these terms.  If you do
 not agree with these terms, please do not use, install, modify or
 redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and
 subject to these terms, Apple grants you a personal, non-exclusive
 license, under Apple's copyrights in this original Apple software (the
 "Apple Software"), to use, reproduce, modify and redistribute the Apple
 Software, with or without modifications, in source and/or binary forms;
 provided that if you redistribute the Apple Software in its entirety and
 without modifications, you must retain this notice and the following
 text and disclaimers in all such redistributions of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may
 be used to endorse or promote products derived from the Apple Software
 without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or
 implied, are granted by Apple herein, including but not limited to any
 patent rights that may be infringed by your derivative works or by other
 works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2014 Apple Inc. All Rights Reserved.
 
 */

#import <Foundation/Foundation.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import <netinet/in.h>

typedef NS_ENUM(NSInteger, PAGNetWorkTypeCode) {
    // 初始状态，未连接
    PAGNetWorkTypeCode_None = -1,
    // 未知
    PAGNetWorkTypeCode_Unknown = 0,
    PAGNetWorkTypeCode_Mobile = 1,
    PAGNetWorkTypeCode_2G = 2,
    PAGNetWorkTypeCode_3G = 3,
    PAGNetWorkTypeCode_Wifi = 4,
    PAGNetWorkTypeCode_4G = 5,
    PAGNetWorkTypeCode_5G = 6,
};

typedef NS_ENUM(NSInteger) {
    // 程序无法判断出 App 的网络权限设置
    kBUNetworkAuthorizationStatusCantDetermined = 0,
    // App 未开启蜂窝数据网络权限
    kBUNetworkAuthorizationStatusCellNotPermitted,
    // App 未开启无线局域网与蜂窝移动网络权限，此项仅可能在国行 iPhone 手机上出现
    kBUNetworkAuthorizationStatusWLANAndCellNotPermitted,
} kBUNetworkAuthorizationStatus;

extern NSString *kPAGReachabilityChangedNotification;

// 是否开启 “网络状态检测” 的优化，需要外部调用方以 [[NSUserDefaults standardUserDefaults] setBool:forKey:] 方式写入
extern NSString * const PAGReachabilityDetectOptimizeKey;

@interface PAGReachability : NSObject

/**
 判断指定IP地址的连通性

 @param hostAddress IP地址
 @return PAGReachability对象
 */
+ (instancetype)reachabilityWithAddress:(const struct sockaddr_in *)hostAddress;

/**
 判断网络默认连接的连通性（应用连接到指定host主机时除外）

 @return PAGReachability对象
 */
+ (instancetype)reachabilityForInternetConnection;

/**
 开始在当前runloop监听连通性通知

 @return 是否成功开始监听
 */
- (BOOL)startNotifier;


/**
 结束监听连通性通知
 */
- (void)stopNotifier;


/**
 当前连通性状态

 @return NetworkStatus枚举值
 */
- (PAGNetWorkTypeCode)currentReachabilityStatus;

@end


@interface PAGReachability (Cellular)

/**
 2017.5.22添加
 借鉴微信方式判断当前网络是否连通
 
 @return 网络是否可以联调
 */
+ (BOOL)isNetworkConnected;

@end



