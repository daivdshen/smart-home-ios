//
//  HeimdallrPAGEmbed.h
//  HeimdallrPAGEmbed
//
//  Created by xuminghao.eric on 2020/12/14.
//

#import <Foundation/Foundation.h>
#import "HeimdallrPAGEmbedConfig.h"

NS_ASSUME_NONNULL_BEGIN

@interface HeimdallrPAGEmbed : NSObject

/**
 注册一个 SDK 的配置，用于崩溃监控
 */
+ (void)registerSDKWithConfig:(HeimdallrPAGEmbedConfig *)config;

/**
 初始化崩溃监控（需要先注册 SDK 配置）
 */
+ (void)start;

@end

NS_ASSUME_NONNULL_END
