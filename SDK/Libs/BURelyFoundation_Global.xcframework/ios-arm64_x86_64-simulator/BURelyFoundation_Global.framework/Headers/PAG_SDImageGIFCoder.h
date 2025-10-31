/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/*
* This file may have been modified by ByteDance Ltd.
* Original Files：SDImageGIFCoder/SDWebImage (https://github.com/SDWebImage/SDWebImage)
* Copyright Poitrey <rs@dailymotion.com>
* SPDX-License-Identifier: MIT
*/


#import <Foundation/Foundation.h>
#import "PAG_SDImageCoder.h"

/**
 Built in coder using ImageIO that supports animated GIF encoding/decoding
 @note `SDImageIOCoder` supports GIF but only as static (will use the 1st frame).
 @note Use `SDImageGIFCoder` for fully animated GIFs. For `UIImageView`, it will produce animated `UIImage`(`NSImage` on macOS) for rendering. For `SDAnimatedImageView`, it will use `BU_SDAnimatedImage` for rendering.
 @note The recommended approach for animated GIFs is using `BU_SDAnimatedImage` with `SDAnimatedImageView`. It's more performant than `UIImageView` for GIF displaying(especially on memory usage)
 */
@interface PAG_SDImageGIFCoder : NSObject <PAG_SDProgressiveImageCoder, PAG_SDAnimatedImageCoder>

@property (nonatomic, class, readonly, nonnull) PAG_SDImageGIFCoder *sharedCoder;

@end
