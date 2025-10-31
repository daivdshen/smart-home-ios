/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 * (c) Fabrice Aneche
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/*
* This file may have been modified by ByteDance Ltd.
* Original Files：NSData+ImageContentType/SDWebImage (https://github.com/SDWebImage/SDWebImage)
* Copyright Poitrey <rs@dailymotion.com>
* SPDX-License-Identifier: MIT
*/

#import <Foundation/Foundation.h>
#import "PAG_SDWebImageCompat.h"

/**
 You can use switch case like normal enum. It's also recommended to add a default case. You should not assume anything about the raw value.
 For custom coder plugin, it can also extern the enum for supported format. See `SDImageCoder` for more detailed information.
 */
typedef NSInteger PAG_SDImageFormat NS_TYPED_EXTENSIBLE_ENUM;
static const PAG_SDImageFormat PAG_SDImageFormatUndefined = -1;
static const PAG_SDImageFormat PAG_SDImageFormatJPEG      = 0;
static const PAG_SDImageFormat PAG_SDImageFormatPNG       = 1;
static const PAG_SDImageFormat PAG_SDImageFormatGIF       = 2;
static const PAG_SDImageFormat PAG_SDImageFormatTIFF      = 3;
static const PAG_SDImageFormat PAG_SDImageFormatWebP      = 4;
static const PAG_SDImageFormat PAG_SDImageFormatHEIC      = 5;
static const PAG_SDImageFormat PAG_SDImageFormatHEIF      = 6;
static const PAG_SDImageFormat PAG_SDImageFormatPDF       = 7;
static const PAG_SDImageFormat PAG_SDImageFormatSVG       = 8;

/**
 NSData category about the image content type and UTI.
 */
@interface NSData (PAG_ImageContentType)

/**
 *  Return image format
 *
 *  @param data the input image data
 *
 *  @return the image format as `BU_SDImageFormat` (enum)
 */
+ (PAG_SDImageFormat)sdPAG_imageFormatForImageData:(nullable NSData *)data;

/**
 *  Convert BU_SDImageFormat to UTType
 *
 *  @param format Format as BU_SDImageFormat
 *  @return The UTType as CFStringRef
 */
+ (nonnull CFStringRef)sdPAG_UTTypeFromImageFormat:(PAG_SDImageFormat)format CF_RETURNS_NOT_RETAINED NS_SWIFT_NAME(sdBu_UTType(from:));

/**
 *  Convert UTTyppe to BU_SDImageFormat
 *
 *  @param uttype The UTType as CFStringRef
 *  @return The Format as BU_SDImageFormat
 */
+ (PAG_SDImageFormat)sdPAG_imageFormatFromUTType:(nonnull CFStringRef)uttype;

@end
