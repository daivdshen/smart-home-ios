//
//  OctCoreSDKAdRequestModel+Builder.h
//  OctCoreSDK
//
//  Created by guantou on 2020/2/18.
//

#import "OctAdRequestModel.h"

NS_ASSUME_NONNULL_BEGIN

@class OctAdRequestModel;
@class OctAdRecordModel;

@interface OctAdRequestModel (Builder)

+ (NSData *)octRequestWithRecordModel:(OctAdRecordModel *)model;

@end

NS_ASSUME_NONNULL_END
