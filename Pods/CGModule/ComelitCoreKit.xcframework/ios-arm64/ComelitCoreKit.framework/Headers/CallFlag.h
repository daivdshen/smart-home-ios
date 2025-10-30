//
//  CallFlag.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 15/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, CallFlag) {
    CallFlagPriority1 = 0,
    CallFlagPriority2,
    CallFlagPriority3,
    CallFlagPriority4,
    CallFlagPriorityAll,
    CallFlagCaptured,
    CallFlagDiverted,
    CallFlagGroup,
    CallFlagNone,
    CallFlagInvalidValue
};
