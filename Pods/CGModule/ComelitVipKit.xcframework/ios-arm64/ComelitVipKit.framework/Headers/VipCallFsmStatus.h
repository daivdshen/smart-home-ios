//
//  VipCallFsmStatus.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 15/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, VipCallFsmStatus) {
    VipCallFsmStatusIdle = 0,      ///<Stato transiente non segnalato (perchè immediatamente sostituito dal CallFsmStInAlerting o CallFsmStOutInitiated)
    VipCallFsmStatusInAlerting,
    VipCallFsmStatusConnected,
    VipCallFsmStatusOutInitiated,
    VipCallFsmStatusOutAlerting,
    VipCallFsmStatusClosed,
    VipCallFsmStatusInvalidValue
};
