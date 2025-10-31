//
//  PAG_IESGurdApplyPackageManager.h
//  IESGeckoKit
//
//  Created by chenyuchuan on 2019/6/27.
//

#import <Foundation/Foundation.h>

#import "PAG_IESGeckoDefines.h"
#import "PAG_IESGeckoResourceModel.h"

NS_ASSUME_NONNULL_BEGIN

@class PAG_IESGurdApplyPackageManager;

@protocol PAG_IESGurdApplyPackageManagerDelegate <NSObject>

- (void)applyPackageManager:(PAG_IESGurdApplyPackageManager *)manager
didApplyPackageForAccessKey:(NSString *)accessKey
                    channel:(NSString *)channel;

@end

typedef BOOL(^PAGGurdApplyPackageZipBlock)(NSString *zipFilePath,NSString *tempDirectory, BOOL a,BOOL b,NSString *_Nullable c, NSError **error,id _Nullable d);

@interface PAG_IESGurdApplyPackageManager : NSObject

@property (nonatomic, weak) id<PAG_IESGurdApplyPackageManagerDelegate> delegate;

@property (nonatomic, copy) PAGGurdApplyPackageZipBlock zipBlock;

+ (instancetype)sharedManager;

- (void)applyAllInactiveCacheWithCompletion:(IESGurdSyncStatusBlock)completion;

- (void)applyInactiveCacheForAccessKey:(NSString *)accessKey
                               channel:(NSString *)channel
                            completion:(IESGurdSyncStatusBlock)completion;

- (void)saveInactivePackageMetaDataWithAccessKey:(NSString *)accessKey
                                          config:(PAG_IESGurdResourceModel *)config
                                         isPatch:(BOOL)isPatch;

- (void)cleanTempFiles;

@end

NS_ASSUME_NONNULL_END
