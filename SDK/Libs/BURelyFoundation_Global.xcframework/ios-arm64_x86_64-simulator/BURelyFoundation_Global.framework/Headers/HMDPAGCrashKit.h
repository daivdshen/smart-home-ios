//
//  HMDPAGCrashKit.h
//  HeimdallrPAG
//
//  Created by yuanzhangjing on 2019/8/23.
//

#import <Foundation/Foundation.h>
@class HMDPAGCrashReportInfo;
NS_ASSUME_NONNULL_BEGIN

@protocol HMDPAGCrashKitDelegate <NSObject>

- (void)crashKitDidDetectCrashForLastTime:(HMDPAGCrashReportInfo *)crashReport;

@optional
- (void)crashKitDidNotDetectCrashForLastTime;

@end

@interface HMDPAGCrashKit : NSObject

@property(nonatomic,weak) id<HMDPAGCrashKitDelegate> delegate;

+ (instancetype)sharedInstance;

- (void)setup;

- (void)setDynamicValue:(NSString *)value key:(NSString *)key;

- (void)removeDynamicValue:(NSString *)key;

@end

NS_ASSUME_NONNULL_END

#define HMDPAGSharedCrashKit [HMDPAGCrashKit sharedInstance]
