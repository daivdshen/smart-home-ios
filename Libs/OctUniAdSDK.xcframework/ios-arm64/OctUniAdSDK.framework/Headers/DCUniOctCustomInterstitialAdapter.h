//
//  DCUniOctCustomInterstitialAdapter.h
//  DCUniAdKs
//
//  Created by wangzhitong on 2024/8/6.
//

#import <Foundation/Foundation.h>
#import <DCUniAdFoundation/DCUniAdFoundation.h>
NS_ASSUME_NONNULL_BEGIN

@interface DCUniOctCustomInterstitialAdapter : NSObject<DCUniAdCustomInterstitialAdapter>
- (instancetype)initWithBridge:(id<DCUniAdCustomInterstitialAdapterBridge>)bridge;
@end

NS_ASSUME_NONNULL_END
