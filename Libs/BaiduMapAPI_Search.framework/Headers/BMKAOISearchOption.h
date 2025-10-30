//
//  BMKAOISearchOption.h
//  BaiduMapAPI_Search
//
//  Created by zhangbaojin on 2023/3/9.
//  Copyright © 2023 Baidu. All rights reserved.
//

//#import <CoreLocation/CoreLocation.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// aoi请求信息类
@interface BMKAOISearchOption : NSObject

/// 经纬度数组字符串 (必选)
/// 注意以下两点：
/// 多个经纬度之间以 ; 号分割
/// 经纬度类型必须为 BMK_COORDTYPE_BD09LL
/// eg: 116.31380,40.07445;116.31087,40.07361
@property (nonatomic, copy) NSString *locations;

@end


NS_ASSUME_NONNULL_END
