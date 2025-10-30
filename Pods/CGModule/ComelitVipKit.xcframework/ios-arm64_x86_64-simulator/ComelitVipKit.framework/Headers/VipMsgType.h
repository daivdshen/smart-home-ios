//
//  VipMsgType.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 12/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

typedef NS_ENUM(NSInteger, VipMsgType) {
    VipMsgTypeCallFsmStatusChange = 0,
    VipMsgTypeCallEvent,
    VipMsgTypeSubunitFsmStatusChange,
    VipMsgTypeSubunitEvent,
    VipMsgTypeOpenDoorEvent,
    VipMsgTypeSetOutputEvent,
    VipMsgTypeCfpEvent,
    VipMsgTypeElevatorEvent
};
