//
//  DCUniOctCustomDrawAdapter.h
//  DCUniAdKs
//
//  Created by wangzhitong on 2024/8/6.
//

#import <Foundation/Foundation.h>
#import <DCUniAdFoundation/DCUniAdFoundation.h>
NS_ASSUME_NONNULL_BEGIN

@interface DCUniOctCustomDrawAdapter : NSObject<DCUniAdCustomDrawAdapter>
- (instancetype)initWithBridge:(id<DCUniAdCustomDrawAdapterBridge>)bridge;
@end

NS_ASSUME_NONNULL_END
