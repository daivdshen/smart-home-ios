//
//  ComelitEngine.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 10/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "UnitType.h"
#import "CallTimeParam.h"
#import "MediaDirection.h"
#import "CMGMediaType.h"
#import "CMGFrameInfo.h"
#import "CMGStreamInfo.h"
#import "AVBufferParameter.h"
#import "UnitFlag.h"

NS_ASSUME_NONNULL_BEGIN

@protocol ComelitEngine <NSObject>

#pragma mark - Engine Lifecycle

- (void)initEngine:(NSString *)currentIp
    currentNetmask:(NSString *)currentNetmask
       currentPort:(NSInteger)currentPort
    useViperTunnel:(BOOL)useViperTunnel;
- (void)reinitLibraryWithSeed:(NSUInteger)seed;

- (void)start;
- (void)stop;
- (void)destroy;
- (BOOL)isEngineAlive;

- (NSInteger)createBaseSystem;
- (NSInteger)removeSystem:(NSInteger)sysId;

#pragma mark - RTSP

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

#pragma mark Audio/Video

- (NSInteger)getStreamInfo:(NSInteger)sysId
                  unitType:(UnitType)unitType
                    unitId:(NSInteger)unitId
                     fsmId:(NSInteger)fsmId
                streamInfo:(nonnull CMGStreamInfo *)cmgStreamInfo;

- (nullable NSData *)getVideoFrame:(NSInteger)sysId
                            unitId:(NSInteger)unitId
                             fsmId:(NSInteger)fsmId
                          unitType:(UnitType)unitType
                         frameSize:(NSInteger *)frameSize
                               err:(NSInteger *)error
                         frameInfo:(CMGFrameInfo *)frameInfoRef;

- (NSInteger)decodeFrame:(NSInteger)sysId
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

- (BOOL)isOnSource:(NSInteger)sysId
            unitId:(NSInteger)unitId
          unitType:(UnitType)unitType
             fsmId:(NSInteger)fsmId
             error:(NSInteger *)error
         direction:(MediaDirection)direction
             media:(CMGMediaType)media;

- (NSInteger)setVideoPixelFormat:(NSInteger)format
                        bitcolor:(NSInteger)bitcolor;

- (NSInteger)setAVBufferParam:(AVBufferParameter)parameter value:(NSInteger)value;

- (void)setVideoFrameSizeWithWidth:(NSInteger)width
                            height:(NSInteger)height;

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
