//
//  ViperConnEvt.h
//  ComelitVipKit
//
//  Created by Mutti Simone on 04/10/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ComelitCoreKit/UnitEvt.h>

NS_ASSUME_NONNULL_BEGIN

@interface ViperConnEvt : UnitEvt

@property (nonatomic, readonly) NSInteger eventId;
@property (nonatomic, readonly) NSString *event;

- (instancetype)init:(NSInteger)sysId unitId:(NSInteger)unitId eventId:(NSInteger)eventId event:(NSString *)event;

@end

NS_ASSUME_NONNULL_END
