//
//  OctSecureAccessData.h
//  OctCore
//
//  Created by ZY on 2019/12/11.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class OctAdResponseModel;
@class OctSpaceInfoModel;
@class OctResponseModel;
@class OctContentInfoModel;
@class OctAdContentSlotModel;

@interface OctSecureAccessData : NSObject

+ (OctSpaceInfoModel *)spaceInfo:(OctAdResponseModel *)response;

+ (OctResponseModel *)adResponse:(OctSpaceInfoModel *)spaceInfo;

+ (OctContentInfoModel *)adContentInfo:(OctSpaceInfoModel *)spaceInfo;

+ (OctAdContentSlotModel *)adContentSlot:(OctSpaceInfoModel *)spaceInfo;

@end

NS_ASSUME_NONNULL_END
