/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/*
* This file may have been modified by ByteDance Ltd.
* Original Files：NSBezierPath+RoundedCorners/SDWebImage (https://github.com/SDWebImage/SDWebImage)
* Copyright Poitrey <rs@dailymotion.com>
* SPDX-License-Identifier: MIT
*/

#import "PAG_SDWebImageCompat.h"

#if SD_MAC

#import "UIImage+PAGTransform.h"

@interface NSBezierPath (RoundedCorners)

/**
 Convenience way to create a bezier path with the specify rounding corners on macOS. Same as the one on `UIBezierPath`.
 */
+ (nonnull instancetype)sdBu_bezierPathWithRoundedRect:(NSRect)rect byRoundingCorners:(PAG_SDRectCorner)corners cornerRadius:(CGFloat)cornerRadius;

@end

#endif
