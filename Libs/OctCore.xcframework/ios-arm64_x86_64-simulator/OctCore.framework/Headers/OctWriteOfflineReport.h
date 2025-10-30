//
//  OctWriteOfflineReport.h
//  OctCoreSDK
//
//  Created by guantou on 2020/3/2.
//

#import <Foundation/Foundation.h>
#import "OctAdvertisingType.h"
#import "OctAdRecordModel.h"
#import "OctConfigureModel.h"

NS_ASSUME_NONNULL_BEGIN

@interface OctWriteOfflineReport : NSObject

/// 写入离线日志
/// @param dataString 替换好的写入内容
/// @param domain 文件目录z地址（未MD5）
/// @param adRecordModel 给SB实现内容逻辑需要model
/// @param channelType type
/// @param eventCode 事件码
/// @param spin 是否自旋
+ (void)writeLogWithString:(NSString *)dataString Domain:(NSString *)domain Model:(OctAdRecordModel *)adRecordModel channel:(OctChannelType)channelType eventCode:(NSString *)eventCode iSSpin:(BOOL)spin;
//  检查离线日志有效性
+ (void)checkLogValidity:(OctDataModel *)dataModel;

@end

NS_ASSUME_NONNULL_END
