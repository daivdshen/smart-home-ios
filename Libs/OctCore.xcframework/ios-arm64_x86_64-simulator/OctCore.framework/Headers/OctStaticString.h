//
//  OctTimeTools.h
//  OctCoreSDK
//
//  Created by guantou on 2019/12/10.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

/**
 开启外部错误log
 */
UIKIT_EXTERN BOOL Oct_OpenTheLog;

/**
 应用内部调试log
 */
//  是否开启请求时长log
UIKIT_EXTERN BOOL Oct_RequestTimeLog;
//  是否开启生命周期log
UIKIT_EXTERN BOOL Oct_OpenCycleLog;
//  是否开启广告请求log
UIKIT_EXTERN BOOL Oct_OpenAdRequestLog;
//  是否开启广告返回log
UIKIT_EXTERN BOOL Oct_OpenAdResponseLog;
//  是否开启广告返回转化数据log
UIKIT_EXTERN BOOL Oct_OpenAdDisposeResponseLog;
//  是否开启心跳请求log
UIKIT_EXTERN BOOL Oct_OpenHeartbeatRequestLog;
//  是否开启心跳返回log
UIKIT_EXTERN BOOL Oct_OpenHeartbeatResponseLog;
//  是否开启Log请求log
UIKIT_EXTERN BOOL Oct_OpenLogRequestLog;
//  是否开启Log返回log
UIKIT_EXTERN BOOL Oct_OpenLogResponseLog;
//  是否开启内部其他log
UIKIT_EXTERN BOOL Oct_OpenOtherLog;
//  是否开启内部错误log
UIKIT_EXTERN BOOL Oct_OpenErrorLog;
//  是否开启渠道上报log
UIKIT_EXTERN BOOL Oct_OpenChannelReportLog;
//  是否开启Oct上报log
UIKIT_EXTERN BOOL Oct_OpenOctReportLog;

/**
 渠道是否初始化
 */
//UIKIT_EXTERN BOOL kChannelFalconInitConfiguration;
UIKIT_EXTERN BOOL kOctChannelBUAdInitConfiguration;

/**
 渠道名称
 */
static NSString *const kChannelOct = @"octopus";
//static NSString *const kChannelFalcon = @"lieying";

/**
 网络超时时长
 */
static CGFloat const kOctAdRequestTime = 5.0f;
static CGFloat const kOctReportRequestTime = 15.0f;

static CGFloat const kOctLogRequestTime = 1800;

static NSString *const kOctLogRequestLastTime = @"OctLogRequestLastTime";

/**
 静态
 */
static NSString *const kOctDefaultConfigure = @"OctDefaultConfigure";
static NSString *const kOctLandingPageBackDone = @"OctLandingPageBackDone";
static NSString *const kOctCrashDict = @"kOctCrashDict";

//  处理JS使用的静态字段
static NSString *const kOct_JSBackGotoPage = @"oct://GotoPage";
static NSString *const kOct_JSBackCallNo = @"oct://CallNo";
static NSString *const kOct_JSBackClosePage = @"oct://ClosePage";
static NSString *const kOct_JSBackDownload = @"oct://Download";
static NSString *const kOct_JSBackDeepLink = @"oct://DeepLink";

static NSString * _Nonnull const OctLoaderInters = @"OctAdInterstitialAd";
static NSString * _Nonnull const OctLoaderDraw = @"OctCsjDrawAd";
static NSString * _Nonnull const OctLoaderDrawCustom = @"OctCsjDrawCustomAd";
static NSString * _Nonnull const OctLoaderVideo = @"OctAdVideoAd";
static NSString * _Nonnull const OctLoaderSplash = @"OctSplashAd";
static NSString * _Nonnull const OctLoaderFsVideo = @"OctCsjFullScreenAd";
static NSString * _Nonnull const OctLoaderBanner = @"OctAdBannerAd";
static NSString * _Nonnull const OctLoaderCustomFlow = @"OctAdFlowCustomAd";
static NSString * _Nonnull const OctLoaderFlow = @"OctAdFlowAd";

static NSString * _Nonnull const OctLoaderCsjInters = @"OctCsjInterstitialAd";
static NSString * _Nonnull const OctLoaderCsjDraw = @"OctCsjDrawAd";
static NSString * _Nonnull const OctLoaderCsjDrawCustom = @"OctCsjDrawCustomAd";
static NSString * _Nonnull const OctLoaderCsjVideo = @"OctCsjVideoAd";
static NSString * _Nonnull const OctLoaderCsjSplash = @"OctCsjSplashAd";
static NSString * _Nonnull const OctLoaderCsjFsVideo = @"OctCsjFullScreenAd";
static NSString * _Nonnull const OctLoaderCsjBanner = @"OctCsjBannerAd";
static NSString * _Nonnull const OctLoaderCsjCustomFlow = @"OctCsjFlowCustomAd";
static NSString * _Nonnull const OctLoaderCsjFlow = @"OctCsjFlowAd";

