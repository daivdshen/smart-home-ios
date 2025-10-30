//
//  CGEvent.h
//  Module
//
//  Created by Mutti Simone on 16/10/17.
//  Copyright © 2017 ComelitGroup. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CGEvent : NSObject


extern NSString *const CG_EVENT_CONNECTION_STATUS_CHANGE;

extern NSString *const CG_EVENT_CAPABILITY_REPORT;
extern NSString *const CG_EVENT_DIVERTED;
extern NSString *const CG_EVENT_RELEASE;
extern NSString *const CG_EVENT_CALL_ERROR;
extern NSString *const CG_EVENT_MEDIA_REQUEST;

extern NSString *const CG_EVENT_CALL_FSM_STATUS_CHANGE;

extern NSString *const CG_EVENT_RTSP_FSM_STATUS_CHANGE;

extern NSString *const CG_EVENT_CONNECT_REQUEST_COMPLETED;

@end
