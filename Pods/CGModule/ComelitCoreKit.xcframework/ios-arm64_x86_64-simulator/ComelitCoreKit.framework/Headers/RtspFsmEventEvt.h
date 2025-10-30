//
//  RtspFsmEventEvt.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 26/03/18.
//  Copyright © 2018 ComelitGroup. All rights reserved.
//

#import "FsmEvt.h"
#import "RtspReply.h"

NS_ASSUME_NONNULL_BEGIN

@interface RtspFsmEventEvt : FsmEvt

@property (nonatomic) RtspReply rtspEvent;

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId fsmId:(NSInteger)fsmId event:(RtspReply)event;

@end

NS_ASSUME_NONNULL_END
