//
//  PAGQueueManager.h
//  BUFoundation
//
//  Created by Rush.D.Xzj on 2021/8/6.
//

#import <Foundation/Foundation.h>
#import "PAGQueueManagerConfig.h"

NS_ASSUME_NONNULL_BEGIN


typedef NS_ENUM(NSInteger, PAGHierarchy) {
    PAGHierarchy_BUAdSDK,
    PAGHierarchy_BUFoundation,
    PAGHierarchy_BURelyFoundation,
    PAGHierarchy_BUCore,
};

@interface PAGQueueManager : NSObject

@property (nonatomic, strong, readonly) PAGQueueManagerConfig *config;

- (dispatch_queue_t)serialQueueWithHierarchy:(PAGHierarchy)hierarchy suffixLabel:(NSString *)suffixLabel;
- (dispatch_queue_t)concurrentQueueWithHierarchy:(PAGHierarchy)hierarchy suffixLabel:(NSString *)suffixLabel;

#pragma mark - Signal
+ (instancetype)sharedInstance;
@end

NS_ASSUME_NONNULL_END
