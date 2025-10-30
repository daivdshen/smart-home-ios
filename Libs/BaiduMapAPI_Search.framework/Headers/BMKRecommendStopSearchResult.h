//
//  BMKRecommendStopSearchResult.h
//  BaiduMapAPI_Search
//
//  Created by Baidu on 2020/7/27.
//  Copyright © 2020 Baidu. All rights reserved.
//

#import <BaiduMapAPI_Base/BMKTypes.h>

@class BMKRecommendStopInfo;
@class BMKStationRecommendStopInfo;
NS_ASSUME_NONNULL_BEGIN

@interface BMKRecommendStopSearchResult : NSObject

/// 推荐上车点返回结果列表，当isNeedStationInfo为NO时返回
@property (nonatomic, copy) NSArray<BMKRecommendStopInfo *> *recommendStopInfoList;

/// 场站推荐上车点返回结果列表，当isNeedStationInfo为YES时返回
@property (nonatomic, copy) NSArray<BMKStationRecommendStopInfo *> *stationInfoList;

@end

@interface BMKStationRecommendStopInfo : NSObject
/// 场站分层名称信息，逗号隔开，如:(北京首都国际机场,T1航站楼,停车场)，无场站分层信息时为空字符串
@property (nonatomic, copy) NSString *stationName;
/// 推荐上车点返回结果列表，无场站分层信息时为普通推荐上车点
@property (nonatomic, copy) NSArray<BMKRecommendStopInfo *> *recommendStopsInfoList;

@end


@interface BMKRecommendStopInfo : NSObject

/// 推荐上车点名称
@property (nonatomic, copy) NSString *name;

/// 推荐上车点地址
@property (nonatomic, copy) NSString *address;

/// 推荐上车点经纬度
@property (nonatomic, assign) CLLocationCoordinate2D location;

/// 推荐点poi的uid
@property (nonatomic, copy) NSString *uid;

/// 距查找点的距离，单位米
@property (nonatomic, assign) CGFloat distance;

@end

NS_ASSUME_NONNULL_END
