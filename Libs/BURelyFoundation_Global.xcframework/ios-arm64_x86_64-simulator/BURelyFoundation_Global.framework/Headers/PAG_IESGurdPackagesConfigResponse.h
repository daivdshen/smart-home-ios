//
//  PAG_IESGurdPackagesConfigResponse.h
//  IESGeckoKit
//
//  Created by 陈煜钏 on 2020/3/1.
//

#import <Foundation/Foundation.h>

#import "PAG_IESGeckoResourceModel.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, PAG_IESGurdSpecifiedCleanType) {
    PAG_IESGurdSpecifiedCleanTypeUnknown = 0,
    PAG_IESGurdSpecifiedCleanTypeMatch,             //本地packageId与下发packageId完全匹配 则清理
    PAG_IESGurdSpecifiedCleanTypeLessThan,          //本地packageId小于下发packageId 则清理
    PAG_IESGurdSpecifiedCleanTypeNoMatter           //直接清理
};

@interface PAG_IESGurdSpecifiedClean : NSObject

@property (nonatomic, assign) PAG_IESGurdSpecifiedCleanType cleanType;

@property (nonatomic, copy) NSString *channel;

@property (nonatomic, copy) NSArray<NSNumber *> *versions;

- (BOOL)shouldCleanWithVersion:(int64_t)version;

@end

@interface PAG_IESGurdGroupClean : NSObject

@property (nonatomic, assign) NSInteger rule;   // 1- "channel_num_limit" 2-"file_size_limit"

@property (nonatomic, assign) NSInteger policy; // 1-FIFO 2-LFU 3-LRU

@property (nonatomic, assign) NSInteger limit;  // rule为1时代表channel个数；rule为2时代表文件大小

@end

@interface PAG_IESGurdUniversalStrategies : NSObject
// 指定删除channels
@property (nonatomic, copy) NSArray<PAG_IESGurdSpecifiedClean *> *specifiedCleanArray;

@property (nonatomic, strong) PAG_IESGurdGroupClean *groupClean;

+ (instancetype _Nullable)strategiesWithDictionary:(NSDictionary *)dictionary;

@end

@interface PAG_IESGurdPackagesConfigResponse : NSObject

// accessKey : @[ PAG_IESGurdResourceModel, ... ]
@property (nonatomic, copy) NSDictionary<NSString *, NSArray<PAG_IESGurdResourceModel *> *> *packagesDictionary;

// accessKey : PAG_IESGurdUniversalStrategies
@property (nonatomic, copy) NSDictionary<NSString *, PAG_IESGurdUniversalStrategies *> *universalStrategiesDictionary;

@end

NS_ASSUME_NONNULL_END
