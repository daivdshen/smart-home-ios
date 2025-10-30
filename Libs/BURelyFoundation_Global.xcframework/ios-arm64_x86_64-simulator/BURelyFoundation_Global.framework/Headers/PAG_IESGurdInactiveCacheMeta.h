//
//  PAG_IESGurdInactiveCacheMeta.h
//  IESGeckoKit
//
//  Created by chenyuchuan on 2019/6/9.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PAG_IESGurdInactiveCacheMeta : NSObject <NSSecureCoding>

@property (nonatomic, copy) NSString *accessKey;

@property (nonatomic, copy) NSString *channel;

@property (nonatomic, copy) NSString *md5;

@property (nonatomic, assign) uint64_t version;

@property (nonatomic, assign) uint64_t packageID;

@property (nonatomic, assign) uint64_t patchID;

@property (nonatomic, assign) NSInteger packageType;

@property (nonatomic, assign) BOOL fromPatch;

@property (nonatomic, copy) NSString *fileName; //单文件激活后的文件名

@property (nonatomic, copy) NSString *groupName;

@property (nonatomic, copy) NSString *logId;

@end

NS_ASSUME_NONNULL_END
