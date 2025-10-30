//
//  CMGFrameInfo.h
//  ComelitCoreKit
//
//  Created by Busi Andrea on 05/08/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface CMGFrameInfo : NSObject

/**
 frame's width[pixels]
 */
@property (nonatomic) NSInteger width;

/**
 frame's height [pixels]
 */
@property (nonatomic) NSInteger height;

/**
 frame's timestamp
 */
@property (nonatomic) NSUInteger ts;

/**
 VideoPacketRx flags field
 */
@property (nonatomic) NSUInteger flags;

@end

NS_ASSUME_NONNULL_END
