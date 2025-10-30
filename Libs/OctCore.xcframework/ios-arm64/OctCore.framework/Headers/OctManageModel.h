//
//  OctManageModel.h
//  OctCoreSDK
//
//  Created by guantou on 2019/12/10.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 生命周期全局Model，跟随APP生命周期
*/
@interface OctManageModel : NSObject

+ (OctManageModel *)shareInstance;

//  appID
@property (nonatomic, copy) NSString *appId;

//  SessionID
@property (nonatomic, copy) NSString *SessionID;

@property (nonatomic, assign) uint64_t adInitStartTime;
@property (nonatomic, assign) uint64_t adInitEndTime;

@end

NS_ASSUME_NONNULL_END

