//
//  CGPresenter.h
//  CGModule
//
//  Created by Cornelli Fabio on 21/06/18.
//  Copyright © 2018 Cornelli Fabio. All rights reserved.
//
#import "CGVideoReceiver.h"

@protocol CGPresenter<NSObject>

//ui actions
-(void)toggleAnswerCall;
-(void)toggleMute;
-(void)openDoor;
-(void)releaseCall;
-(void)onStealVideoButtonPressed;

-(void)setVideoReceiver:(id<CGVideoReceiver>)receiver;
-(void)setupAudio;

@end
