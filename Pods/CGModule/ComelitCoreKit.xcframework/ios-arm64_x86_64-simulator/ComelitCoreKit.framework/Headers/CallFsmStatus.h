//
//  CallFsmStatus.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 26/03/18.
//  Copyright © 2018 ComelitGroup. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, CallFsmStatus) {
    CallFsmStatusIdle,
    CallFsmStatusConnected,
    CallFsmStatusClosed,
    CallFsmStatusWaitIn,
    CallFsmStatusWaitOut,
    CallFsmStatusInvalidValue
};
