//
//  PAG_IESGurdLRUCacheLinkedNode.h
//  Pods
//
//  Created by 陈煜钏 on 2019/8/20.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PAG_IESGurdLRUCacheLinkedNode : NSObject

@property (nonatomic, copy) NSString *channel;

@property (nonatomic, weak) PAG_IESGurdLRUCacheLinkedNode *preLinkedNode;

@property (nonatomic, weak) PAG_IESGurdLRUCacheLinkedNode *nextLinkedNode;

+ (instancetype)nodeWithChannel:(NSString *)channel;

@end

NS_ASSUME_NONNULL_END
