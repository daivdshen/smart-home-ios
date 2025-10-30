//
//  HeimdallrPAGEmbedConfig.h
//  HeimdallrPAGEmbed
//
//  Created by xuminghao.eric on 2020/12/14.
//

#import <Foundation/Foundation.h>
#import "HMDPAGEmbedAddressRange.h"

NS_ASSUME_NONNULL_BEGIN

@interface HeimdallrPAGEmbedConfig : NSObject

/**
 这个 sdkID 只做客户端标识用，不决定日志上报，APMPlus 上的 appID 需要传给 uploadAid
 如果不设置该属性，将自动填充为 uploadAid
 */
@property (atomic, copy) NSString *sdkID;

/**
 apm上报host
 */
@property (atomic, copy) NSString *domain;

/**
 自定义维度，配置后崩溃日志和 PV 日志上报会携带该数据
 */
@property (nonatomic, strong) NSDictionary<NSString *, NSString *> *filter;

/**
 SDK 符号的地址区间，用于客户端过滤 SDK 发生的崩溃
 */
@property (atomic, strong) NSArray<HMDPAGEmbedAddressRange *> *addressRanges;

/**
 在 APMPlus 上创建应用后，得到的应用 ID ，配置该字段以确定日志上报到 APMPlus 的哪个所属应用
 */
@property (atomic, copy) NSString *uploadAid;

/**
 设备标识符
 */
@property (atomic, copy) NSString *deviceID;

/**
 SDK 版本
 */
@property (atomic, copy) NSString *sdkVersion;

/**
 宿主应用标识符，应为 SDK 产品体系中为宿主应用分配的标识符（如果有的话）
 */
@property (atomic, copy) NSString *hostAppID;

/**
 动态库名称，用于客户端过滤 SDK 发生的崩溃
 */
@property (atomic, strong) NSArray<NSString *> *libNames;

/**
 单例，一些模块（例如PV）的单例的配置参数将使用此实例，强烈建议使用此单例
 */
+ (instancetype)defaultConfig;

@end

NS_ASSUME_NONNULL_END
