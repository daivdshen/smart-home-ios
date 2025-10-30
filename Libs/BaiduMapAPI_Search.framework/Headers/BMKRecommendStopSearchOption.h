//
//  BMKRecommendStopSearchOption.h
//  BaiduMapAPI_Search
//
//  Created by Baidu on 2020/7/27.
//  Copyright © 2020 Baidu. All rights reserved.
//

#import <CoreLocation/CoreLocation.h>

NS_ASSUME_NONNULL_BEGIN

@interface BMKRecommendStopSearchOption : NSObject

/// 推荐上车点经纬度 （必选）
@property (nonatomic, assign) CLLocationCoordinate2D location;
/// 是否需要场站上车点推荐（可选），默认：NO，设置为YES时，location位置在场站附近时返回对应的场站推荐上车点信息
@property (nonatomic, assign) BOOL isNeedStationInfo;

@end

NS_ASSUME_NONNULL_END
