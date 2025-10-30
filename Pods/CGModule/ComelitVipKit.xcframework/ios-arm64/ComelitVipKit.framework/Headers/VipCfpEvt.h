//
//  VipCfpEvt.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 30/09/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import "VipOOCEvtAbs.h"

NS_ASSUME_NONNULL_BEGIN

@interface VipCfpEvt : VipOOCEvtAbs

- (instancetype)init:(NSInteger)sysId unitId:(NSInteger)unitId vipAddr:(NSString *)vipAddr subAddr:(NSString *)subAddr;

@end

NS_ASSUME_NONNULL_END
