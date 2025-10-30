//
//  PAG_IESGurdLRUCacheLinkedList.h
//  Pods
//
//  Created by 陈煜钏 on 2019/8/20.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PAG_IESGurdLRUCacheLinkedList : NSObject

@property (nonatomic, assign) NSInteger capacity;

- (void)appendLinkedNodeForChannel:(NSString *)channel;

- (void)bringLinkedNodeToHeadForChannel:(NSString *)channel;

- (NSArray<NSString *> *)channelsToBeDelete;

- (void)deleteLinkedNodeForChannel:(NSString *)channel;

@end

NS_ASSUME_NONNULL_END
