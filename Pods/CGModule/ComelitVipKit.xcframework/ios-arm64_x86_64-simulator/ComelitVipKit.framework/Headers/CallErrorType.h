//
//  CallErrorType.h
//  ComelitVipKit
//
//  Created by Busi Andrea on 02/08/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, CallErrorType) {
    CallErrorTypeConnectionError = 0,
    CallErrorTypeTimeoutExpired
};
