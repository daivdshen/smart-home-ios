#import <AVFoundation/AVFoundation.h>
//
//  ComelitConnectionHandler.m
//  CGAppTest
//
//  Created by Cornelli Fabio on 17/10/17.
//  Copyright © 2017 Cornelli Fabio. All rights reserved.
//

#import "ComelitConnectionHandler.h"
#import <CGModule/AddressBookElement.h>
#import "AppDelegate.h"

/*
 * Simple Wrapper of CGModule api
 */
@interface ComelitConnectionHandler()<CGCallback,CMGCallStartReceiver,CGCallEventCallback>

@property (nonatomic) CGModule* comelitHandler;
@property (nonatomic) BOOL      softwareDecode;

@end
NSString * const IP_ADDR_SETTINGS_KEY = @"ipAddress";
NSString * const PORT_SETTINGS_KEY = @"port";
NSString * const ACT_CODE_SETTINGS_KEY = @"actCode";

NSString * const UserDefaultPushToken = @"push_token";
NSString * const UserDefaultVoipPushToken = @"voip_push_token";

@implementation ComelitConnectionHandler

+ (ComelitConnectionHandler*)sharedManager
{
    static ComelitConnectionHandler *sharedManager = nil;
    static dispatch_once_t onceToken;
    
    dispatch_once(&onceToken, ^{
        sharedManager = [[self alloc] init];
    });
    
    return sharedManager;
}

-(id)init
{
    if (self = [super init])
    {
        _comelitHandler = [CGModule sharedManager];
        _softwareDecode = YES;
        _withUI = YES;

        //callkit configuration
        _comelitHandler.enableCallKit = YES; //indicates if callkit should be used or not for notify incoming calls (defaults to YES)
        _comelitHandler.callKitTitle = @"Comelit App Test";
        _comelitHandler.callKitIncomingCallDescription = @"Intercom incoming call";
        _comelitHandler.callKitIconName = @"icon-refresh";
        _comelitHandler.callKitConnectionTimeout = 10.0;
    }
    return self;
}

- (void)setCallbackReceiver:(id<CGCallback>)callbackReceiver
{
    _callbackReceiver = callbackReceiver;
    _comelitHandler.connDelegate = callbackReceiver;
}

-(void)setCallStartReceiver:(id<CMGCallStartReceiver>)delegate{
    _comelitHandler.callStartDelegate = delegate;
}

-(void)setCallEventCallback:(id<CGCallEventCallback>)delegate{
    _comelitHandler.callEventDelegate = delegate;
}

-(void)connectToSystem
{
    [[ComelitConnectionHandler sharedManager] setCallbackReceiver:self];
    NSLog(@"VCP  connectToSystem.....");
    //READ PARAMETERS FROM USER DEFAULTS
//    NSString *ip = @"perfect10.tplinkdns.com";
//    NSString *port = @"64100";
//    NSString *actCode = @"c49il8";
//    NSString *token = @"";
    
    NSString *ip = @"192.168.2.200";
    NSString *port = @"64100";
    NSString *actCode = @"0wshqh";
    NSString *token = @"";
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    ip = [defaults objectForKey:IP_ADDR_SETTINGS_KEY];
    port = [defaults objectForKey:PORT_SETTINGS_KEY];
    actCode = [defaults objectForKey:ACT_CODE_SETTINGS_KEY];
    
    if ([CGModule sharedManager].enableCallKit)
    {
        //use voip push token when callkit is enabled
        token = [defaults objectForKey:UserDefaultVoipPushToken];
    }
    else
    {
        //use push token when callkit is disabled
        token = [defaults objectForKey:UserDefaultPushToken];
    }
    
    NSLog(@"VCP Param host:%@ port:%@ actCode:%@ Token.....%@",ip,port,actCode,token);
    
    //connect with new API, you can configure only a subset of the builder parameters
    [_comelitHandler connect:[CMGParameter makeWithBuilder:^(CMGParameterBuilder* builder){
        //======= these are the main parameters to connect to a comelit device =====
        builder.hostname = ip; //default value, if not passed -> @""
        builder.port = [port intValue]; //default value, if not passed -> 64100
        builder.activationCode = actCode; //default value, if not passed -> @""
        builder.pushToken = token; //default value, if not passed -> @""
        builder.callback = self.callbackReceiver; //default value, if not passed -> nil
        NSLog(@"VCP hello world!3333.....");
#ifdef DEBUG
        builder.debug = YES; //default value, if not passed -> NO
#endif
        //======================================
        builder.softwareDecode = self.softwareDecode; //indicates if we want to decode the video with a software decoder or with the device hardwared decoder. Default value, if not passed -> NO
        builder.withUI = YES; //indicates if we want to use the comelit SDK internal UI for the call. Default value, if not passed -> YES
        builder.alertTime = 40; //alerting timeout in seconds, default value, if not passed -> 60 seconds
        builder.closeTime = 15; //call close time in seconds after an opendoor command. Default value, if not passed -> 5 seconds
        builder.connectionTimeExtUnit = 600; //connection timeout in seconds for external unit. Default value, if not passed -> 90 seconds
        builder.connectionTimeIntUnit = 300; //connection timeout in seconds for internal unit. Default value, if not passed -> 60 seconds
    }]];
}

