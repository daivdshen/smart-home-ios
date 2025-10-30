//
//  SoftwareVideoDecoder.h
//  CMGMultimedia
//
//  Created by Cornelli Fabio on 11/06/2020.
//  Copyright © 2020 Comelit. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ComelitCoreKit/UnitType.h>
#import <ComelitCoreKit/ComelitEngineWrapper.h>
#import "VideoDecoder.h"

NS_ASSUME_NONNULL_BEGIN

@interface SoftwareVideoDecoder : NSObject<VideoDecoder>

- (instancetype)initWithWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId fsmId:(NSInteger)fsmId unitType:(UnitType)unitType comelitEngineWrapper:(ComelitEngineWrapper *)comelitEngineWrapper;

@end

NS_ASSUME_NONNULL_END
