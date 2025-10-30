//
//  ComelitCoreKit.h
//  ComelitCoreKit
//
//  Created by Busi Andrea on 08/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <UIKit/UIKit.h>

//! Project version number for ComelitCoreKit.
FOUNDATION_EXPORT double ComelitCoreKitVersionNumber;

//! Project version string for ComelitCoreKit.
FOUNDATION_EXPORT const unsigned char ComelitCoreKitVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <ComelitCoreKit/PublicHeader.h>
// engine
#import <ComelitCoreKit/ComelitEngine.h>
#import <ComelitCoreKit/ComelitEngineWrapper.h>
#import <ComelitCoreKit/JsonSocketHandler.h>
#import <ComelitCorekit/UnitMessageParserProtocol.h>
#import <ComelitCoreKit/RtspMessageParser.h>
// types and models
#import <ComelitCoreKit/CallDirection.h>
#import <ComelitCoreKit/OutOfCallType.h>
#import <ComelitCoreKit/ReleaseCause.h>
#import <ComelitCoreKit/OpenDoorStatus.h>
#import <ComelitCoreKit/CallType.h>
#import <ComelitCoreKit/Role.h>
#import <ComelitCoreKit/CallFlag.h>
#import <ComelitCoreKit/UnitCapability.h>
#import <ComelitCoreKit/CallFsmEvent.h>
#import <ComelitCoreKit/OutDivertActionResult.h>
#import <ComelitCoreKit/DivertDirection.h>
#import <ComelitCoreKit/CallError.h>
#import <ComelitCoreKit/MediaReqChannel.h>
#import <ComelitCoreKit/MediaReqType.h>
#import <ComelitCoreKit/EventUnitType.h>
#import <ComelitCoreKit/CMGMediaType.h>
#import <ComelitCoreKit/MediaDirection.h>
#import <ComelitCoreKit/CallTimeParam.h>
#import <ComelitCoreKit/P2pFilterStrategy.h>
#import <ComelitCoreKit/CMGP2PParameters.h>
// events
#import <ComelitCoreKit/UnitEvt.h>
#import <ComelitCoreKit/FsmEvt.h>
#import <ComelitCoreKit/CallFsmEvt.h>
#import <ComelitCoreKit/CallFsmStatusChangeEvt.h>
#import <ComelitCoreKit/RtspFsmEventEvt.h>
#import <ComelitCoreKit/RtspFsmStatusChangeEvt.h>
// utils and constants
#import <ComelitCoreKit/CMGNetUtils.h>
#import <ComelitCoreKit/CMGParsingUtils.h>
#import <ComelitCoreKit/CMGCommonEvents.h>
// comelit event bus (to be able to use it in .swift file within the framework)
#import <ComelitCoreKit/ToloEventBus.h>
