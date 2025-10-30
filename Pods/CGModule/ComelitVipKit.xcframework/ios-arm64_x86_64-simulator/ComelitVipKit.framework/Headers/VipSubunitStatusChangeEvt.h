//
//  VipSubunitStatusChangeEvt.h
//  ComelitVipKit
//
//  Created by Mutti Simone on 30/09/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import "SubUnitFsmStates.h"
#import <ComelitCoreKit/UnitEvt.h>

NS_ASSUME_NONNULL_BEGIN

@interface VipSubunitStatusChangeEvt : UnitEvt

@property (nonatomic, readonly) SubUnitFsmStates status;
@property (nonatomic, readonly) NSString *vipAddr;
@property (nonatomic, readonly) NSString *subAddr;

- (instancetype)init:(NSInteger)sysId
              unitId:(NSInteger)unitId
          callStatus:(SubUnitFsmStates)status
             vipAddr:(NSString *)vipAddr
             subAddr:(NSString *)subAddr;

@end

NS_ASSUME_NONNULL_END
