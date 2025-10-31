//
//  BMKAOISearchResult.h
//  BaiduMapAPI_Search
//
//  Created by zhangbaojin on 2023/3/9.
//  Copyright © 2023 Baidu. All rights reserved.
//

#import <BaiduMapAPI_Base/BMKTypes.h>

NS_ASSUME_NONNULL_BEGIN

@class BMKAOIInfo;

/// aoi检索返回结果类
@interface BMKAOISearchResult : NSObject

/// aoi信息数组
@property (nonatomic, copy) NSArray<BMKAOIInfo *> *aoiInfoList;

@end

/// aoi类型
typedef enum {
    BMK_AOI_TYPE_UNKNOWN              = 0,  ///< 未知类型
    BMK_AOI_TYPE_AIRPORT              = 1,  ///< 机场
    BMK_AOI_TYPE_RAILWAT_STATION      = 2,  ///< 火车站
    BMK_AOI_TYPE_SHOPPINGMALL         = 3,  ///< 商场
    BMK_AOI_TYPE_GAS_STATION          = 4,  ///< 加油站
    BMK_AOI_TYPE_SCHOOL               = 5,  ///< 学校
    BMK_AOI_TYPE_HOSPITAL             = 6,  ///< 医院
    BMK_AOI_TYPE_RESIDENTIAL_DISTRICT = 7,  ///< 住宅区
    BMK_AOI_TYPE_SCENIC_AREA          = 8,  ///< 风景区
    BMK_AOI_TYPE_PARK                 = 9,  ///< 公园
    BMK_AOI_TYPE_FREEWAY_SERVICE      = 10, ///< 服务区
    BMK_AOI_TYPE_WATER                = 11  ///< 水域
}BMKAOIType;


/// aoi信息类
@interface BMKAOIInfo : NSObject

/// 对应输入点顺序，用来区分是哪个点返回的aoi。
/// 注意：该字段从1开始计数
@property (nonatomic, assign, readonly) int order;

/// aoid的uid
@property (nonatomic, copy, readonly) NSString *uid;

/// aoi的名称
@property (nonatomic, copy, readonly) NSString *name;

/// aoi的类型
@property (nonatomic, assign, readonly) BMKAOIType type;

/// aoi的加密面数据
@property (nonatomic, copy, readonly) NSString *paths;

/// 点距离aoi边界最近距离，单位：米
/// 注：点在aoi面内则返回0
@property (nonatomic, assign, readonly) int nearestDistance;

/// 对应输入点与aoi的关系
/// 0（aoi内），1（aoi外），2（未找到）
@property (nonatomic, assign, readonly) int relation;

@end

NS_ASSUME_NONNULL_END
