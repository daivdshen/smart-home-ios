//
//  AudioPlayer.h
//  CMGMultimedia
//
//  Created by Cornelli Fabio on 11/06/2020.
//  Copyright © 2020 Comelit. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#import <AudioToolbox/AudioQueue.h>
#import <AudioToolbox/AudioServices.h>

#import <TPCircularBuffer/TPCircularBuffer.h>
#import <TPCircularBuffer/TPCircularBuffer+AudioBufferList.h>

#import "MediaController.h"

#define APLAYBUF_SIZE   1920
#define AUDIO_PACKET_SIZE 160
#define AUDIO_RECORD_PACKET_SIZE 640
//AUDIO REMOTE I/O
#define kOutputBus 0
#define kInputBus 1

static AudioComponentDescription desc;
static AudioComponent component;
static AudioComponentInstance unit;
static AURenderCallbackStruct outputCallback;
static AURenderCallbackStruct inputCallback;
static AudioBufferList bufferRecList;

static TPCircularBuffer playBuffer;
static TPCircularBuffer recBuffer;

static int bufferAudioLength = 640*3;//todo local variable?
static NSInteger audioplay_nsamples; //todo local variable?

@interface AudioPlayer : NSObject


@property (nonatomic,strong) id<MediaController> mediaController;

@property (nonatomic) uint32_t rtp_ts;

@property (nonatomic) BOOL isRunning;
@property (nonatomic) BOOL isMute;
@property (nonatomic) BOOL isAudioStarted;

- (instancetype)initWithMediaController:(id<MediaController>)mediaController;

- (void)startPlayer;
- (void)stopPlayer;

- (void)startSound;
- (void)stopSound;

- (void)proximityChanged:(NSNotification *)notification;

@end
