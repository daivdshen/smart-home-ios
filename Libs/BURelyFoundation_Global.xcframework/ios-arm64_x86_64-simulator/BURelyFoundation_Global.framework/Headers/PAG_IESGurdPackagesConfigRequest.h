//
//  PAG_IESGurdPackagesConfigRequest.h
//  IESGeckoKit
//
//  Created by 陈煜钏 on 2020/3/1.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PAG_IESGurdPackagesConfigRequest : NSObject

// accessKey : @[ channel, ... ]
@property (nonatomic, copy) NSDictionary<NSString *, NSArray<NSString *> *> *targetChannelsDictionary;
// accessKey : @{ channel : version }
@property (nonatomic, copy) NSDictionary<NSString *, NSDictionary<NSString *, NSString *> *> *targetVersionDictionary;
// accessKey : groupName
@property (nonatomic, copy) NSDictionary<NSString *, NSString *> *groupNamesDictionary;

@property (nonatomic, copy) NSString *businessDomain;

@property (nonatomic, copy) NSDictionary *customParams;

+ (instancetype)requestWithAccessKey:(NSString *)accessKey
                            channels:(NSArray<NSString *> *)channels
                        customParams:(NSDictionary *)customParams;
@end

NS_ASSUME_NONNULL_END
