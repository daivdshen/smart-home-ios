//
//  PAG_IESGurdDelegateDispatcher.h
//  IESGeckoKit
//
//  Created by chenyuchuan on 2019/6/5.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PAG_IESGurdDelegateDispatcher : NSObject

+ (instancetype)dispatcherWithProtocol:(Protocol *)protocol;

- (void)registerDelegate:(id)delegate;

@end

NS_ASSUME_NONNULL_END
