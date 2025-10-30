//
//  UnitCapability.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 15/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_OPTIONS(NSInteger, UnitCapability) {
    UnitCapabilityAudioDst  =  0x00000001,  ///< Can receive an audio flow
    UnitCapabilityAudioSrc  =  0x00000002,  ///< Can send an audio flow
    UnitCapabilityVideoDst  =  0x00000004,  ///< Can receive a video flow
    UnitCapabilityVideoSrc  =  0x00000008,  ///< Can send a video flow
    UnitCapabilityOpenDoor  =  0x00000010,  ///< Can open a door
    UnitCapabilityMStream   =  0x00000020,  ///< Can send multiple video/audio streams
    UnitCapabilityNone      =  0
};
