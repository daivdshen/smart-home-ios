//
//  VideoPlayer.h
//  CMGMultimedia
//
//  Created by Cornelli Fabio on 11/06/2020.
//  Copyright © 2020 Comelit. All rights reserved.
//

#import "VideoDecoder.h"
#import "VideoReceiver.h"
#import "MediaController.h"
#import <ComelitCoreKit/CMGLogger.h>
#import <ComelitCoreKit/UnitType.h>

NS_ASSUME_NONNULL_BEGIN

@interface VideoPlayer : NSObject

@property (nonatomic, strong) id<MediaController> mediaController;
@property (nonatomic, strong) id<VideoDecoder> videoDecoder;

@property (nonatomic) BOOL isRunning;

@property (nonatomic) NSOperationQueue *queue;


- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithMediaController:(id<MediaController>)mediaController videoDecoder:(id<VideoDecoder>)videoDecoder;

- (void)startPlayer;
- (void)stopPlayer;

- (BOOL)startRecording:(NSString *)path;
- (void)stopRecording;
- (BOOL)takeSnapshot:(NSString *)path;

- (void)setVideoReceiver:(nullable id<VideoReceiver>)videoReceiver;

@end

NS_ASSUME_NONNULL_END