-(void)handlePushNotification:(PKPushPayload *)payload forType:(NSString *)type
{
    NSLog(@"VCP handlePushNotification");
    if (TRUE)
    {
        [_comelitHandler handlePushNotification:payload forType:type];
    }
    else
    {
        [_comelitHandler handlePushNotificationWithoutUI:payload forType:type softwareDecode:self.softwareDecode];
    }
}

-(void)disconnectFromSystem
{
//    [_comelitHandler disconnect]; //disconnect from the current system
}

-(NSArray*)getExternalUnitAddressBook
{
    return [_comelitHandler getExternalUnitAddressBook];
}

-(NSArray*)getInternalUnitAddressBook
{
    return [_comelitHandler getInternalUnitAddressBook];
}

-(NSArray*)getOpendoorAddressBook
{
    return [_comelitHandler getOpendoorAddressBook];
}

-(NSArray*)getSwitchboardAddressBook
{
    return [_comelitHandler getSwitchboardAddressBook];
}

-(NSArray*)getCameraAddressBook
{
    return [_comelitHandler getCameraAddressBook];
}

-(NSArray*)getRtspCameraAddressBook
{
    return [_comelitHandler getRtspCameraAddressBook];
}

-(NSArray*)getActuatorAddressBook
{
    return  [_comelitHandler getActuatorAddressBook];
}

-(void)callElementId:(NSString*)elementId
{
    [_comelitHandler call:elementId];
}

-(id<CGPresenter>)getCurrentPresenter
{
   return [_comelitHandler getCurrentPresenter];
}


-(void)onConnect
{
    NSLog(@"VCP CONNECTED");
}

-(void)onDisconnect
{
    NSLog(@"VCP DISCONNECTED");
}

// 发送 JSON 格式的 POST 请求
- (void)sendJsonPostRequest {
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString *serverHost = [NSString stringWithFormat:@"%@%@", [defaults objectForKey:@"serverHost"], @"/home-service/vcp/reGeneratorCode"];
//    NSString *serverHost = @"http://192.168.251.60:8062/vcp/reGeneratorCode";
    NSLog(@"VCP Host URL %@",serverHost);
    // 1. 接口 URL
    NSURL *url = [NSURL URLWithString:serverHost];
    if (!url) {
        NSLog(@"无效的 URL");
        return;
    }
    
    // 2. 准备请求参数（字典形式）
    NSDictionary *params = @{
        @"unitId": [defaults objectForKey:@"unitId"],
        @"ownerId": [defaults objectForKey:@"userId"]
    };
    
    NSLog(@"VCP Request NSDictionary %@",params);
    // 3. 将字典转换为 JSON 数据
    NSError *jsonError;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:params
                                                       options:0
                                                         error:&jsonError];
    if (jsonError) {
        NSLog(@"JSON 序列化失败: %@", jsonError.localizedDescription);
        return;
    }
    
    NSLog(@"VCP Request param %@",jsonData);
    
    // 4. 创建请求对象
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    request.HTTPMethod = @"POST"; // 设置为 POST 方法
    request.HTTPBody = jsonData;  // 设置 JSON 数据为请求体
    request.timeoutInterval = 15; // 超时时间 15 秒
    
    // 5. 设置请求头（关键：指定 Content-Type 为 application/json）
    [request setValue:@"application/json; charset=utf-8" forHTTPHeaderField:@"Content-Type"];
    
    // 6. 创建 URLSession 并发送请求
    NSURLSession *session = [NSURLSession sharedSession];
    NSURLSessionDataTask *task = [session dataTaskWithRequest:request
                                           completionHandler:^(NSData * _Nullable data,
                                                               NSURLResponse * _Nullable response,
                                                               NSError * _Nullable error) {
        // 7. 处理响应（回调在子线程）
        if (error) {
            NSLog(@"请求错误: %@", error.localizedDescription);
            return;
        }
        
        // 解析 HTTP 响应状态码
        NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)response;
        NSLog(@"状态码: %ld", (long)httpResponse.statusCode);
        
        // 处理返回的 JSON 数据
        if (data) {
            NSError *responseError;
            NSDictionary *responseDict = [NSJSONSerialization JSONObjectWithData:data
                                                                        options:0
                                                                        error:&responseError];
            NSLog(@"VCP 返回 JSON: %@", responseDict);
            if (responseError) {
                // 如果不是 JSON 格式，尝试转换为字符串
                NSString *responseString = [[NSString alloc] initWithData:data
                                                                 encoding:NSUTF8StringEncoding];
                NSLog(@"返回内容: %@", responseString);
            } else {
                NSString * actCode=[responseDict objectForKey:@"data"];
                if (actCode !=nil){
                    [defaults setObject:actCode forKey:ACT_CODE_SETTINGS_KEY];
                    [self connectToSystem];
                }
         
                
            }
        }
        
    }];
    
    // 启动任务
    [task resume];
}


