//
//  UnitEvt.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 26/03/18.
//  Copyright © 2019 ComelitGroup. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface UnitEvt : NSObject

@property (nonatomic) NSInteger sysId;
@property (nonatomic) NSInteger unitId;

- (instancetype)init NS_UNAVAILABLE;

- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId;

@end

NS_ASSUME_NONNULL_END
