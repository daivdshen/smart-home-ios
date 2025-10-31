/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/*
* This file may have been modified by ByteDance Ltd.
* Original Files：SDImageAPNGCoder/SDWebImage (https://github.com/SDWebImage/SDWebImage)
* Copyright Poitrey <rs@dailymotion.com>
* SPDX-License-Identifier: MIT
*/

#import <Foundation/Foundation.h>
#import "PAG_SDImageCoder.h"

/**
 Built in coder using ImageIO that supports APNG encoding/decoding
 */
@interface PAG_SDImageAPNGCoder : NSObject <PAG_SDProgressiveImageCoder, PAG_SDAnimatedImageCoder>

@property (nonatomic, class, readonly, nonnull) PAG_SDImageAPNGCoder *sharedCoder;

@end
