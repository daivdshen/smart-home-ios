//
//  OctDisposeResponseModel.h
//  OctCoreSDK
//
//  Created by guantou on 2019/12/9.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class OctAdResponseModel;
@class OctSpaceInfoModel;

@interface OctDisposeResponseModel : NSObject

- (instancetype)initWithData:(OctAdResponseModel *)serverResponse;

@property (nonatomic, strong, readonly) OctSpaceInfoModel *spaceInfo;

@property (nonatomic, assign, readonly) CGFloat widthThanHeigh;
@property (nonatomic, assign, readonly) CGFloat heighThanWidth;

@end

NS_ASSUME_NONNULL_END
