//
//  VipCapabReportEvt.h
//  ComelitVipKit
//
//  Created by Mutti Simone on 30/09/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <ComelitCoreKit/CallFsmEvt.h>
#import <ComelitCorekit/Role.h>

NS_ASSUME_NONNULL_BEGIN

@interface VipCapabReportEvt : CallFsmEvt

/**
 Array of VipUnitCapability elements
 */
@property (nonatomic, readonly) NSArray *capabilities;

/**
 Role of the call peer
 */
@property (nonatomic, readonly) Role peerRole;

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId fsmId:(NSInteger)fsmId event:(CallFsmEvent)event NS_UNAVAILABLE;

- (instancetype)initWithSysId:(NSInteger)sysId
                       unitId:(NSInteger)unitId
                        fsmId:(NSInteger)fsmId
                        event:(CallFsmEvent)event
                 capabilities:(NSArray *)capabilities
                     peerRole:(Role)peerRole;

@end

NS_ASSUME_NONNULL_END
