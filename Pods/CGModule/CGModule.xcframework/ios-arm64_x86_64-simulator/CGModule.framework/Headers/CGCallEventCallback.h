//
//  CGCallEventCallback.h
//  CGModule
//
//  Created by Cornelli Fabio on 21/06/18.
//  Copyright © 2018 Cornelli Fabio. All rights reserved.
//
#import "CMGCallReleaseType.h"

@protocol CGCallEventCallback<NSObject>

-(void)onCallEnd;
-(void)onCallCapabilityVideo;
-(void)onCallCapabilityDoor;
-(void)onCallCapabilityAudio;
-(void)onCallAnswer;
-(void)onCallRelease:(CMGCallReleaseType)cause;
-(void)onCallRequestVideo;
-(void)onCallOpenDoor;
-(void)onCallToggleMic;
-(void)onCallStartRec;
-(void)onCallStopRec:(NSString*)path;
-(void)onCallVirtualButton1;
-(void)onCallVirtualButton2;

@end
