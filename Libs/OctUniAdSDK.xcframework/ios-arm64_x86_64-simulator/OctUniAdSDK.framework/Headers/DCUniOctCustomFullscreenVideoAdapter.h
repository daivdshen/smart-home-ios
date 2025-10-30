//
//  DCUniOctCustomFullscreenVideoAdapter.h
//  DCUniAdKs
//
//  Created by wangzhitong on 2024/8/6.
//

#import <Foundation/Foundation.h>
#import <DCUniAdFoundation/DCUniAdFoundation.h>
NS_ASSUME_NONNULL_BEGIN

@interface DCUniOctCustomFullscreenVideoAdapter : NSObject<DCUniAdCustomFullscreenVideoAdapter>
- (instancetype)initWithBridge:(id<DCUniAdCustomFullscreenVideoAdapterBridge>)bridge;
@end

NS_ASSUME_NONNULL_END
