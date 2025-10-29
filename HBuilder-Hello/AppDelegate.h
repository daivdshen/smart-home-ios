//
//  AppDelegate.h
//  Pandora
//
//  Created by Mac Pro_C on 12-12-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CGModule/CGModule.h>
#import <PushKit/PushKit.h>
#import <UserNotifications/UserNotifications.h>

extern int _uniArgc;
extern char** _uniArgv;
@interface AppDelegate : UIResponder <UIApplicationDelegate,PKPushRegistryDelegate>
{
    PKPushRegistry *pushRegistry;
}
@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) UINavigationController *rootViewController;
@end
