//
//  PAGRemoteImageBridge.h
//  BURelyFoundation_Global
//
//  Created by Willie on 2024/5/30.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol PAGRemoteImageProtocol <NSObject>

@optional

+ (void)imageWithFileName:(NSString *)name
               completion:(void(^)(UIImage *image))completion;

@end

@interface PAGRemoteImageBridge : NSObject <PAGRemoteImageProtocol>

@end

NS_ASSUME_NONNULL_END
