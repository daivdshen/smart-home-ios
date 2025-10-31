//
//  BMKAOISearch.h
//  BaiduMapAPI_Search
//
//  Created by zhangbaojin on 2023/3/9.
//  Copyright © 2023 Baidu. All rights reserved.
//

#import "BMKSearchBase.h"
#import "BMKAOISearchOption.h"
#import "BMKAOISearchResult.h"

NS_ASSUME_NONNULL_BEGIN

@protocol BMKAOISearchDelegate;
/// aoi检索，aoi检索服务为高级服务，AK需要申请高级权限 since 6.6.0
@interface BMKAOISearch : BMKSearchBase

///  aoi检索模块的Delegate
@property (nonatomic, weak) id<BMKAOISearchDelegate> delegate;

/// aoi检索
- (BOOL)aoiSearch:(BMKAOISearchOption *)aoiOption;

@end


/// 搜索delegate，用于获取搜索结果
@protocol BMKAOISearchDelegate <NSObject>
@optional

/// 返回aoi检索结果
/// @param searcher 检索对象
/// @param result 检索结果
/// @param errorCode 错误号，@see BMKSearchErrorCode
- (void)onGetAOIResult:(BMKAOISearch *)searcher result:(BMKAOISearchResult *)result errorCode:(BMKSearchErrorCode)errorCode;

@end

NS_ASSUME_NONNULL_END
