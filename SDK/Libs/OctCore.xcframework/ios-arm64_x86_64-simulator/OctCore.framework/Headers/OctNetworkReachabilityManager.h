//
//  OctNetworkReachabilityManager.h
//  OctCoreSDK
//
//  Created by guantou on 2023/10/10.
//

#import <Foundation/Foundation.h>

#if !TARGET_OS_WATCH
#import <SystemConfiguration/SystemConfiguration.h>

typedef NS_ENUM(NSInteger, OctNetworkReachabilityStatus) {
    OctNetworkReachabilityStatusUnknown          = -1,
    OctNetworkReachabilityStatusNotReachable     = 0,
    OctNetworkReachabilityStatusReachableViaWWAN = 1,
    OctNetworkReachabilityStatusReachableViaWiFi = 2,
};

NS_ASSUME_NONNULL_BEGIN

@interface OctNetworkReachabilityManager : NSObject

@property (readonly, nonatomic, assign) OctNetworkReachabilityStatus networkReachabilityStatus;

@property (readonly, nonatomic, assign, getter = isReachable) BOOL reachable;

@property (readonly, nonatomic, assign, getter = isReachableViaWWAN) BOOL reachableViaWWAN;

@property (readonly, nonatomic, assign, getter = isReachableViaWiFi) BOOL reachableViaWiFi;

+ (instancetype)sharedManager;

+ (instancetype)manager;

+ (instancetype)managerForDomain:(NSString *)domain;

+ (instancetype)managerForAddress:(const void *)address;

- (instancetype)initWithReachability:(SCNetworkReachabilityRef)reachability NS_DESIGNATED_INITIALIZER;

+ (instancetype)new NS_UNAVAILABLE;

- (instancetype)init NS_UNAVAILABLE;

- (void)startMonitoring;

- (void)stopMonitoring;

- (NSString *)localizedNetworkReachabilityStatusString;

- (void)setReachabilityStatusChangeBlock:(nullable void (^)(OctNetworkReachabilityStatus status))block;

@end

FOUNDATION_EXPORT NSString * const OctNetworkingReachabilityDidChangeNotification;
FOUNDATION_EXPORT NSString * const OctNetworkingReachabilityNotificationStatusItem;

FOUNDATION_EXPORT NSString * OctStringFromNetworkReachabilityStatus(OctNetworkReachabilityStatus status);

NS_ASSUME_NONNULL_END
#endif
