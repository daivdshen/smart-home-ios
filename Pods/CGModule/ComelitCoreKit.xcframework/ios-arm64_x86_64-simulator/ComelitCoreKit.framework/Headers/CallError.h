//
//  CallError.h
//  ComelitCoreKit
//
//  Created by Busi Andrea on 31/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, CallError) {
    CallErrorConnectionError,
    CallErrorTimeoutExpired
};
