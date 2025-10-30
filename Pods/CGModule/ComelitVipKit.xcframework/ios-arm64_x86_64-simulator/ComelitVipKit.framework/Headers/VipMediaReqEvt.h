//
//  VipMediaReqEvt.h
//  ComelitVipKit
//
//  Created by Mutti Simone on 30/09/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <ComelitCoreKit/CallFsmEvt.h>
#import <ComelitCoreKit/MediaReqChannel.h>
#import <ComelitCoreKit/MediaReqType.h>

NS_ASSUME_NONNULL_BEGIN

@interface VipMediaReqEvt : CallFsmEvt

@property (nonatomic, readonly) MediaReqChannel channel;
@property (nonatomic, readonly) MediaReqType action;

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId fsmId:(NSInteger)fsmId event:(CallFsmEvent)event NS_UNAVAILABLE;

- (instancetype)initWithSysId:(NSInteger)sysId
                       unitId:(NSInteger)unitId
                        fsmId:(NSInteger)fsmId
                        event:(CallFsmEvent)event
                      channel:(MediaReqChannel)channel
                       action:(MediaReqType)action;

@end

NS_ASSUME_NONNULL_END
