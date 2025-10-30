//
//  PAG_RelyWeakProxy.h
//  BURelyFoundation
//
//  Created by zth on 2022/7/11.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PAG_RelyWeakProxy : NSProxy

@property (nonatomic, weak, readonly, nullable) id target;

- (nonnull instancetype)initWithTarget:(nonnull id)target;
+ (nonnull instancetype)proxyWithTarget:(nonnull id)target;

@end

NS_ASSUME_NONNULL_END
