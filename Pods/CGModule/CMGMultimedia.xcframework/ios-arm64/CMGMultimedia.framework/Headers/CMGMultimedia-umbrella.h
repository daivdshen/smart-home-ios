#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "AudioPlayer.h"
#import "CallMediaController.h"
#import "CMGMultimedia.h"
#import "MediaController.h"
#import "HardwareVideoDecoder.h"
#import "SoftwareVideoDecoder.h"
#import "VideoDecoder.h"
#import "VideoPlayer.h"
#import "VideoReceiver.h"

FOUNDATION_EXPORT double CMGMultimediaVersionNumber;
FOUNDATION_EXPORT const unsigned char CMGMultimediaVersionString[];

