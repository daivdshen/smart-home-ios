//
//  VipCallErrorEvt.h
//  ComelitVipKit
//
//  Created by Mutti Simone on 30/09/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <ComelitCoreKit/CallFsmEvt.h>
#import "CallErrorType.h"

NS_ASSUME_NONNULL_BEGIN

@interface VipCallErrorEvt : CallFsmEvt

@property (nonatomic, readonly) CallErrorType type;

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId fsmId:(NSInteger)fsmId event:(CallFsmEvent)event NS_UNAVAILABLE;

- (instancetype)initWithSysId:(NSInteger)sysId
                       unitId:(NSInteger)unitId
                        fsmId:(NSInteger)fsmId
                        event:(CallFsmEvent)event
                         type:(CallErrorType)type;

@end

NS_ASSUME_NONNULL_END
