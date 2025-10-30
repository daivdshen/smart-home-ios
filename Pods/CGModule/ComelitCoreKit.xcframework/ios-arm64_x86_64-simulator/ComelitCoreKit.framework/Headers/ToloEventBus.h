//
//  ToloEventBus.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 19/08/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ToloEventBus : NSObject

//use this macro to subscribe for a particular type of event
#define COMELIT_EVENT_BUS_SUBSCRIBE(_event_type_) - (void) on##_event_type_:(_event_type_ *) event

- (instancetype)init;
- (void)registerToBus:(NSObject *)object;
- (void)unregisterFromBus:(NSObject *)object;
- (void)post:(id<NSObject>)object;

@end

NS_ASSUME_NONNULL_END
