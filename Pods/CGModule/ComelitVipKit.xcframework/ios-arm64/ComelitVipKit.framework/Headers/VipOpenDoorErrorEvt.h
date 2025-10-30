//
//  VipOpenDoorErrorEvt.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 30/09/17.
//  Copyright © 2017 ComelitGroup. All rights reserved.
//

#import "VipOOCEvtAbs.h"
#import "ConnError.h"


@interface VipOpenDoorErrorEvt : VipOOCEvtAbs

@property (nonatomic, readonly) ConnError error;

- (instancetype)init:(NSInteger)sysId unitId:(NSInteger)unitId vipAddr:(NSString *)vipAddr subAddr:(NSString *)subAddr error:(ConnError)error;

@end
