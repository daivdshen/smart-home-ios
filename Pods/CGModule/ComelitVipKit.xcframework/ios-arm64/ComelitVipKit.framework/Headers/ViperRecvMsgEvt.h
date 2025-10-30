//
//  ViperRecvMsgEvt.h
//  ComelitVipKit
//
//  Created by Mutti Simone on 24/09/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ChannelType.h"
#import <ComelitCoreKit/UnitEvt.h>

NS_ASSUME_NONNULL_BEGIN

@interface ViperRecvMsgEvt : UnitEvt

@property (nonatomic, readonly) ChannelType channel;
@property (nonatomic, readonly) NSDictionary *msg;

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId channel:(ChannelType)channel msg:(NSDictionary *)msg;

@end

NS_ASSUME_NONNULL_END
