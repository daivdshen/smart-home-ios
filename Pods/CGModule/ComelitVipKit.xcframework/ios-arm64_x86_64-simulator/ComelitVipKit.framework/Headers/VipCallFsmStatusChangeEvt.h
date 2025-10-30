//
//  VipCallFsmStatusChangeEvt.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 30/09/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import "VipCallFsmStatus.h"

#import <ComelitCoreKit/CallType.h>
#import <ComelitCoreKit/Role.h>
#import <ComelitCoreKit/CallFsmStatusChangeEvt.h>

NS_ASSUME_NONNULL_BEGIN

@interface VipCallFsmStatusChangeEvt : CallFsmStatusChangeEvt

@property (nonatomic, readonly) CallType callType;
@property (nonatomic, readonly) Role peerRole;

- (instancetype)initWithSysId:(NSInteger)sysId
                       unitId:(NSInteger)unitId
                        fsmId:(NSInteger)fsmId
                   callStatus:(VipCallFsmStatus)callStatus
                callDirection:(CallDirection)callDirection
                     peerRole:(Role)peerRole
           mainLogicalAddress:(NSString *)mainLogicalAddress
            subLogicalAddress:(NSString *)subLogicalAddress
                        flags:(NSArray *)flags
                     unitType:(UnitType)unitType;

@end

NS_ASSUME_NONNULL_END
