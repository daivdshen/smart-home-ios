//
//  VipDivertedEvt.h
//  ComelitVipKit
//
//  Created by Mutti Simone on 30/09/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <ComelitCoreKit/CallFsmEvt.h>
#import <ComelitCoreKit/OutDivertActionResult.h>
#import <ComelitCoreKit/CallDirection.h>

NS_ASSUME_NONNULL_BEGIN

@interface VipDivertedEvt : CallFsmEvt

@property (nonatomic, readonly) NSString *peerVipAddr;
@property (nonatomic, readonly) NSString *peerSubAddr;
@property (nonatomic, readonly) OutDivertActionResult outDivertActionResult;
@property (nonatomic, readonly) CallDirection direction;

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId fsmId:(NSInteger)fsmId event:(CallFsmEvent)event NS_UNAVAILABLE;

- (instancetype)initWithSysId:(NSInteger)sysId
                       unitId:(NSInteger)unitId
                        fsmId:(NSInteger)fsmId
                        event:(CallFsmEvent)event
                  peerVipAddr:(NSString *)peerVipAddr
                  peerSubAddr:(NSString *)peerSubAddr
        outDivertActionResult:(OutDivertActionResult)outDivertActionResult
                    direction:(CallDirection)direction;

@end

NS_ASSUME_NONNULL_END
