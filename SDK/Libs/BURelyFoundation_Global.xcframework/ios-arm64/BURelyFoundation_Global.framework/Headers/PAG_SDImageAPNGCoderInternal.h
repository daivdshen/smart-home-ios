/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/*
* This file may have been modified by ByteDance Ltd.
* Original Files：SDImageAPNGCoderInternal/SDWebImage (https://github.com/SDWebImage/SDWebImage)
* Copyright Poitrey <rs@dailymotion.com>
* SPDX-License-Identifier: MIT
*/

#import "PAG_SDWebImageCompat.h"
#import "PAG_SDImageAPNGCoder.h"

@interface PAG_SDImageAPNGCoder ()

- (float)sdBu_frameDurationAtIndex:(NSUInteger)index source:(nonnull CGImageSourceRef)source;
- (NSUInteger)sdBu_imageLoopCountWithSource:(nonnull CGImageSourceRef)source;

@end
