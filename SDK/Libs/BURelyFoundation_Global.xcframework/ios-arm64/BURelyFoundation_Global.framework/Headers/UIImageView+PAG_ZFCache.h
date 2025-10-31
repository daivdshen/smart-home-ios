//
//  UIImageView+ZFCache.h
//  Player
//
// Copyright (c) 2016年 任子丰 ( http://github.com/renzifeng )
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

/*
* This file may have been modified by ByteDance Ltd.
* Original Files：UIImageView+ZFCache/ZFPlayer (http://github.com/renzifeng)
* Copyright  2016 任子丰.
* SPDX-License-Identifier: MIT
*/

#import <UIKit/UIKit.h>

typedef void (^PAG_ZFDownLoadDataCallBack)(NSData *data, NSError *error);
typedef void (^PAG_ZFDownloadProgressBlock)(unsigned long long total, unsigned long long current);

@interface PAG_ZFImageDownloader : NSObject<NSURLSessionDownloadDelegate>

@property (nonatomic, strong) NSURLSession *session;
@property (nonatomic, strong) NSURLSessionDownloadTask *task;

@property (nonatomic, assign) unsigned long long totalLength;
@property (nonatomic, assign) unsigned long long currentLength;

@property (nonatomic, copy) PAG_ZFDownloadProgressBlock progressBlock;
@property (nonatomic, copy) PAG_ZFDownLoadDataCallBack callbackOnFinished;

- (void)startDownloadImageWithUrl:(NSString *)url
                         progress:(PAG_ZFDownloadProgressBlock)progress
                         finished:(PAG_ZFDownLoadDataCallBack)finished;

@end

typedef void (^PAG_ZFImageBlock)(UIImage *image);

@interface UIImageView (PAG_ZFCache)

/**
 *  Get/Set the callback block when download the image finished.
 *
 *  param image The image object from network or from disk.
 */
@property (nonatomic, copy) PAG_ZFImageBlock pag_completion;

/**
 *  Image downloader
 */
@property (nonatomic, strong) PAG_ZFImageDownloader *pag_imageDownloader;

/**
 *	Specify the URL to download images fails, the number of retries, the default is 2
 */
@property (nonatomic, assign) NSUInteger pag_attemptToReloadTimesForFailedURL;

/**
 *	Will automatically download to cutting for UIImageView size of image.The default value is NO.
 *  If set to YES, then the download after a successful store only after cutting the image
 */
@property (nonatomic, assign) BOOL pag_shouldAutoClipImageToViewSize;

/**
 * Set the imageView `image` with an `url` and a placeholder.
 *
 * The download is asynchronous and cached.
 *
 * @param url         The url for the image.
 * @param placeholderImage The image to be set initially, until the image request finishes.
 */
- (void)setPAGImageWithURLString:(NSString *)url placeholder:(UIImage *)placeholderImage;

/**
 * Set the imageView `image` with an `url`, placeholder.
 *
 * The download is asynchronous and cached.
 *
 * @param url            The url for the image.
 * @param placeholderImage    The image to be set initially, until the image request finishes.
 * @param completion     A block called when operation has been completed. This block has no return value
 *                       and takes the requested UIImage as first parameter. In case of error the image parameter
 *                       is nil and the second parameter may contain an NSError. The third parameter is a Boolean
 *                       indicating if the image was retrieved from the local cache or from the network.
 *                       The fourth parameter is the original image url.
 */
- (void)setPAGImageWithURLString:(NSString *)url
                  placeholder:(UIImage *)placeholderImage
                   completion:(void (^)(UIImage *image))completion;

@end
