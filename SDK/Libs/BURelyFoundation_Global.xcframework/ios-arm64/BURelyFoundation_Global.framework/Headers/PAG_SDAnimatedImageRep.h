/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/*
* This file may have been modified by ByteDance Ltd.
* Original Files：SDAnimatedImageRep/SDWebImage (https://github.com/SDWebImage/SDWebImage)
* Copyright Poitrey <rs@dailymotion.com>
* SPDX-License-Identifier: MIT
*/

#import "PAG_SDWebImageCompat.h"

#if SD_MAC

/**
 A subclass of `NSBitmapImageRep` to fix that GIF loop count issue because `NSBitmapImageRep` will reset `NSImageCurrentFrameDuration` by using `kCGImagePropertyGIFDelayTime` but not `kCGImagePropertyGIFUnclampedDelayTime`.
 Built in GIF coder use this instead of `NSBitmapImageRep` for better GIF rendering. If you do not want this, only enable `SDImageIOCoder`, which just call `NSImage` API and actually use `NSBitmapImageRep` for GIF image.
 This also support APNG format using `SDImageAPNGCoder`. Which provide full alpha-channel support and the correct duration match the `kPAGCGImagePropertyAPNGUnclampedDelayTime`.
 */
@interface PAG_SDAnimatedImageRep : NSBitmapImageRep

@end

#endif
