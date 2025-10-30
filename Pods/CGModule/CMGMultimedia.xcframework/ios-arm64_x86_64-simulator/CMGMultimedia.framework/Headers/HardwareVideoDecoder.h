//
//  HardwareVideoDecoder.h
//  CMGMultimedia
//
//  Created by Cornelli Fabio on 11/06/2020.
//  Copyright © 2020 Comelit. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VideoDecoder.h"

NS_ASSUME_NONNULL_BEGIN

@interface HardwareVideoDecoder : NSObject<VideoDecoder>

- (instancetype)init;

@end

NS_ASSUME_NONNULL_END
