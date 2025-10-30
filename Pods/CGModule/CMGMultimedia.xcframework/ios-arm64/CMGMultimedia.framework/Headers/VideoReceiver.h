//
//  VideoReceiver.h
//  CMGMultimedia
//
//  Created by Cornelli Fabio on 11/06/2020.
//  Copyright © 2020 Comelit. All rights reserved.
//

#import <AVFoundation/AVSampleBufferDisplayLayer.h>
#import <UIKit/UIImage.h>

NS_ASSUME_NONNULL_BEGIN

@protocol VideoReceiver <NSObject>

@property (nonatomic, nullable) AVSampleBufferDisplayLayer *videoDisplayLayer;

- (void)onFrameReceived:(UIImage *)frame;

@end

NS_ASSUME_NONNULL_END