static NSString * _Nonnull const OctLoaderGdtInters = @"OctGdtInterstitialAd";
static NSString * _Nonnull const OctLoaderGdtVideo = @"OctGdtVideoAd";
static NSString * _Nonnull const OctLoaderGdtSplash = @"OctGdtSplashAd";
static NSString * _Nonnull const OctLoaderGdtBanner = @"OctGdtBannerAd";
static NSString * _Nonnull const OctLoaderGdtCustomFlow = @"OctGdtFlowCustomAd";
static NSString * _Nonnull const OctLoaderGdtFlow = @"OctGdtFlowAd";

static NSString * _Nonnull const OctLoaderKsInters = @"OctKsInterstitialAd";
static NSString * _Nonnull const OctLoaderKsDraw = @"OctKsDrawAd";
static NSString * _Nonnull const OctLoaderKsVideo = @"OctKsVideoAd";
static NSString * _Nonnull const OctLoaderKsSplash = @"OctKSSplashAd";
static NSString * _Nonnull const OctLoaderKsFsVideo = @"OctKsFullScreenAd";
static NSString * _Nonnull const OctLoaderKsCustomFlow = @"OctKsFlowCustomAd";
static NSString * _Nonnull const OctLoaderKsFlow = @"OctKsFlowAd";

static NSString * _Nonnull const OctLoaderBdInters = @"OctBdInterstitialAd";
static NSString * _Nonnull const OctLoaderBdExpInters = @"OctBdExpInterstitialAd";
static NSString * _Nonnull const OctLoaderBdDraw = @"OctBdDrawVideoAd";
static NSString * _Nonnull const OctLoaderBdVideo = @"OctBdRewardVideoAd";
static NSString * _Nonnull const OctLoaderBdSplash = @"OctBdSplashAd";
static NSString * _Nonnull const OctLoaderBdFsVideo = @"OctBdFullScreenAd";
static NSString * _Nonnull const OctLoaderBdBanner = @"OctBdBannerAd";
static NSString * _Nonnull const OctLoaderBdCustomFlow = @"OctBdCustomFlowAd";
static NSString * _Nonnull const OctLoaderBdFlow = @"OctBdFlowAd";

static NSString * _Nonnull const OctLoaderLyInters = @"OctLyInterstitialAd";
static NSString * _Nonnull const OctLoaderLyVideo = @"OctLyVideoAd";
static NSString * _Nonnull const OctLoaderLySplash = @"OctLySplashAd";
static NSString * _Nonnull const OctLoaderLyBanner = @"OctLyBannerAd";
static NSString * _Nonnull const OctLoaderLyCustomFlow = @"OctLyFlowCustomAd";
static NSString * _Nonnull const OctLoaderLyFlow = @"OctLyFlowAd";

static NSString * _Nonnull const APPCsjAdAdapter = @"OctCsjAdAdapter";
static NSString * _Nonnull const APPGdtAdAdapter = @"OctGdtAdAdapter";
static NSString * _Nonnull const APPKsAdAdapter = @"OctKsAdapter";
static NSString * _Nonnull const APPBdAdAdapter = @"OctBdAdAdapter";
static NSString * _Nonnull const APPLyAdAdapter = @"OctLyAdAdapter";



static dispatch_queue_t oct_queue_static_configure() {
    static dispatch_queue_t oct_queue_configure;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        oct_queue_configure = dispatch_queue_create("com.lvjunxue.octQueue.configure", DISPATCH_QUEUE_SERIAL);
    });
    return oct_queue_configure;
}

static dispatch_queue_t oct_queue_static_adRequest() {
    static dispatch_queue_t oct_queue_adRequest;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        oct_queue_adRequest = dispatch_queue_create("com.lvjunxue.octQueue.adRequest", DISPATCH_QUEUE_CONCURRENT);
    });
    return oct_queue_adRequest;
}

static dispatch_queue_t oct_queue_static_adReport() {
    static dispatch_queue_t oct_queue_adReport;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        oct_queue_adReport = dispatch_queue_create("com.lvjunxue.octQueue.adReport", DISPATCH_QUEUE_SERIAL);
    });
    return oct_queue_adReport;
}

static dispatch_queue_t oct_queue_static_offLineReport() {
    static dispatch_queue_t oct_queue_offLineReport;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        oct_queue_offLineReport = dispatch_queue_create("com.lvjunxue.octQueue.offLineReport", DISPATCH_QUEUE_SERIAL);
    });
    return oct_queue_offLineReport;
}

static dispatch_queue_t oct_queue_group_request() {
    static dispatch_queue_t oct_queue_groupRequest;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        oct_queue_groupRequest = dispatch_queue_create("com.lvjunxue.octQueue.groupRequest", DISPATCH_QUEUE_CONCURRENT);
    });
    return oct_queue_groupRequest;
}

static dispatch_queue_t oct_queue_catch_manager() {
    static dispatch_queue_t oct_queue_catchManager;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        oct_queue_catchManager = dispatch_queue_create("com.lvjunxue.octQueue.catchManager", DISPATCH_QUEUE_SERIAL);
    });
    return oct_queue_catchManager;
}

static dispatch_queue_t oct_queue_normal_request() {
    static dispatch_queue_t oct_queue_normalRequest;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        oct_queue_normalRequest = dispatch_queue_create("com.lvjunxue.octQueue.normalRequest", DISPATCH_QUEUE_CONCURRENT);
    });
    return oct_queue_normalRequest;
}
