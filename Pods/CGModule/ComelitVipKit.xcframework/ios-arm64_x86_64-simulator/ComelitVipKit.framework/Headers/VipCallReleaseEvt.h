//
//  VipCallReleaseEvt.h
//  ComelitVipKit
//
//  Created by Mutti Simone on 30/09/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <ComelitCoreKit/CallFsmEvt.h>
#import <ComelitCoreKit/ReleaseCause.h>

NS_ASSUME_NONNULL_BEGIN

@interface VipCallReleaseEvt : CallFsmEvt

@property (nonatomic, readonly) ReleaseCause cause;

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId fsmId:(NSInteger)fsmId event:(CallFsmEvent)event NS_UNAVAILABLE;

- (instancetype)initWithSysId:(NSInteger)sysId
                       unitId:(NSInteger)unitId
                        fsmId:(NSInteger)fsmId
                        event:(CallFsmEvent)event
                        cause:(ReleaseCause)cause;

@end

NS_ASSUME_NONNULL_END
