//
//  MediaController.h
//  CMGMultimedia
//
//  Created by Cornelli Fabio on 11/06/2020.
//  Copyright © 2020 Comelit. All rights reserved.
//

#ifndef MEDIACONTROLLER_H
#define MEDIACONTROLLER_H

#import <ComelitCoreKit/CMGFrameInfo.h>
#import <ComelitCoreKit/CMGStreamInfo.h>
#import <ComelitCoreKit/MediaDirection.h>
#import <ComelitCoreKit/CMGMediaType.h>

@protocol MediaController <NSObject>

- (NSInteger)requestVideoKeyFrameGeneration;
- (CMGStreamInfo *)getStreamInfo;
- (NSData *)getVideoFrameWithFrameSize:(NSInteger *)size frameInfo:(CMGFrameInfo *)frameInfo;
- (NSInteger)sendVideoPacket:(NSData *)videoPacket length:(NSInteger)length timestamp:(NSTimeInterval)timestamp;

- (NSInteger)takeSnapshot:(NSString *)path;
- (NSInteger)startRecording:(NSString *)path;
- (NSInteger)stopRecording;

- (NSInteger)sendAudioPacket:(signed short *)pck length:(NSInteger)len timestamp:(NSTimeInterval)ts;
- (short *)getAudioPacket:(NSInteger *)nsamples timestamp:(NSTimeInterval *)timestamp;
- (BOOL)isOnSource:(MediaDirection)mediaDirection mediaType:(CMGMediaType)mediaType;

@end

#endif /* MEDIACONTROLLER_H */
