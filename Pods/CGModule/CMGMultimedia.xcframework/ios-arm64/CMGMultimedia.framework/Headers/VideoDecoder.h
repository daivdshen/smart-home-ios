//
//  VideoDecoder.h
//  CMGMultimedia
//
//  Created by Cornelli Fabio on 11/06/2020.
//  Copyright © 2020 Comelit. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ComelitCoreKit/CMGFrameInfo.h>
#import "VideoReceiver.h"

NS_ASSUME_NONNULL_BEGIN

@protocol VideoDecoder<NSObject>

@property (nonatomic, strong, nullable) id<VideoReceiver> videoReceiver;

- (void)decodeRawVideoFrame:(NSData *)frameData withSize:(NSInteger)frameSize frameInfo:(nonnull CMGFrameInfo *)frameInfo;

@end

NS_ASSUME_NONNULL_END
