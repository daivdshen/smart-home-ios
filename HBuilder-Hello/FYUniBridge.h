//
//  FYUniBridge.h
//  HBuilder
//
//  Created by flynn.yang on 2024/5/4.
//  Copyright © 2024 DCloud. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DCUniModule.h"

NS_ASSUME_NONNULL_BEGIN

@interface FYUniBridge : DCUniModule
+ (void) notifyEvent: (NSString *) event;
@end

NS_ASSUME_NONNULL_END
