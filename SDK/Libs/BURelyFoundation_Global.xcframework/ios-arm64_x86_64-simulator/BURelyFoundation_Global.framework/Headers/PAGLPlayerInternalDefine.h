//
//  PAGPlayerInternalDefine.h
//  BUFoundation
//
//  Created by bytedance on 2020/12/17.
//

#ifndef PAGPlayerInternalDefine_h
#define PAGPlayerInternalDefine_h

typedef NS_ENUM(NSInteger, PAGVideoPlayerState) {
    PAGVideoPlayerStateFailed    = 0,
    PAGVideoPlayerStateBuffering = 1,
    PAGVideoPlayerStatePlaying   = 2,
    PAGVideoPlayerStateStopped   = 3,
    PAGVideoPlayerStatePause     = 4,
    PAGVideoPlayerStateDefault    = 5
};

typedef NS_ENUM(NSUInteger, PAGVideoPlayerDecoeMode) {
    PAGVideoPlayerDecoeMode_H265_Local = 0,
    PAGVideoPlayerDecoeMode_H265_Cache = 1,
    PAGVideoPlayerDecoeMode_H265_Remote = 2,
    
    PAGVideoPlayerDecoeMode_H264_Local = 3,
    PAGVideoPlayerDecoeMode_H264_Cache = 4,
    PAGVideoPlayerDecoeMode_H264_Remote = 5,
    PAGVideoPlayerDecoeMode_Last = 5,
};

@class PAGLPlayer;

@protocol PAGLVideoPlayerDelegate <NSObject>

@optional
/**
 This method is called when the player status changes.
 */
- (void)player:(PAGLPlayer *)player stateDidChanged:(PAGVideoPlayerState)playerState;

/**
 This method is called when the player is ready.
 */
- (void)playerReadyToPlay:(PAGLPlayer *)player;

/**
 This method is called when the player is ready, while application in background.
 */
- (void)playerReadyToPlayWhenApplicationEnterBackground:(PAGLPlayer *)player;

/**
 This method is called when the player plays completion or occurrs error.
 */
- (void)playerDidPlayFinish:(PAGLPlayer *)player error:(NSError *)error;

/**
 This method is called when the player is clicked.
 */
- (void)player:(PAGLPlayer *)player recognizeTapGesture:(UITapGestureRecognizer *)gesture;


/**
 This method is called when the view is clicked during ad play.
 */
- (void)playerTouchesBegan:(PAGLPlayer *)player;


/// Will be called when set BoundaryTime.
/// @param player player
- (void)playerBoundaryTimeReached:(PAGLPlayer *)player;

- (void)player:(PAGLPlayer *)player videoDidReachProgressTime:(NSTimeInterval)videoProgress duration:(NSTimeInterval)duration;

@end

@protocol PAGLVideoPlayerTimeDelegate <NSObject>

@optional

- (void)player:(PAGLPlayer *)player second:(NSTimeInterval)second;

@end



#endif /* PAGPlayerInternalDefine_h */
