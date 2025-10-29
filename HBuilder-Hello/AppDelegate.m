//
//  AppDelegate.m
//  Pandora
//
//  Created by Mac Pro_C on 12-12-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "AppDelegate.h"
#import "PDRCore.h"
#import "PDRCommonString.h"
#import "ViewController.h"
#import "PDRCoreApp.h"
#import "PDRCoreAppManager.h"
#import "ComelitConnectionHandler.h"
#import <PushKit/PushKit.h>
#import <AVFoundation/AVFoundation.h>
#import <AVFoundation/AVFoundation.h>
#define ENABLEAD


@interface AppDelegate()<PDRCoreDelegate,UNUserNotificationCenterDelegate>
@property (strong, nonatomic) ViewController *h5ViewContoller;
@end

@implementation AppDelegate

@synthesize window = _window;
@synthesize rootViewController;
#pragma mark -
#pragma mark app lifecycle

NSString * const UserDefaultVoipPushToken2 = @"voip_push_token";

/*
 * @Summary:程序启动时收到push消息
 */
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    
    //ask for microphone usage permission
    [[AVAudioSession sharedInstance] requestRecordPermission:^(BOOL granted) {
        
        if (granted) {
            
            NSLog(@"Permission granted");
            
        }
        else {
            
            NSLog(@"Permission denied");
            
        }
        
    }];

    //Local notification permission
    UNAuthorizationOptions options = UNAuthorizationOptionAlert + UNAuthorizationOptionSound;
    UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];
    [center requestAuthorizationWithOptions:options
                          completionHandler:^(BOOL granted, NSError * _Nullable error) {
                              if (!granted) {
                                  NSLog(@"Permission not granted");
                              }
                          }];
    
    BOOL ret = [PDRCore initEngineWihtOptions:launchOptions
                                  withRunMode:PDRCoreRunModeNormal withDelegate:self];

    UIWindow *window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    self.window = window;
    
    ViewController *viewController = [[ViewController alloc] init];
    self.h5ViewContoller = viewController;
    
    UINavigationController *navigationController = [[UINavigationController alloc] initWithRootViewController:viewController];
    self.rootViewController = navigationController;
    navigationController.navigationBarHidden = YES;
    {
        [self startMainApp];
        self.h5ViewContoller.showLoadingView = YES;
    }
    self.window.rootViewController = navigationController;
    [self.window makeKeyAndVisible];
    
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onGotoFriend:) name:@"FYUniBridgeGotoPage" object:NULL];
    
    //Voip push notification (to use if callKitEnabled = YES)
    pushRegistry = [[PKPushRegistry alloc] initWithQueue:dispatch_get_main_queue()];
    pushRegistry.delegate = self;
    pushRegistry.desiredPushTypes = [NSSet setWithObject:PKPushTypeVoIP];
    
    
    [[ComelitConnectionHandler sharedManager] disconnectFromSystem];
    [self voipRegistration];
    return ret;
}

- (void) onGotoFriend: (NSNotification *) noti {
    NSDictionary *dic = noti.userInfo;
    NSLog(@"VCP Receive uniapp param");
    if(dic != nil) {
        NSString *page = dic[@"page"];
        NSDictionary *params = dic[@"params"];
        if (page != nil) {
            if ([page isEqualToString:@"loginVcp"]) {
                NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
                [defaults setObject:params[@"host"] forKey:@"ipAddress"];
                [defaults setObject:params[@"port"] forKey:@"port"];
                [defaults setObject:params[@"actCode"] forKey:@"actCode"];
                [defaults setObject:params[@"unitId"] forKey:@"unitId"];
                [defaults setObject:params[@"userId"] forKey:@"userId"];
                [defaults setObject:params[@"baseUrl"] forKey:@"serverHost"];
                NSLog(@"VCP Connection %@",noti.userInfo);
                dispatch_async(dispatch_get_main_queue(), ^{
                    [[ComelitConnectionHandler sharedManager] connectToSystem];
                });
            }
        }
    }
}

// Register for VoIP notifications
- (void) voipRegistration {
    dispatch_queue_t mainQueue = dispatch_get_main_queue();
  // Create a push registry object
  PKPushRegistry * voipRegistry = [[PKPushRegistry alloc] initWithQueue: mainQueue];
  // Set the registry's delegate to self
  voipRegistry.delegate = self;
  // Set the push type to VoIP
  voipRegistry.desiredPushTypes = [NSSet setWithObject:PKPushTypeVoIP];
}

