//
//  VipOOCEvtAbs.h
//  ComelitVipKit
//
//  Created by Mutti Simone on 28/09/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <ComelitCoreKit/UnitEvt.h>

NS_ASSUME_NONNULL_BEGIN

@interface VipOOCEvtAbs : UnitEvt

@property (nonatomic, readonly) NSString *vipAddr;
@property (nonatomic, readonly) NSString *subAddr;

- (instancetype)init:(NSInteger)sysId unitId:(NSInteger)unitId vipAddr:(NSString *)vipAddr subAddr:(NSString *)subAddr;

@end

NS_ASSUME_NONNULL_END
