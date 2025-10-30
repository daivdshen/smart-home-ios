//
//  OctNetworkStatusTools.h
//  OctCoreSDK
//
//  Created by guantou on 2019/12/10.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/*
 *  网络状态类
 */
typedef NS_ENUM(NSInteger, OctConnectionType) {
    OctConnectionUnknown          = -1,
    OctConnectionNotReachable     = 0,
    OctConnectionNetwork2G        = 2,
    OctConnectionNetwork3G        = 3,
    OctConnectionNetwork4G        = 4,
    OctConnectionWiFi             = 9,
};

typedef NS_ENUM(NSInteger, OctOperatorType) {
    OctOperatorUnknown     = 0,
    OctOperatorCNMobile    = 1, // 中国移动
    OctOperatorCNUnicom    = 2, // 中国联通
    OctOperatorCNTelecom   = 3, // 中国电信
};

typedef NS_ENUM(NSInteger, OctNetworkStatus) {
    OctNetworkUnknown       = -1,
    OctNetworkNotReachable  = 0,
    OctNetworkWWAN          = 1,
    OctNetworkWIFI          = 9,
};

UIKIT_EXTERN OctNetworkStatus baseOctNetworkStatus;

typedef void(^OctNetworkingStatusBlock)(OctNetworkStatus status);

@interface OctNetworkStatusTools : NSObject

@property (nonatomic, readonly) OctNetworkStatus networkStatus; // 返回网络状态，WIFI/运营商网络/无网络
@property (nonatomic, readonly) OctOperatorType carrierType; // 运营商类型，移动/联通/电信/其它
@property (nonatomic, readonly) OctConnectionType connectionType;   // 连接类型，2G/3G/4G/WIFI
@property (nonatomic, readonly) NSString *ipAddress;
@property (nonatomic, readonly) NSString *userAgent;

+ (OctNetworkStatusTools *)shareInstance;

+ (void)ljx_startNetWorkMonitoringWithBlock:(OctNetworkingStatusBlock)networkStatus;

@end

NS_ASSUME_NONNULL_END
