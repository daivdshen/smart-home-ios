//
//  CGModule.h
//  CGModule
//
//  Created by Cornelli Fabio on 28/10/17.
//  Copyright © 2017 Cornelli Fabio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <PushKit/PushKit.h>
#import "AddressBookElement.h"
#import "CGResponse.h"
#import "CMGError.h"
#import "CMGCallDirection.h"
#import "CGVideoReceiver.h"
#import "CGPresenter.h"
#import "CGCallback.h"
#import "CGCallEventCallback.h"
#import "CMGParameter.h"
#import "CMGCallStartReceiver.h"

#import <PushKit/PushKit.h>

@interface CGModule : NSObject

@property (nonatomic, weak) id<CGCallback> connDelegate;
@property (nonatomic, weak) id<CMGCallStartReceiver> callStartDelegate;
@property (nonatomic, weak) id<CGCallEventCallback> callEventDelegate;

@property (nonatomic) BOOL withUI;
@property (nonatomic) BOOL softwareDecode;
@property (nonatomic) BOOL isDebug;


@property (nonatomic) BOOL enableCallKit;
@property (nonatomic, strong) NSString *callKitTitle;
@property (nonatomic, strong) NSString *callKitIncomingCallDescription;
@property (nonatomic, strong) NSString *callKitIconName;
@property (nonatomic) NSTimeInterval callKitConnectionTimeout;

+ (CGModule*)sharedManager;

-(void)connect:(CMGParameter*) params;

-(void)disconnect;

-(void)handlePushNotification:(PKPushPayload *)payload forType:(NSString *)type;

-(void)handlePushNotificationWithoutUI:(PKPushPayload *)payload forType:(NSString *)type softwareDecode:(BOOL)softwareDecode;

-(NSArray*)getOpendoorAddressBook;

-(NSArray*)getActuatorAddressBook;

-(NSArray*)getExternalUnitAddressBook;

-(NSArray*)getInternalUnitAddressBook;

-(NSArray*)getSwitchboardAddressBook;

-(NSArray*)getCameraAddressBook;

-(NSArray*)getRtspCameraAddressBook;

-(CGResponse)call:(NSString*)elementId;

-(id<CGPresenter>)getCurrentPresenter;

@end

@protocol CGConnectionDelegate <NSObject>

- (void) onConnectionChanged:(NSString*)status;
- (void) onConnectRequestCompleted;
- (void) onDestroyRequestCompleted;

@end

