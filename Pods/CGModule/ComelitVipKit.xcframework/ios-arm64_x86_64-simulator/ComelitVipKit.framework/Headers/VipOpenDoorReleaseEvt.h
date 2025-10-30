//
//  VipOpenDoorReleaseEvt.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 30/09/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import "VipOOCEvtAbs.h"
#import <ComelitCoreKit/ReleaseCause.h>

NS_ASSUME_NONNULL_BEGIN

@interface VipOpenDoorReleaseEvt : VipOOCEvtAbs

@property (nonatomic,readonly) ReleaseCause releaseCause;

- (instancetype)init:(NSInteger)sysId
              unitId:(NSInteger)unitId
             vipAddr:(NSString *)vipAddr
             subAddr:(NSString *)subAddr
        releaseCause:(ReleaseCause)releaseCause;

@end

NS_ASSUME_NONNULL_END
