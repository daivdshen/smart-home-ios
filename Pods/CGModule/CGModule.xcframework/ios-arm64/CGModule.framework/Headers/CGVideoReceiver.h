//
//  CGVideoReceiver.h
//  CGModule
//
//  Created by Cornelli Fabio on 21/06/18.
//  Copyright © 2018 Cornelli Fabio. All rights reserved.
//
#import <AVFoundation/AVSampleBufferDisplayLayer.h>
#import <UIKit/UIImage.h>

@protocol CGVideoReceiver<NSObject>

@property (nonatomic) AVSampleBufferDisplayLayer* videoDisplayLayer;

-(void) onFrameReceived:(UIImage*)frame;
@end
