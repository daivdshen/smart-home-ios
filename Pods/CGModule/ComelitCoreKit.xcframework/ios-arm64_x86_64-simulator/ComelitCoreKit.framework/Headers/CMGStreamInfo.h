//
//  CMGStreamInfo.h
//  ComelitSafeKit
//
//  Created by Busi Andrea on 05/08/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface CMGStreamInfo : NSObject

/**
 width[pixels]
 */
@property (nonatomic) NSInteger width;

/**
 height [pixels]
 */
@property (nonatomic) NSInteger height;

/**
 codec type
 */
@property (nonatomic) NSInteger ct;

/**
 framerate of flow (avaiable after rx 2 consecutive frames)
 */
@property (nonatomic) NSInteger fps;

@end

NS_ASSUME_NONNULL_END
