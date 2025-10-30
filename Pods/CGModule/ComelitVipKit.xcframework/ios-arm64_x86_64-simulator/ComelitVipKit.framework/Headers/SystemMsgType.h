//
//  SystemMsgType.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 15/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, SystemMsgType) {
    SystemMsgTypeViperConnectionEvent = 0,  ///< viper_connection_event
    SystemMsgTypeViperChannelOpenRes,       ///< viper_channel_open_res
    SystemMsgTypeViperChannelMsg,           ///< viper_channel_msg
    SystemMsgTypeSystemStatusMsg,           ///< System status message
    SystemMsgTypeLibraryStatysMsg,          ///< Library status message
    SystemMsgTypeInvalidValue
};
