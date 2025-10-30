//
//  CallFsmEvt.h
//  ComelitCoreKit
//
//  Created by Busi Andrea on 07/08/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import "FsmEvt.h"
#import "CallFsmEvent.h"

NS_ASSUME_NONNULL_BEGIN

@interface CallFsmEvt : FsmEvt

@property (nonatomic) CallFsmEvent event;

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId fsmId:(NSInteger)fsmId NS_UNAVAILABLE;

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId fsmId:(NSInteger)fsmId event:(CallFsmEvent)event;

@end

NS_ASSUME_NONNULL_END
