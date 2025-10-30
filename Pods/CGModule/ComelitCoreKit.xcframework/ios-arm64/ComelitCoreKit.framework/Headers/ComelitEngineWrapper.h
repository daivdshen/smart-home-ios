//
//  ComelitEngine.h
//  ComelitCoreKit
//
//  Created by Mutti Simone on 20/09/17.
//  Copyright © 2017 ComelitGroup. All rights reserved.
//
//  This class is a wrapper around libComelit.
//  The class provides all the common methods to exchange information with a Comelit system.
//

#import <Foundation/Foundation.h>
#import "ComelitEngine.h"
#import <ComelitCoreKit/CMGLogger.h>
#import "JsonSocketHandler.h"

NS_ASSUME_NONNULL_BEGIN

@class ComelitEngineWrapper;
@class ComelitEventBus;

@protocol ComelitEngineWrapperDelegate <NSObject>

@optional
- (void)engineWrapperDidFailOnCreation:(ComelitEngineWrapper *)engineWrapper;
@end


@interface ComelitEngineWrapper : NSObject

@property (strong, nonatomic, readonly) ComelitEventBus *comelitEventBus;
@property (weak, nonatomic, nullable) id<ComelitEngineWrapperDelegate> delegate;

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithEngine:(NSObject<ComelitEngine> *)engine
             jsonSocketHandler:(JsonSocketHandler *)socketHandler
                 messageParser:(id<UnitMessageParserProtocol>)messageParser
                useViperTunnel:(BOOL)useViperTunnel;

#pragma mark - Engine Lifecycle

- (void)reinitLibraryWithSeed:(NSUInteger)seed;

- (void)start;
- (void)stop;
- (BOOL)isEngineAlive;

- (NSInteger)createBaseSystem;
- (NSInteger)removeSystem:(NSInteger)sysId;

#pragma mark - Manage RTSP

- (NSInteger)createRtspUnit:(NSInteger)sysId
                      flags:(UnitFlag)flags;

- (NSInteger)removeRtspUnit:(NSInteger)sysId
                 rtspUnitId:(NSInteger)rtspUnitId;

- (NSInteger)rtspSetCallTimeParam:(NSInteger)sysId
                           unitId:(NSInteger)rtspUnitId
                            param:(CallTimeParam)param
                            value:(NSInteger)value;

- (NSInteger)rtspStartCall:(NSInteger)sysId
                rtspUnitId:(NSInteger)rtspUnitId
                       url:(NSString *)url
                      user:(NSString *)user
                      pass:(NSString *)pass;

- (NSInteger)rtspStopCall:(NSInteger)sysId
                   unitId:(NSInteger)rtspUnitId
                    fsmId:(NSInteger)rtspCallId;

- (NSInteger)rtspReleaseCall:(NSInteger)sysId
                      unitId:(NSInteger)rtspUnitId
                       fsmId:(NSInteger)rtspCallId;

- (NSInteger)rtspStopUnit:(NSInteger)sysId
               rtspUnitId:(NSInteger)rtspUnitId;

#pragma mark - Video/Audio

- (NSInteger)getStreamInfo:(NSInteger)sysId
                  unitType:(UnitType)unitType
                    unitId:(NSInteger)unitId
                     fsmId:(NSInteger)fsmId
                streamInfo:(nonnull CMGStreamInfo *)cmgStreamInfo;

- (NSData *)getVideoFrameWithSysId:(NSInteger)sysId
                            unitId:(NSInteger)unitId
                             fsmId:(NSInteger)fsmId
                          unitType:(UnitType)unitType
                         frameSize:(NSInteger *)frameSize
                             error:(NSInteger *)error
                         frameInfo:(CMGFrameInfo *)frameInfo;

- (NSInteger)decodeFrameWithSysId:(NSInteger)sysId
                           unitId:(NSInteger)unitId
                            fsmId:(NSInteger)fsmId
                         unitType:(UnitType)unitType
                        videoBuff:(NSData *)videoBuff
                 videoBuffDecoded:(NSData * _Nullable * _Nullable)videoBuffDecoded
                    sizeVideoBuff:(NSInteger)sizeVideoBuff
                        frameInfo:(CMGFrameInfo *)frameInfoRef;

- (void)sendVideoPacket:(NSInteger)sysId
               unitType:(UnitType)unitType
                 unitId:(NSInteger)unitId
                  fsmId:(NSInteger)fsmId
                    pck:(NSData *)pck
                 length:(NSInteger)length
              timestamp:(NSTimeInterval)timestamp
                  error:(NSInteger *)error;

- (NSInteger)requestVideoKeyFrameGeneration:(NSInteger)sysId
                                   unitType:(UnitType)unitType
                                     unitId:(NSInteger)unitId
                                      fsmId:(NSInteger)fsmId;

- (NSInteger)startRecording:(NSInteger)sysId
                   unitType:(UnitType)unitType
                     unitId:(NSInteger)unitId
                      fsmId:(NSInteger)fsmId
                       path:(NSString *)path
                        err:(NSInteger *)error;

- (NSInteger)stopRecording:(NSInteger)sysId
                  unitType:(UnitType)unitType
                    unitId:(NSInteger)unitId
                     fsmId:(NSInteger)fsmId
                       err:(NSInteger *)error;

- (NSInteger)saveFrame:(NSInteger)sysId
              unitType:(UnitType)unitType
                unitId:(NSInteger)unitId
                 fsmId:(NSInteger)fsmId
                  path:(NSString *)path
                   err:(NSInteger *)error;

- (BOOL)isOnSourceWithSysId:(NSInteger)sysId
                     unitId:(NSInteger)unitId
                   unitType:(UnitType)unitType
                      fsmId:(NSInteger)fsmId
                      error:(NSInteger *)error;

- (void)sendAudioPacket:(NSInteger)sysId
               unitType:(UnitType)unitType
                 unitId:(NSInteger)unitId
                  fsmId:(NSInteger)fsmId
                    pck:(signed short *)pck
                 length:(NSInteger)length
              timestamp:(NSTimeInterval)timestamp
                  error:(NSInteger *)error;

- (short * _Nullable)getAudioPacket:(NSInteger)sysId
                           unitType:(UnitType)unitType
                             unitId:(NSInteger)unitId
                              fsmId:(NSInteger)fsmId
                           nsamples:(NSInteger *)nsamples
                          timestamp:(NSTimeInterval *)timestamp
                              error:(NSInteger *)error;

@end

NS_ASSUME_NONNULL_END
