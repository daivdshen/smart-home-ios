//
//  HMDPAGEmbedSessionTracker.h
//  HeimdallrPAGForEmbed
//
//  Created by xuminghao.eric on 2022/10/28.
//

#import <Foundation/Foundation.h>
#import "HeimdallrPAGEmbedConfig.h"

NS_ASSUME_NONNULL_BEGIN

@interface HMDPAGEmbedSessionTracker : NSObject

/**
 单例，使用 defaultConfig 作为配置参数
 */
+ (instancetype)sharedInstance;

/**
 可以传入任意一个 HeimdallrPAGEmbedConfig 作为配置参数
 */
+ (instancetype)sessionTrackerWithConfig:(HeimdallrPAGEmbedConfig *)config;

/**
 记录一条 PV 日志，记录后将立即上报
 */
- (void)recordSession;

@end

NS_ASSUME_NONNULL_END
