//
//  ReleaseCause.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 12/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, ReleaseCause) {
    ReleaseCauseNormal = 0,
    ReleaseCauseUserBusy,
    ReleaseCauseNonSelected,
    ReleaseCauseRejected,
    ReleaseCauseDiverted,
    ReleaseCauseUserNotFound,
    ReleaseCauseDeviceNotExist,
    ReleaseCauseSystemBusy,
    ReleaseCauseCallUnsupported,
    ReleaseCauseMalformedMessage,
    ReleaseCauseInternalError,
    ReleaseCauseGenericFailure,
    ReleaseCauseUnknown,
    ReleaseCauseCtpDisconnected,
    ReleaseCauseUnexpectedMsg,
    ReleaseCauseInvalidValue
};
