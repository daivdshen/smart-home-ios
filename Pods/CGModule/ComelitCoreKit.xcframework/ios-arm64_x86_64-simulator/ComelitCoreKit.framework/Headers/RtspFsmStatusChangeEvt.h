//
//  RtspFsmStatusChangeEvt.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 26/03/18.
//  Copyright © 2018 ComelitGroup. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CallFsmStatusChangeEvt.h"
#import "RtspCallFsmStatus.h"

NS_ASSUME_NONNULL_BEGIN

@interface RtspFsmStatusChangeEvt : CallFsmStatusChangeEvt

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId fsmId:(NSInteger)fsmId status:(RtspCallFsmStatus)status;

@end

NS_ASSUME_NONNULL_END
