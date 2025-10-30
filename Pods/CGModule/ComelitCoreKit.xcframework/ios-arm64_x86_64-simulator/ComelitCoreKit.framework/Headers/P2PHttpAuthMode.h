//
//  P2PHttpAuthMode.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 21/01/21.
//  Copyright © 2021 Comelit Group Spa. All rights reserved.
//

#ifndef P2PHttpAuthMode_h
#define P2PHttpAuthMode_h

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, P2PHttpAuthMode) {
    P2PHttpAuthModeViperToken = 0,
    P2PHttpAuthModeCloudActToken,
    P2PHttpAuthModeSafeToken
};

#endif /* P2PHttpAuthMode_h */