+ (NSString *)stringFromDeviceToken:(NSData *)deviceToken {
    NSUInteger length = deviceToken.length;
    if (length == 0) {
        return nil;
    }
    const unsigned char *buffer = deviceToken.bytes;
    NSMutableString *hexString  = [NSMutableString stringWithCapacity:(length * 2)];
    for (int i = 0; i < length; ++i) {
        [hexString appendFormat:@"%02x", buffer[i]];
    }
    return [hexString copy];
}

// Handle updated push credentials
- (void)pushRegistry:(PKPushRegistry *)registry didUpdatePushCredentials: (PKPushCredentials *)credentials forType:(NSString *)type {
  // Register VoIP push token (a property of PKPushCredentials) with server
    if([credentials.token length] == 0) {
        NSLog(@"voip token NULL");
        return;
    }
    
    NSString *token;

    if (@available(iOS 13, *)) {
        token = [AppDelegate stringFromDeviceToken: credentials.token];
    } else {
        token = [[credentials.token description] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"<>"]];
        token = [token stringByReplacingOccurrencesOfString:@" " withString:@""];
    }

    NSLog(@"VoiP push token: %@",token);
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:token forKey:UserDefaultVoipPushToken2];
}
// Handle incoming pushes
- (void)pushRegistry:(PKPushRegistry *)registry didReceiveIncomingPushWithPayload:(PKPushPayload *)payload forType:(NSString *)type {

  NSLog(@"VCP @@ VoiP push received @@");
  [[CGModule sharedManager] handlePushNotification:payload forType:type];
//  [[ComelitConnectionHandler sharedManager] disconnectFromSystem];
    
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    ViewController *viewController = [[ViewController alloc] init];
    UINavigationController *navigationController = [[UINavigationController alloc] initWithRootViewController:viewController];
    self.window.rootViewController = navigationController;
    [self.window makeKeyAndVisible];
//    completion();

}

-(BOOL)getStatusBarHidden {
    return [self.h5ViewContoller getStatusBarHidden];
}

-(UIStatusBarStyle)getStatusBarStyle {
    return [self.h5ViewContoller getStatusBarStyle];
}
-(void)setStatusBarStyle:(UIStatusBarStyle)statusBarStyle {
    [self.h5ViewContoller setStatusBarStyle:statusBarStyle];
}

-(void)wantsFullScreen:(BOOL)fullScreen
{
    [self.h5ViewContoller wantsFullScreen:fullScreen];
}

#pragma mark -
- (void)startMainApp {
    dispatch_async(dispatch_get_main_queue(), ^(void) {
        [[PDRCore Instance] start];
    });
}

- (void)application:(UIApplication *)application performActionForShortcutItem:(UIApplicationShortcutItem *)shortcutItem
  completionHandler:(void(^)(BOOL succeeded))completionHandler{
    [PDRCore handleSysEvent:PDRCoreSysEventPeekQuickAction withObject:shortcutItem];
    completionHandler(true);
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [PDRCore handleSysEvent:PDRCoreSysEventBecomeActive withObject:nil];

    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [PDRCore handleSysEvent:PDRCoreSysEventResignActive withObject:nil];

    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    [PDRCore handleSysEvent:PDRCoreSysEventEnterBackground withObject:nil];
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    [PDRCore handleSysEvent:PDRCoreSysEventEnterForeGround withObject:nil];
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    [PDRCore destoryEngine];
}

#pragma mark -
#pragma mark URL

- (BOOL)application:(UIApplication *)application
            openURL:(NSURL *)url
  sourceApplication:(NSString *)sourceApplication
         annotation:(id)annotation {
    [self application:application handleOpenURL:url];
    return YES;
}

/*
 * @Summary:程序被第三方调用，传入参数启动
 *
 */
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
    [PDRCore handleSysEvent:PDRCoreSysEventOpenURL withObject:url];
    return YES;
}
- (BOOL)application:(UIApplication *)application openURL:(nonnull NSURL *)url options:(nonnull NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options {
    [PDRCore handleSysEvent:PDRCoreSysEventOpenURLWithOptions withObject:@[url,options]];
    return YES;
}

/*
 * @Summary:通用链接
 */
- (BOOL)application:(UIApplication *)application continueUserActivity:(NSUserActivity *)userActivity restorationHandler:(void(^)(NSArray<id<UIUserActivityRestoring>> * __nullable restorableObjects))restorationHandler {
    [PDRCore handleSysEvent:PDRCoreSysEventContinueUserActivity withObject:userActivity];
    restorationHandler(nil);
    return YES;
}
@end

@implementation UINavigationController(Orient)

-(BOOL)shouldAutorotate{
    return ![PDRCore Instance].lockScreen;
    return YES;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations{
    if([self.topViewController isKindOfClass:[ViewController class]])
        return [self.topViewController supportedInterfaceOrientations];
    return UIInterfaceOrientationMaskPortrait;
}
@end

