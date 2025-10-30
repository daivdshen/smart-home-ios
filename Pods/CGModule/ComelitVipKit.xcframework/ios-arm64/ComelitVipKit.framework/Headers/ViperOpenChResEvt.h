//
//  ViperOpenChResEvt.h
//  ComelitVipKit
//
//  Created by Mutti Simone on 24/09/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ChannelType.h"
#import "ViperChannelError.h"
#import <ComelitCoreKit/UnitEvt.h>

NS_ASSUME_NONNULL_BEGIN

@interface ViperOpenChResEvt : UnitEvt

@property (nonatomic, readonly) ViperChannelError response;
@property (nonatomic, readonly) ChannelType channel;

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId response:(ViperChannelError)response channel:(ChannelType)channel;

@end

NS_ASSUME_NONNULL_END
