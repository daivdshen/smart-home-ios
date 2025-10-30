//
//  VipActuatorConnErrorEvt.h
//  Module
//
//  Created by Marco Gusmini on 30/09/2017.
//  Copyright © 2017 ComelitGroup. All rights reserved.
//

#import "VipOOCEvtAbs.h"
#import "ConnError.h"


@interface VipActuatorConnErrorEvt : VipOOCEvtAbs

@property (nonatomic, readonly) ConnError error;

- (instancetype)init:(NSInteger)sysId
              unitId:(NSInteger)unitId
             vipAddr:(NSString *)vipAddr
             subAddr:(NSString *)subAddr
               error:(ConnError)error;

@end
