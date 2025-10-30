//
//  UnitFlag.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 22/12/21.
//

#import <Foundation/Foundation.h>

typedef NS_OPTIONS(NSInteger, UnitFlag) {
    UnitFlagEnableFsmRefCount          = 1 << 0,
    UnitFlagVipUnitMasterNotDoSubproxy = 1 << 1,
    UnitFlagEnableRtpOverRtsp          = 1 << 2,
    UnitFlagEnableRtspOverViper        = 1 << 3,
    UnitFlagNone                       = 0
};
