//
//  KsCustomSplashAdapter.h
//  DCUniAdKs
//
//  Created by wangzhitong on 2024/7/29.
//

#import <Foundation/Foundation.h>
#import <DCUniAdFoundation/DCUniAdFoundation.h>
NS_ASSUME_NONNULL_BEGIN

@interface DCUniOctCustomSplashAdapter : NSObject<DCUniAdCustomSplashAdapter>
- (instancetype)initWithBridge:(id<DCUniAdCustomSplashAdapterBridge>)bridge;
@end

NS_ASSUME_NONNULL_END
