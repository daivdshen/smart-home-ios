//
//  CMGCallStartReceiver.h
//  CGModule
//
//  Created by Cornelli Fabio on 11/03/2019.
//  Copyright © 2019 Cornelli Fabio. All rights reserved.
//

#ifndef CMGCallStartReceiver_h
#define CMGCallStartReceiver_h

@protocol CMGCallStartReceiver<NSObject>

-(void)onCallStartWithName:(NSString*)name andDirection:(CMGCallDirection)direction;

@end

#endif /* CMGCallStartReceiver_h */
