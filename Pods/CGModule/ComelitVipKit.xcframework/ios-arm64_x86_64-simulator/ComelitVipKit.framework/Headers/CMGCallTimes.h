//
//  CMGCallTimes.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 11/07/18.
//  Copyright © 2018 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface CMGCallTimes : NSObject

@property (nonatomic) int alert;               ///< Maximum time in the state calls for Alerting
@property (nonatomic) int outInitated;         ///< Maximum waiting time response to the connection request from the destination
@property (nonatomic) int connected;           ///< Maximum Connection Time
@property (nonatomic) int icConnected;         ///< Maximum Connection Time in Intercom Calls
@property (nonatomic) int slConnected;         ///< Maximum Connection Time in Self Igniction Calls
@property (nonatomic) int close;               ///< Waiting time before sending a release call
@property (nonatomic) int proceeding;          ///< Maximum Proceeding Time waiting for alerting

- (instancetype)initWithAlert:(int)alert
                  outInitated:(int)outInitated
                    connected:(int)connected
                  icConnected:(int)icConnected
                  slConnected:(int)slConnected
                        close:(int)close
                   proceeding:(int)proceeding;

@end
