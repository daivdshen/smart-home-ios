//
//  RtspReply.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 15/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, RtspReply) {
    RtspReplyOk,
    RtspReplyErrInvalidUrl,
    RtspReplyErrSocket,
    RtspReplyErrNoRep,
    RtspReplyErrCmd,
    RtspReplyErrLogin,
    RtspReplyErrGeneric,
    RtspReplyErrUnsup,
    RtspReplyErrStatus,
    RtspReplyErrVideo,
    RtspReplyInvalidValue
};
