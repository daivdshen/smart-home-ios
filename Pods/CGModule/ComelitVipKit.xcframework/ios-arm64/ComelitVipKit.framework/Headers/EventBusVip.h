//
//  EventBusVip.h
//  Module
//
//  Created by Mutti Simone on 30/09/17.
//  Copyright © 2017 ComelitGroup. All rights reserved.
//
//  Definition of constants used by other classes.
//

#import <Foundation/Foundation.h>

@interface EventBusVip : NSObject


////VIPERTUNNEL EVENTS
#pragma mark - High level (tunnel) events
extern NSString *const EVENT_OPEN_CHANNEL;


////other events
#pragma mark - Command error
extern NSString *const EVENT_RESPONSE_ERROR;

#pragma mark - Command responses
extern NSString *const EVENT_ACTIVATE_USER_RESPONSE;
extern NSString *const EVENT_ADD_USER_RESPONSE;
extern NSString *const EVENT_BASIC_RESPONSE;
extern NSString *const EVENT_MODIFY_USER_RESPONSE;
extern NSString *const EVENT_REMOVE_USER_RESPONSE;
extern NSString *const EVENT_SERVER_INFO_RESPONSE;
extern NSString *const EVENT_USER_CFG_RESPONSE;

extern NSString *const EVENT_CHAIN_ERROR;
extern NSString *const EVENT_CHAIN_DONE;

extern NSString *const EVENT_CONNECTION_STATUS_CHANGE;

extern NSString *const EVENT_CONFIGURATION_CHANGE;
extern NSString *const EVENT_CONFIGURATION_SAVE_DONE;
extern NSString *const EVENT_CONFIGURATION_SAVE_ERROR;

extern NSString *const EVENT_CONNECT_REQUEST_COMPLETED;
@end
