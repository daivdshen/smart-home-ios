//
//  VipOpenDoorStatusEvt.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 30/09/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import "VipOOCEvtAbs.h"
#import <ComelitCoreKit/OpenDoorStatus.h>

NS_ASSUME_NONNULL_BEGIN

@interface VipOpenDoorStatusEvt : VipOOCEvtAbs

@property (nonatomic, readonly) OpenDoorStatus status;

- (instancetype)init:(NSInteger)sysId
              unitId:(NSInteger)unitId
             vipAddr:(NSString *)vipAddr
             subAddr:(NSString *)subAddr
      openDoorStatus:(OpenDoorStatus)status;

@end

NS_ASSUME_NONNULL_END
