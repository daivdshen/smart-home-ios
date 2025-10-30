//
//  OutOfCallType.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 15/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, OutOfCallType) {
    OutOfCallTypeRelease = 0,
    OutOfCallTypeUnexpectedMsg,
    OutOfCallTypeCtpDisconnected,
    OutOfCallTypeReceivedStatus,
    OutOfCallTypeReceivedODRequest,
    OutOfCallTypeReceivedSORequest,
    OutOfCallTypeCfpCall,
    OutOfCallTypeInvalidValue
};

