//
//  CMGP2PParameters.h
//  ComelitCoreKit
//
//  Created by Busi Andrea on 31/03/2020.
//  Copyright © 2020 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "P2pFilterStrategy.h"
#import "P2PHttpAuthMode.h"
#import "P2PIceRole.h"
#import "P2PIceSdpMode.h"

NS_ASSUME_NONNULL_BEGIN

@interface CMGP2PParameters : NSObject

/// Role of this peer in the p2p negotiation ('a' for controlled role, 'o' for controlling role)
@property (nonatomic) P2PIceRole iceRole;

/// Mode of sdp exchange transport (can be MQTT or HTTP)
@property (nonatomic) P2PIceSdpMode iceSdpMode;

/// Client id to use on the MQTT broker
@property (strong, nonatomic) NSString *mqttName;

/// MQTT server used to exchange MQTT messages
@property (strong, nonatomic) NSString *mqttServer;

/// Topic to use to publish MQTT message (is the topic where the controlling agent listen)
@property (strong, nonatomic) NSString *mqttPublicTopic;

/// MQTT topic where the controllend agent listen
@property (strong, nonatomic) NSString *mqttSubTopic;

/// MQTT username used to authenticate on MQTT server
@property (strong, nonatomic) NSString *mqttUsername;

/// MQTT password used to authenticate on MQTT server
@property (strong, nonatomic) NSString *mqttPassword;

/// HTTP endpoint for sdp exchange
@property (strong, nonatomic) NSString *httpEndpoint;

/// HTTP ccs token
@property (strong, nonatomic) NSString *httpCcsToken;

/// HTTP device uuid
@property (strong, nonatomic) NSString *httpDuuid;

/// HTTP authentication mode (viper token or cloud activation code)
@property (nonatomic) P2PHttpAuthMode httpAuthMode;

/// HTTP authentication token
@property (strong, nonatomic) NSString *httpAuthToken;

/// STUN server address used to gathering candidates
@property (strong, nonatomic) NSString *stunServerAddress1;

/// Another STUN server address (used in case the first one is unreachable)
@property (strong, nonatomic) NSString *stunServerAddress2;

/// Selected strategy used to filter the remote candidates
@property (nonatomic) P2pFilterStrategy strategy;

/// Certificate needed in order to use the mqtts on the MQTT server (NULL <=> plain-text MQTT messages)
@property (strong, nonatomic) NSString *certificatePath;

/// Flag to force the ipv6 address resolution
@property (nonatomic) BOOL ipv6Only;

/// Delay the start of udp and tcp negotitation
@property (nonatomic) NSInteger remNegoDelay;


- (instancetype)initWithIceSdpMode:(P2PIceSdpMode)iceSdpMode
                          mqttName:(NSString *)mqttName
                        mqttServer:(NSString *)mqttServer
                   mqttPublicTopic:(NSString *)mqttPublicTopic
                      mqttSubTopic:(NSString *)mqttSubTopic
                      mqttUsername:(NSString *)mqttUsername
                      mqttPassword:(NSString *)mqttPassword
                      httpEndpoint:(NSString *)httpEndpoint
                      httpCcsToken:(NSString *)httpCcsToken
                         httpDuuid:(NSString *)httpDuuid
                      httpAuthMode:(P2PHttpAuthMode)httpAuthMode
                     httpAuthToken:(NSString *)httpAuthToken
                stunServerAddress1:(NSString *)stunServerAddress1
                stunServerAddress2:(NSString *)stunServerAddress2
                          strategy:(P2pFilterStrategy)strategy
                   certificatePath:(NSString *)certificatePath
                          ipv6Only:(BOOL)ipv6Only
                             delay:(NSInteger)delay;


@end

NS_ASSUME_NONNULL_END
