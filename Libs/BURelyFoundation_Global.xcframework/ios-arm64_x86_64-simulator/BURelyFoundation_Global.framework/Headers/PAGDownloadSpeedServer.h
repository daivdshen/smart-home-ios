//
//  PAGDownloadSpeedServer.h
//  BUFoundation
//
//  Created by bytedance on 2021/4/22.
//

#import <Foundation/Foundation.h>

@class PAGDownloadSpeedModel;

NS_ASSUME_NONNULL_BEGIN

@interface PAGDownloadSpeedServer : NSObject

+ (instancetype)server;

- (void)startDownloadWithUrl:(NSURL *)url;

- (PAGDownloadSpeedModel *)downloadingWithUrl:(NSURL *)url;

- (void)finishDownloadWithUrl:(NSURL *)url;

@end

@interface PAGDownloadSpeedModel : NSObject

@property (atomic, assign) NSTimeInterval startDownloadTime;

@property (atomic, assign) NSTimeInterval finishDownloadTime;

@property (atomic, assign)    NSInteger downloadSize;

@property (nonatomic, assign) double speed;

@end

NS_ASSUME_NONNULL_END
