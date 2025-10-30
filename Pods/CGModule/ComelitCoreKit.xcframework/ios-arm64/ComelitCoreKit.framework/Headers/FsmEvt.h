//
//  FsmEvt.h
//  ComelitCoreKit
//
//  Created by Busi Andrea on 07/08/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import "UnitEvt.h"

NS_ASSUME_NONNULL_BEGIN

@interface FsmEvt : UnitEvt

@property (nonatomic) NSInteger fsmId;

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId NS_UNAVAILABLE;

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId fsmId:(NSInteger)fsmId;

@end

NS_ASSUME_NONNULL_END
