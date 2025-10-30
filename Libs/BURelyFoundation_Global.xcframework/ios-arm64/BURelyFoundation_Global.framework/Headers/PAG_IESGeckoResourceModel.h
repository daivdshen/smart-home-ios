//
//  PAG_IESGurdResourceModel.h
//  PAG_IESGurdKit
//
//  Created by 01 on 17/6/30.
//

#import <Foundation/Foundation.h>

#import "PAG_IESGeckoDefines.h"

#define IESGeckoResourceModel   PAG_IESGurdResourceModel

NS_ASSUME_NONNULL_BEGIN

//Gurd内部配置拦截前缀 channel
extern NSString * const PAG_kIESGurdOfflinePrefixChannel;

@interface PAG_IESGurdResourceStrategies : NSObject

@property (nonatomic, assign) BOOL deleteIfDownloadFailed;
@property (nonatomic, assign) BOOL deleteBeforeDownload;

@end

@interface PAG_IESGurdResourceURLInfo : NSObject

// 包ID
@property (nonatomic, assign) long long ID;

// 下载URL
@property (nonatomic, copy) NSString *url;

// 下载URL列表(包含url)
@property (nonatomic, copy) NSArray<NSString *> *urlList;

// 包MD5值
@property (nonatomic, copy) NSString *md5;

@end

@interface PAG_IESGurdResourceModel : NSObject

// 版本号
@property (nonatomic, assign) long long version;

// 所属频道
@property (nonatomic, copy) NSString *channel;

// 文件类型
@property (nonatomic, assign) IESGurdChannelFileType packageType;

// 全量包信息
@property (nonatomic, strong) PAG_IESGurdResourceURLInfo *package;

// 增量包信息
@property (nonatomic, strong) PAG_IESGurdResourceURLInfo * _Nullable patch;

// 离线包策略
@property (nonatomic, strong) PAG_IESGurdResourceStrategies * _Nullable strategies;

// 需要解压
@property (nonatomic, readonly, assign) BOOL needUnzip;

// 组名
@property (nonatomic, copy) NSString *groupName;

@property (nonatomic, readonly, copy) NSString *logId;

// 当channel为BU_kIESGurdOfflinePrefixChannel时有效
@property (nonatomic, copy) NSArray<NSString *> *offlinePrefixURLsArray;

+ (instancetype _Nullable)instanceWithDict:(NSDictionary *)dict;

@end

NS_ASSUME_NONNULL_END
