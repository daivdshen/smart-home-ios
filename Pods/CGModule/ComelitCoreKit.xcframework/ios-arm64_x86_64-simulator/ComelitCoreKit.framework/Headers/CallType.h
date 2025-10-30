//
//  CallType.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 15/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, CallType) {
    CallTypeCfp = 0,        ///< CFP (out-of-door call)
    CallTypeNormal,         ///< Normal door entry call
    CallTypeIntercom,       ///< Intercom call
    CallTypeTvcc,           ///< Video surveillance call
    CallTypeTest,           ///< Connectivity/bandwidth test call
    CallTypeVip,            ///< ViP calls
    CallTypeInvalidValue
};
