//
//  CGCallback.h
//  CGModule
//
//  Created by Cornelli Fabio on 21/06/18.
//  Copyright © 2018 Cornelli Fabio. All rights reserved.
//
#import "CMGError.h"

@protocol CGCallback<NSObject>

-(void)onConnect;
-(void)onDisconnect;
-(void)onError:(CMGError)error;

@end
