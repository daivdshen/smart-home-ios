//
//  CallMediaController.h
//  CMGMultimedia
//
//  Created by Cornelli Fabio on 11/06/2020.
//  Copyright © 2020 Comelit. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MediaController.h"
#import "CallMediaController.h"
#import <ComelitCoreKit/UnitType.h>
#import <ComelitCoreKit/ComelitEngineWrapper.h>

NS_ASSUME_NONNULL_BEGIN

@interface CallMediaController : NSObject<MediaController>

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId fsmId:(NSInteger)fsmId unitType:(UnitType)unitType comelitEngineWrapper:(ComelitEngineWrapper*)comelitEngineWrapper;

@end

NS_ASSUME_NONNULL_END
