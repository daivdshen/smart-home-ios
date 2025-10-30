//
//  CallFsmStatusChangeEvt.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 26/03/18.
//  Copyright © 2018 ComelitGroup. All rights reserved.
//

#import "FsmEvt.h"
#import "CallFsmStatus.h"
#import "CallDirection.h"
#import "UnitType.h"

NS_ASSUME_NONNULL_BEGIN

@interface CallFsmStatusChangeEvt : FsmEvt

@property (nonatomic, strong) NSString *mainLogicalAddress;
@property (nonatomic, strong) NSString *subLogicalAddress;
@property (nonatomic) CallDirection callDirection;
@property (nonatomic) CallFsmStatus status;
@property (nonatomic, strong, nullable) NSArray *callFlags;
@property (nonatomic) UnitType unitType;

- (instancetype)initWithSysId:(NSInteger)sysId
                       unitId:(NSInteger)unitId
                        fsmId:(NSInteger)fsmId
                    fsmStatus:(CallFsmStatus)status
                callDirection:(CallDirection)callDirection
           mainLogicalAddress:(NSString *)mainLogicalAddress
            subLogicalAddress:(NSString *)subLogicalAddress
                     unitType:(UnitType)unitType;

@end

NS_ASSUME_NONNULL_END
