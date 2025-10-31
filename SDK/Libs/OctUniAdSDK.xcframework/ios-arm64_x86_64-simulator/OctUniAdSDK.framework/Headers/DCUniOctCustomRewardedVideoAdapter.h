//
//  DCUniOctCustomRewardedVideoAdapter.h
//  DCUniAdKs
//
//  Created by wangzhitong on 2024/8/5.
//

#import <Foundation/Foundation.h>
#import <DCUniAdFoundation/DCUniAdFoundation.h>
NS_ASSUME_NONNULL_BEGIN

@interface DCUniOctCustomRewardedVideoAdapter : NSObject<DCUniAdCustomRewardedVideoAdapter>
- (instancetype)initWithBridge:(id<DCUniAdCustomRewardedVideoAdapterBridge>)bridge;
@end

NS_ASSUME_NONNULL_END
