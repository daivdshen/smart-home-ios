/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "PAG_SDWebImageCompat.h"

typedef NS_ENUM(NSUInteger, PAG_SDImageScaleMode) {
    PAG_SDImageScaleModeFill = 0,
    PAG_SDImageScaleModeAspectFit = 1,
    PAG_SDImageScaleModeAspectFill = 2
};

#if SD_UIKIT || SD_WATCH
typedef UIRectCorner PAG_SDRectCorner;
#else
typedef NS_OPTIONS(NSUInteger, PAG_SDRectCorner) {
    PAG_SDRectCornerTopLeft     = 1 << 0,
    PAG_SDRectCornerTopRight    = 1 << 1,
    PAG_SDRectCornerBottomLeft  = 1 << 2,
    PAG_SDRectCornerBottomRight = 1 << 3,
    PAG_SDRectCornerAllCorners  = ~0UL
};
#endif

/**
 Provide some commen method for `UIImage`.
 Image process is based on Core Graphics and vImage.
 */
@interface UIImage (PAGTransform)

#pragma mark - Image Geometry

/**
 Rounds a new image with a given corner radius and corners.
 
 @param cornerRadius The radius of each corner oval. Values larger than half the
 rectangle's width or height are clamped appropriately to
 half the width or height.
 @param corners      A bitmask value that identifies the corners that you want
 rounded. You can use this parameter to round only a subset
 of the corners of the rectangle.
 @param borderWidth  The inset border line width. Values larger than half the rectangle's
 width or height are clamped appropriately to half the width
 or height.
 @param borderColor  The border stroke color. nil means clear color.
 @return The new image with the round corner.
 */
- (nullable UIImage *)sdPAG_roundedCornerImageWithRadius:(CGFloat)cornerRadius
                                              corners:(PAG_SDRectCorner)corners
                                          borderWidth:(CGFloat)borderWidth
                                          borderColor:(nullable UIColor *)borderColor;

@end
