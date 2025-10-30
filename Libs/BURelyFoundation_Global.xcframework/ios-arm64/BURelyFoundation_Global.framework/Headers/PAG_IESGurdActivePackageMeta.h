//
//  PAG_IESGurdActivePackageMeta.h
//  IESGeckoKit
//
//  Created by chenyuchuan on 2019/6/28.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PAG_IESGurdActivePackageMeta : NSObject <NSSecureCoding>

@property (nonatomic, copy) NSString *accessKey;

@property (nonatomic, copy) NSString *channel;

@property (nonatomic, copy) NSString *md5;

@property (nonatomic, assign) uint64_t version;

@property (nonatomic, assign) uint64_t packageID;

@property (nonatomic, assign) NSInteger packageType;

@end

NS_ASSUME_NONNULL_END
