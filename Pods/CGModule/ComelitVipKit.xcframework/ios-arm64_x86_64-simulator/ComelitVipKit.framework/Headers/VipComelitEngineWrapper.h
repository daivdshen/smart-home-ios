//
//  VipComelitEngineWrapper.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 12/07/19.
//  Copyright © 2019 ComelitGroup. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ComelitCoreKit/ComelitEngineWrapper.h>
#import <ComelitCoreKit/CMGP2PParameters.h>
#import <ComelitCoreKit/UnitCapability.h>
#import <ComelitCoreKit/UnitFlag.h>
#import "ActuatorAction.h"
#import "ChannelType.h"

NS_ASSUME_NONNULL_BEGIN

@interface VipComelitEngineWrapper : ComelitEngineWrapper

- (instancetype)initWithEngine:(NSObject<ComelitEngine> *)engine
             jsonSocketHandler:(JsonSocketHandler *)socketHandler
                 messageParser:(id<UnitMessageParserProtocol>)messageParser
                useViperTunnel:(BOOL)useViperTunnel NS_UNAVAILABLE;
- (instancetype)initWithJsonSocketHandler:(JsonSocketHandler *)socketHandler
                            messageParser:(id<UnitMessageParserProtocol>)messageParser
                           useViperTunnel:(BOOL)useViperTunnel;

#pragma mark - Manage channel

- (NSInteger)openChannel:(NSInteger)sysId
             channelType:(ChannelType)channelType;

- (NSInteger)closeChannel:(NSInteger)sysId
              channelType:(ChannelType)channelType;

- (NSInteger)sendOnChannel:(NSInteger)sysId
               channelType:(ChannelType)channelType
                      data:(NSString *)data;

#pragma mark - Manage Tunnel

- (NSInteger)createViperTunnel:(NSInteger)sysId
                     ipAddress:(NSString *)ipAddress
                       tcpPort:(NSInteger)tcpPort
                       udpPort:(NSInteger)udpPort
                       timeout:(NSInteger)timeout;

- (NSInteger)createViperTunnel:(NSInteger)sysId
                 p2pParameters:(CMGP2PParameters *)p2pInfo;

#pragma mark - Manage vipunit

- (NSInteger)createVipUnit:(NSInteger)sysId
                vipAddress:(NSString *)vipAddress
                subAddress:(NSString *)subAddress
              capabilities:(UnitCapability)capabilities
                     flags:(UnitFlag)flags;

- (NSInteger)removeVipUnit:(NSInteger)sysId
                 vipUnitId:(NSInteger)vipUnitId;

- (NSInteger)setVipUnitMaxRtpPayloadSize:(NSInteger)sysId
                                  unitId:(NSInteger)unitId
                                    size:(NSInteger)size;

- (NSInteger)setVipUnitBitrate:(NSInteger)sysId
                        unitId:(NSInteger)unitId
                       enabled:(BOOL)enabled
                       bitrate:(NSInteger)bitrate;

- (NSInteger)setVipUnitRtpMaxVideoResolution:(NSInteger)sysId
                                      unitId:(NSInteger)unitId
                                        xMax:(NSInteger)xMax
                                        yMax:(NSInteger)yMax
                                         fps:(NSInteger)fps;

- (NSInteger)setVipUnitRtpPreferredVideoResolution:(NSInteger)sysId
                                            unitId:(NSInteger)unitId
                                             xPref:(NSInteger)xPref
                                             yPref:(NSInteger)yPref;

#pragma mark - Video/Audio

- (NSInteger)addReceivedPacketFromSocket:(NSInteger)sysId
                                    data:(NSData *)buffer
                                    size:(NSInteger)bytesRead;

- (NSInteger)requestVideo:(NSInteger)sysId
                vipUnitId:(NSInteger)vipUnitId
                    fsmId:(NSInteger)fsmId;

#pragma mark - Call actions

- (NSInteger)releaseCall:(NSInteger)sysId
                  unitId:(NSInteger)unitId
                   fsmId:(NSInteger)fsmId
                unitType:(UnitType)unitType;

- (NSInteger)answerCall:(NSInteger)sysId
              vipUnitId:(NSInteger)vipUnitId
                  fsmId:(NSInteger)fsmId;

- (NSInteger)startOutgoingCall:(NSInteger)sysId
                     vipUnitId:(NSInteger)vipUnitId
                    vipAddress:(NSString *)vipAddress
                    subAddress:(NSString *)subAddress;

#pragma mark - Vip element actions

- (NSInteger)sendOpenDoor:(NSInteger)sysId
                vipUnitId:(NSInteger)vipUnitId
                  vipAddr:(NSString *)vipaddr
               arrayRelay:(NSInteger *)arrayRelay
                 relayNum:(NSInteger)relayNum;

- (NSInteger)sendOpenDoorOnCall:(NSInteger)sysId
                      vipUnitId:(NSInteger)vipUnitId
                          fsmId:(NSInteger)fsmId
                     arrayRelay:(NSInteger *)arrayRelay
                       numRelay:(NSInteger)numRelay;

- (NSInteger)sendOpenActuator:(NSInteger)sysId
                    vipUnitId:(NSInteger)vipUnitId
                   vipAddress:(NSString *)vipAddress
                        relay:(NSInteger)relay
                       module:(NSInteger)module;

- (NSInteger)sendOpenActuatorAction:(NSInteger)sysId
                          vipUnitId:(NSInteger)vipUnitId
                         vipAddress:(NSString *)vipAddress
                              relay:(NSInteger)relay
                             module:(NSInteger)module
                             action:(ActuatorAction)action;

#pragma mark - Vip call times

- (NSInteger)setCallTimeParam:(NSInteger)sysId
                    vipUnitId:(NSInteger)vipUnitId
                        param:(CallTimeParam)param value:(NSInteger)value;

#pragma mark - Library log session

- (NSInteger)logStartSession:(NSInteger)sysId
                         tag:(NSString *)tag;

- (NSInteger)logAddEvent:(NSInteger)sysId
               eventName:(NSString *)eventName;

- (NSInteger)logAddElement:(NSInteger)sysId
                       key:(NSString *)key
                     value:(NSString *)value;

- (NSString *)logStopSession:(NSInteger)sysId;

@end

NS_ASSUME_NONNULL_END
