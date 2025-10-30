//
//  SubUnitFsmStates.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 12/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, SubUnitFsmStates) {
    SubUnitFsmStatesRegistering = 0,
    SubUnitFsmStatesFailwait,
    SubUnitFsmStatesRegistered,
    SubUnitFsmStatesInvalidValue
};