-(void)onError:(CMGError)error
{
    NSLog(@"VCP ERROR %ld",error);
    if (error == CG_ACTIVATION_CODE_ERROR){
        NSLog(@"VCP Repeat Connecting.... CG_ACTIVATION_CODE_ERROR %ld",error);
        [self sendJsonPostRequest];
    }  else if (error == CG_CONNECTION_ERROR){
        NSLog(@"VCP ERROR CG_CONNECTION_ERROR  %ld",error);
    }
//    dispatch_async(dispatch_get_main_queue(), ^{f
//        if (error == CG_CONNECTION_ERROR)
//            self.connStatusLb.text = @"CONNECTION ERROR!";
//        else if (error == CG_ACTIVATION_CODE_ERROR)
//            self.connStatusLb.text = @"INVALID ACTIVATION CODE";
//        else if (error == CG_GENERIC_ERROR)
//            self.connStatusLb.text = @"GENERIC ERROR";
//
//        if ([UIApplication sharedApplication].applicationState == UIApplicationStateBackground) {
//            [[ComelitConnectionHandler sharedManager] disconnectFromSystem];
//        }
//    });
}

#pragma mark - CGCallEventCallback

- (void)onCallAnswer
{
    NSLog(@"VCP onCallAnswer");
}

- (void)onCallCapabilityAudio
{
    NSLog(@"VCP onCallCapabilityAudio");
}

- (void)onCallCapabilityDoor
{
    NSLog(@"VCP onCallCapabilityDoor");
}

- (void)onCallCapabilityVideo
{
    NSLog(@"VCP onCallCapabilityVideo");
}

- (void)onCallEnd
{
    NSLog(@"VCP onCallEnd");
}

- (void)onCallOpenDoor
{
    NSLog(@"VCP onCallOpenDoor");
}

- (void)onCallReleaseDeviceNotFound
{
    NSLog(@"VCP onCallReleaseDeviceNotFound");
}

- (void)onCallReleaseDiverted
{
    NSLog(@"VCP onCallReleaseDiverted");
}

- (void)onCallReleaseError
{
    NSLog(@"VCP onCallReleaseError");
}

- (void)onCallReleaseNormal
{
    NSLog(@"VCP onCallReleaseNormal");
}

- (void)onCallReleaseNotSelected
{
    NSLog(@"VCP onCallReleaseNotSelected");
}

- (void)onCallReleaseRejected
{
    NSLog(@"VCP onCallReleaseRejected");
}

- (void)onCallReleaseSystemBusy
{
    NSLog(@"VCP onCallReleaseSystemBusy");
}

- (void)onCallReleaseTimeout
{
    NSLog(@"VCP onCallReleaseTimeout");
}

- (void)onCallRequestVideo
{
    NSLog(@"VCP onCallRequestVideo");
}

- (void)onCallToggleMic
{
    NSLog(@"VCP onCallToggleMic");
}

-(void)onCallStartRec
{
    NSLog(@"VCP onCallStartRec");
}

-(void)onCallStopRec:(NSString*)path
{
    NSLog(@"VCP onCallStopRec");
}

- (void)onCallVirtualButton1
{
    NSLog(@"VCP onCallVirtualButton1");
}

- (void)onCallVirtualButton2
{
    NSLog(@"VCP onCallVirtualButton2");
}

- (void)onCallRelease:(CMGCallReleaseType)cause
{
    switch (cause) {
        case CMG_CALL_RELEASE_BY_USER:
        case CMG_CALL_RELEASE_NORMAL:
            [self onCallReleaseNormal];
            break;
        case CMG_CALL_RELEASE_TIMEOUT:
            [self onCallReleaseTimeout];
            break;
        case CMG_CALL_RELEASE_ERROR:
            [self onCallReleaseError];
            break;
        case CMG_CALL_RELEASE_NOT_SELECTED:
            [self onCallReleaseNotSelected];
            break;
        case CMG_CALL_RELEASE_SYSTEM_BUSY:
            [self onCallReleaseSystemBusy];
            break;
        case CMG_CALL_RELEASE_DIVERTED:
            [self onCallReleaseDiverted];
            break;
        case CMG_CALL_RELEASE_REJECTED:
            [self onCallReleaseRejected];
            break;
        case CMG_CALL_RELEASE_DEVICE_NOT_FOUND:
            [self onCallReleaseDeviceNotFound];
            break;
    }
}


- (void)onCallStartWithName:(NSString *)name andDirection:(CMGCallDirection)direction
{
    NSLog(@"VCP onCallStart");
}
@end

