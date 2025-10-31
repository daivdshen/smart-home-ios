//
//  KsCustomNativeAdapter.h
//  DCUniAdKs
//
//  Created by wangzhitong on 2024/7/31.
//

#import <Foundation/Foundation.h>
#import <DCUniAdFoundation/DCUniAdFoundation.h>
NS_ASSUME_NONNULL_BEGIN

@interface DCUniOctCustomNativeAdapter : NSObject<DCUniAdCustomNativeAdapter>
- (instancetype)initWithBridge:(id<DCUniAdCustomNativeAdapterBridge>)bridge;
@end

NS_ASSUME_NONNULL_END
