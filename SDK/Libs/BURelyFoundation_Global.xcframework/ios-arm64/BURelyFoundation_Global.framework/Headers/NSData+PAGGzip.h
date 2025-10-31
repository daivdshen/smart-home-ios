//
//  NSData+PAGGzip.h
//  PAGAdSDK
//
//  Created by ByteDance on 2023/2/16.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSData (PAGGzip)

- (nullable NSData *)dataWithGzipPack;

@end

NS_ASSUME_NONNULL_END
