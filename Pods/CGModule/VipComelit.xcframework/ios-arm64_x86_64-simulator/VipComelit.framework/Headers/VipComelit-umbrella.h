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

#import "cfg.h"
#import "comelitengine.h"
#import "echo_canceller_bzaec.h"
#import "echo_canceller_webrtc.h"
#import "libdefs.h"
#import "libevent.h"
#import "libevent_rtsp.h"
#import "libevent_safe.h"
#import "libevent_tvcc.h"
#import "libevent_vip.h"
#import "logger.h"
#import "timing_utils.h"

FOUNDATION_EXPORT double VipComelitVersionNumber;
FOUNDATION_EXPORT const unsigned char VipComelitVersionString[];

