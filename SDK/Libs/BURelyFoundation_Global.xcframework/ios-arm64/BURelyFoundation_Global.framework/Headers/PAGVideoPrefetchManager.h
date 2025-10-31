//
//  PAGVideoPrefetchManager.h
//  BUAdSDK
//
//  Created by 李盛 on 2018/9/20.
//  Copyright © 2018年 bytedance. All rights reserved.
//

/*
* This file may have been modified by ByteDance Ltd.
* Original Files：VideoPrefetchManager/ZFPlayer (http://github.com/renzifeng)
* Copyright  2016 任子丰.
* SPDX-License-Identifier: MIT
*/

#import <Foundation/Foundation.h>
#import "PAGLPlayerSettingsProtocol.h"

typedef void (^prefetchVideoCancelBlock)(NSInteger preloadSize);

@interface PAGVideoPrefetchManager : NSObject<PAGLPlayerSettingsProtocol>

+ (instancetype)sharedInstance;

/// 预加载视频
/// @param videoUrl 视频 URL
/// @param storgeName 视频文件存储名称
/// @param prefetchSize 预加载大小
/// @param prefetchStartBlock 预加载开始回调
/// @param prefetchFinshBlock 预加载结束回调
- (void)prefetchWithVideoUrl:(NSURL *)videoUrl
                   cachePath:(NSString *)cachePath
                  storgeName:(NSString *)storgeName
                prefetchSize:(NSUInteger)prefetchSize
          prefetchStartBlock:(void (^)(void))prefetchStartBlock
          prefetchFinshBlock:(void (^)(BOOL, NSInteger, NSError *, NSString *))prefetchFinshBlock
         prefetchCancelBlock:(prefetchVideoCancelBlock)prefetchCancelBlock;

/// 判断url是否正在下载
/// @param videoURL 视频 URL
- (BOOL)prefetchUrlInQueue:(NSURL *)videoURL;

/// 取消预加载，播放时应先取消预加载。
/// @param videoURL 视频 URL
- (void)cancelPrefetch:(NSURL *)videoURL;


/// 查询视频预加载大小
/// @param videoUrl 视频 URL
- (NSInteger)queryPrefetchSizeWithVideoUrl:(NSURL *)videoUrl cachePath:(NSString *)cachePath;

@end
