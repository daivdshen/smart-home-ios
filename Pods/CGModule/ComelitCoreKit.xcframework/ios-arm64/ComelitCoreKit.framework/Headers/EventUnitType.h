//
//  EventUnitType.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 15/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, EventUnitType) {
    EventUnitTypeNone = -1,
    EventUnitTypeVip = 0,
    EventUnitTypeSafe,
    EventUnitTypeHomeAutomation,
    EventUnitTypeTvcc,
    EventUnitTypeRtsp,
    EventUnitTypeUnknown
};
