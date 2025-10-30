//
//  CMGVipEvents.h
//  ComelitVipKit
//
//  Created by Busi Andrea on 02/08/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#pragma mark - Channel

static NSString * const EVENT_VIPER_CONNECTED = @"ViperConnectionEvt";
static NSString * const EVENT_VIPER_OPEN_CHANNEL            = @"ViperOpenChResEvt";
static NSString * const EVENT_VIPER_RECEIVE_MESSAGE         = @"ViperRecvMsgEvt";

#pragma mark - Opendoor

static NSString * const EVENT_OPENDOOR_ERROR                = @"VipOpenDoorErrorEvt";
static NSString * const EVENT_OPENDOOR_RELEASE              = @"VipOpenDoorReleaseEvt";
static NSString * const EVENT_OPENDOOR_STATUS               = @"VipOpenDoorStatusEvt";

#pragma mark - Actuator events

static NSString * const EVENT_ACTUATOR_RELEASE              = @"VipActuatorReleaseEvt";
static NSString * const EVENT_ACTUATOR_CONNECTION_ERROR     = @"VipActuatorConnErrorEvt";

#pragma mark - Call events

static NSString * const EVENT_CAPABILITY_REPORT             = @"VipCapabReportEvt";
static NSString * const EVENT_DIVERTED                      = @"VipDivertedEvt";
static NSString * const EVENT_RELEASE                       = @"VipCallReleaseEvt";
static NSString * const EVENT_CALL_ERROR                    = @"VipCallErrorEvt";
static NSString * const EVENT_MEDIA_REQUEST                 = @"VipMediaReqEvt";

#pragma mark - Fsm events

static NSString * const EVENT_CALL_FSM_STATUS_CHANGE        = @"VipCallFsmStatusChangeEvt";

#pragma mark - Subunit events

static NSString * const EVENT_SUBUNIT_STATUS_CHANGE         = @"VipSubunitStatusChangeEvt";
