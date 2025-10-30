//
//  CallFsmEvent.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 15/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, CallFsmEvent) {
    CallFsmEventCapReport = 0,
    CallFsmEventInDiverted,
    CallFsmEventOutDiverted,
    CallFsmEventReleased,
    CallFsmEventConnectionError,
    CallFsmEventHandleMediareq,
    CallFsmEventTimerExpired,
    CallFsmEventVoiceStatus,
    CallFsmEventInvalidValue
};
