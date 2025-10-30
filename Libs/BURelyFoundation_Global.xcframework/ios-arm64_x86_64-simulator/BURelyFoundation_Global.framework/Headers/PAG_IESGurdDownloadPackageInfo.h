//
//  PAG_IESGurdDownloadPackageInfo.h
//  IESGeckoKit
//
//  Created by chenyuchuan on 2019/6/10.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PAG_IESGurdDownloadPackageInfo : NSObject

@property (nonatomic, assign, getter=isSuccessful) BOOL successful;

@property (nonatomic, assign) uint64_t downloadSize; //单位：bytes

@property (nonatomic, assign, getter=isPatch) BOOL patch; //是否增量包

@property (nonatomic, assign) uint64_t packageId;

@property (nonatomic, assign) NSInteger downloadDuration;

@property (nonatomic, strong) NSError *error;

@end

NS_ASSUME_NONNULL_END
