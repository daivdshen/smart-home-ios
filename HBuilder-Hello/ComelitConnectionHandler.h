//
//  ComelitConnectionHandler.h
//  CGAppTest
//
//  Created by Cornelli Fabio on 17/10/17.
//  Copyright © 2017 Cornelli Fabio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <PushKit/PushKit.h>
#import <CGModule/CGModule.h>

extern NSString * const UserDefaultVoipPushToken;

@interface ComelitConnectionHandler : NSObject

@property(nonatomic,weak) id<CGCallback> callbackReceiver;

@property (nonatomic) BOOL      withUI;

+ (ComelitConnectionHandler*)sharedManager;

-(void)setCallStartReceiver:(id<CMGCallStartReceiver>)delegate;

-(void)setCallEventCallback:(id<CGCallEventCallback>)delegate;

-(void)connectToSystem;

-(void)disconnectFromSystem;

-(void)handlePushNotification:(PKPushPayload *)payload forType:(NSString *)type;

-(id<CGPresenter>)getCurrentPresenter;

-(void)callElementId:(NSString*)elementId;

-(NSArray*)getExternalUnitAddressBook;
-(NSArray*)getInternalUnitAddressBook;
-(NSArray*)getOpendoorAddressBook;
-(NSArray*)getSwitchboardAddressBook;
-(NSArray*)getCameraAddressBook;
-(NSArray*)getRtspCameraAddressBook;
-(NSArray*)getActuatorAddressBook;

@end

