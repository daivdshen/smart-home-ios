//
//  VipActuatorReleaseEvt.h
//  Module
//
//  Created by Marco Gusmini on 30/09/2017.
//  Copyright © 2017 ComelitGroup. All rights reserved.
//

#import "VipOOCEvtAbs.h"
#import <ComelitCoreKit/ReleaseCause.h>


@interface VipActuatorReleaseEvt : VipOOCEvtAbs

@property (nonatomic,readonly) ReleaseCause releaseCause;

- (instancetype)init:(NSInteger)sysId unitId:(NSInteger)unitId vipAddr:(NSString *)vipAddr subAddr:(NSString *)subAddr releaseCause:(ReleaseCause)releaseCause;

@end
