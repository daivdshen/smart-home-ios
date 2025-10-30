//
//  CallTimeParam.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 16/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, CallTimeParam) {
    CallTimeParamAlerted = 0,       ///< Maximum time in the state calls for Alerting
    CallTimeParamOutInitiated,      ///< Maximum waiting time response to the connection request from the destination
    CallTimeParamConnected,         ///< Maximum Connection Time
    CallTimeParamClose,             ///< Waiting time before sending a release call
    CallTimeParamIcConnected,       ///< Maximum Connection Time in Intercom Calls
    CallTimeParamSlConnected,       ///< Maximum Connection Time in Self Igniction Calls
    CallTimeParamProceeding,        ///< Maximum Proceeding Time waiting for alerting
    CallTimeParamMaxOnClosed        ///< Maximum Time waiting for call destroyed
};
