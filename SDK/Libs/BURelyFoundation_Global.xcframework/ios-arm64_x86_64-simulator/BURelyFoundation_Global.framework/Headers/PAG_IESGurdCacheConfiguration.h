//
//  PAG_IESGurdCacheConfiguration.h
//  IESGeckoKit
//
//  Created by chenyuchuan on 2019/6/6.
//

#import <Foundation/Foundation.h>

#import "PAG_IESGeckoDefines.h"

NS_ASSUME_NONNULL_BEGIN

@interface PAG_IESGurdCacheConfiguration : NSObject

@property (nonatomic, assign) IESGurdCleanCachePolicy cachePolicy;

@property (nonatomic, assign) NSInteger channelLimitCount;

@end

@interface PAG_IESGurdCacheConfiguration (Conveniences)

/**
 初始化FIFO配置
 默认channelLimitCount为10
 */
+ (instancetype)FIFOConfiguration;

@end

NS_ASSUME_NONNULL_END
