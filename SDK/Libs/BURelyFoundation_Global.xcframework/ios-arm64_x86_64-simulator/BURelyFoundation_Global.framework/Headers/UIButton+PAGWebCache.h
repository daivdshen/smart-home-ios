/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "PAG_SDWebImageCompat.h"

#if SD_UIKIT

#import "PAG_SDWebImageManager.h"

/**
 * Integrates SDWebImage async downloading and caching of remote images with UIButton.
 */
@interface UIButton (BUWebCache)

#pragma mark - Image

/**
 * Get the current image URL.
 */
@property (nonatomic, strong, readonly, nullable) NSURL *sdPAG_currentImageURL;

/**
 * Set the button `image` with an `url`.
 *
 * The download is asynchronous and cached.
 *
 * @param url   The url for the image.
 * @param state The state that uses the specified title. The values are described in UIControlState.
 */
- (void)sdPAG_setImageWithURL:(nullable NSURL *)url
                  forState:(UIControlState)state NS_REFINED_FOR_SWIFT;

/**
 * Set the button `image` with an `url`, placeholder and custom options.
 *
 * The download is asynchronous and cached.
 *
 * @param url            The url for the image.
 * @param state          The state that uses the specified title. The values are described in UIControlState.
 * @param placeholder    The image to be set initially, until the image request finishes.
 * @param options        The options to use when downloading the image. @see PAG_SDWebImageOptions for the possible values.
 * @param completedBlock A block called when operation has been completed. This block has no return value
 *                       and takes the requested UIImage as first parameter. In case of error the image parameter
 *                       is nil and the second parameter may contain an NSError. The third parameter is a Boolean
 *                       indicating if the image was retrieved from the local cache or from the network.
 *                       The fourth parameter is the original image url.
 */
- (void)sdPAG_setImageWithURL:(nullable NSURL *)url
                  forState:(UIControlState)state
          placeholderImage:(nullable UIImage *)placeholder
                   options:(PAG_SDWebImageOptions)options
                 completed:(nullable PAG_SDExternalCompletionBlock)completedBlock;

/**
 * Set the button `image` with an `url`, placeholder and custom options.
 *
 * The download is asynchronous and cached.
 *
 * @param url            The url for the image.
 * @param state          The state that uses the specified title. The values are described in UIControlState.
 * @param placeholder    The image to be set initially, until the image request finishes.
 * @param options        The options to use when downloading the image. @see PAG_SDWebImageOptions for the possible values.
 * @param progressBlock  A block called while image is downloading
 *                       @note the progress block is executed on a background queue
 * @param completedBlock A block called when operation has been completed. This block has no return value
 *                       and takes the requested UIImage as first parameter. In case of error the image parameter
 *                       is nil and the second parameter may contain an NSError. The third parameter is a Boolean
 *                       indicating if the image was retrieved from the local cache or from the network.
 *                       The fourth parameter is the original image url.
 */
- (void)sdPAG_setImageWithURL:(nullable NSURL *)url
                  forState:(UIControlState)state
          placeholderImage:(nullable UIImage *)placeholder
                   options:(PAG_SDWebImageOptions)options
                  progress:(nullable SDImageLoaderProgressBlock)progressBlock
                 completed:(nullable PAG_SDExternalCompletionBlock)completedBlock;

/**
 * Set the button `image` with an `url`, placeholder, custom options and context.
 *
 * The download is asynchronous and cached.
 *
 * @param url            The url for the image.
 * @param state          The state that uses the specified title. The values are described in UIControlState.
 * @param placeholder    The image to be set initially, until the image request finishes.
 * @param options        The options to use when downloading the image. @see PAG_SDWebImageOptions for the possible values.
 * @param context        A context contains different options to perform specify changes or processes, see `BU_SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 * @param progressBlock  A block called while image is downloading
 *                       @note the progress block is executed on a background queue
 * @param completedBlock A block called when operation has been completed. This block has no return value
 *                       and takes the requested UIImage as first parameter. In case of error the image parameter
 *                       is nil and the second parameter may contain an NSError. The third parameter is a Boolean
 *                       indicating if the image was retrieved from the local cache or from the network.
 *                       The fourth parameter is the original image url.
 */
- (void)sdPAG_setImageWithURL:(nullable NSURL *)url
                  forState:(UIControlState)state
          placeholderImage:(nullable UIImage *)placeholder
                   options:(PAG_SDWebImageOptions)options
                   context:(nullable SDWebImageContext *)context
                  progress:(nullable SDImageLoaderProgressBlock)progressBlock
                 completed:(nullable PAG_SDExternalCompletionBlock)completedBlock;

#pragma mark - Background Image

/**
 * Get the current background image URL.
 */
@property (nonatomic, strong, readonly, nullable) NSURL *sdPAG_currentBackgroundImageURL;

@end

#endif
