//
//  PAGTimer.h
//  BUAdSDK
//
//  Created by 李盛 on 2018/6/20.
//  Copyright © 2018年 bytedance. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PAGTimer : NSObject

@property (nonatomic, copy) NSString *runLoopMode;

+ (PAGTimer *)timerWithTimeInterval:(NSTimeInterval)seconds
                            target:(nullable id)target
                          selector:(SEL)aSelector
                           repeats:(BOOL)repeats;

+ (PAGTimer *)timerWithTimeInterval:(NSTimeInterval)seconds
                           repeats:(BOOL)repeats
                             block:(void (^)(PAGTimer *timer))block;

- (BOOL)isValid;
- (void)invalidate;
- (BOOL)isScheduled;
- (BOOL)scheduleNow;
- (BOOL)pause;
- (BOOL)resume; // 从起始时间恢复
- (NSTimeInterval)initialTimeInterval;

@end

NS_ASSUME_NONNULL_END
