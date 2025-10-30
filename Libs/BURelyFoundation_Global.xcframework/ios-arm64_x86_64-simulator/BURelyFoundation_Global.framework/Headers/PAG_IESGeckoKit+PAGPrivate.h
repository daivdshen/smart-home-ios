//
//  PAG_IESGurdKit+Private.h
//  PAG_IESGurdKit
//
//  Created by li keliang on 2019/3/1.
//

#import "PAG_IESGeckoKit.h"

NS_ASSUME_NONNULL_BEGIN

@interface PAG_IESGurdKit ()

+ (instancetype)sharedInstance;

@property (nonatomic, copy) NSString *appId;

@property (nonatomic, copy) NSString *appVersion;

@property (nonatomic, copy) NSString *deviceID;

@property (nonatomic, copy) NSString *domain;

@property (nonatomic, strong) id<PAG_IESGurdNetworkDelegate> networkDelegate;

@property (nonatomic, strong) id<PAG_IESGurdDownloaderDelegate> downloaderDelegate;

@end

NS_ASSUME_NONNULL_END
