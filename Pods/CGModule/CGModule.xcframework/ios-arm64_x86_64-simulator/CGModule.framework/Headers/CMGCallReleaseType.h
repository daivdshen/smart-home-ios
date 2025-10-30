//
//  CMGCallReleaseType.h
//  CGModule
//
//  Created by Cornelli Fabio on 11/03/2019.
//  Copyright © 2019 Cornelli Fabio. All rights reserved.
//

#ifndef CMGCallReleaseType_h
#define CMGCallReleaseType_h

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, CMGCallReleaseType)
{
    CMG_CALL_RELEASE_NORMAL,
    CMG_CALL_RELEASE_BY_USER,
    CMG_CALL_RELEASE_TIMEOUT,
    CMG_CALL_RELEASE_REJECTED,
    CMG_CALL_RELEASE_DIVERTED,
    CMG_CALL_RELEASE_DEVICE_NOT_FOUND,
    CMG_CALL_RELEASE_SYSTEM_BUSY,
    CMG_CALL_RELEASE_NOT_SELECTED,
    CMG_CALL_RELEASE_ERROR
};

#endif /* CMGCallReleaseType_h */
