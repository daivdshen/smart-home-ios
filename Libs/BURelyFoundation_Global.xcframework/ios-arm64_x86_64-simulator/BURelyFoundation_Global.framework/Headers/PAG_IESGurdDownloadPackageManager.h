//
//  PAG_IESGurdDownloadPackageManager.h
//  IESGeckoKit
//
//  Created by chenyuchuan on 2019/6/10.
//

#import <Foundation/Foundation.h>

#import "PAG_IESGeckoResourceModel.h"

NS_ASSUME_NONNULL_BEGIN

typedef void(^PAG_IESGurdDownloadPackageResultBlock)(NSString *packagePath, BOOL isPatch, NSError *error);

@interface PAG_IESGurdDownloadPackageManager : NSObject

+ (instancetype)sharedManager;

- (void)downloadPackageForAccessKey:(NSString *)accessKey
                             config:(PAG_IESGurdResourceModel *)config
                        resultBlock:(PAG_IESGurdDownloadPackageResultBlock)resultBlock;

@end

NS_ASSUME_NONNULL_END
