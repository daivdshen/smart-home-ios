#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "CMGVipEvents.h"
#import "ComelitVipKit.h"
#import "VipComelitEngineWrapper.h"
#import "VipEngine.h"
#import "CGEvent.h"
#import "CMGCallTimes.h"
#import "CMGConstants.h"
#import "EventBusVip.h"
#import "VipActuatorReleaseEvt.h"
#import "VipCallErrorEvt.h"
#import "VipCallReleaseEvt.h"
#import "VipCapabReportEvt.h"
#import "VipDivertedEvt.h"
#import "VipMediaReqEvt.h"
#import "VipCfpEvt.h"
#import "VipCallFsmStatusChangeEvt.h"
#import "VipOpenDoorReleaseEvt.h"
#import "VipOpenDoorStatusEvt.h"
#import "VipSubunitStatusChangeEvt.h"
#import "VipActuatorConnErrorEvt.h"
#import "VipOOCEvtAbs.h"
#import "VipOpenDoorErrorEvt.h"
#import "ViperConnEvt.h"
#import "ViperOpenChResEvt.h"
#import "ViperRecvMsgEvt.h"
#import "VipCallEventParser.h"
#import "VipCallFsmStatusChangeParser.h"
#import "VipMessageParser.h"
#import "VipOpendoorEventParser.h"
#import "VipSetOutputEventParser.h"
#import "VipSubUnitFsmStatusChangeParser.h"
#import "ViperTunnelMessageParser.h"
#import "VipMessageParserFactory.h"
#import "CallErrorType.h"
#import "ConnError.h"
#import "SystemMsgType.h"
#import "ActuatorAction.h"
#import "SubUnitFsmStates.h"
#import "VipCallFsmStatus.h"
#import "VipMsgType.h"
#import "ChannelType.h"
#import "ViperChannelError.h"

FOUNDATION_EXPORT double ComelitVipKitVersionNumber;
FOUNDATION_EXPORT const unsigned char ComelitVipKitVersionString[];

