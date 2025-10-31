//
//  OctReplaceURLTools.h
//  OctCoreSDK
//
//  Created by guantou on 2019/12/10.
//

#import <Foundation/Foundation.h>
#import "OctAdRecordModel.h"
#import <UIKit/UIKit.h>
@class OctBuyerModel;
NS_ASSUME_NONNULL_BEGIN

@interface OctReplaceURLTools : NSObject

//  配置文件宏替换
+ (NSString *)oct_replaceConfigureWithMacro:(NSString *)replaceString;

//  事件上报宏替换
+ (NSString *)oct_replaceReportWithMacro:(NSString *__nullable)replaceString
                                     model:(OctAdRecordModel *__nullable)adRecordModel
                                   buyerId:(NSString *__nullable)buyerID
                              channelAppID:(NSString *__nullable)appID
                            channelSpaceID:(NSString *__nullable)spaceID
                                 eventCode:(NSString *__nullable)eventCode
                                     Error:(NSError *__nullable)error;

//  曝光上报宏替换
+ (NSString *)oct_replacerReportURLWithUrlString:(NSString *)urlString withDataString:(NSString *)dataString randomUUID:(NSString *)randomUUID;

//  点击上报宏替换
+ (NSString *)oct_replacerReportURLWithUrlString:(NSString *)urlString withDataString:(NSString *)dataString randomUUID:(NSString *)randomUUID adClickPoint:(CGPoint)adClickPoint srClickPoint:(CGPoint)srClickPoint;

+ (NSString *)oct_logReplaceWithMacro:(NSString *__nullable)replaceString
                             adTypeId:(NSString *_Nullable)adTypeId
                        channelSlotId:(NSString *_Nullable)channelSlotId
                                buyer:(OctBuyerModel *_Nullable)buyer
                                reqId:(NSString *)reqId
                                ptime:(NSTimeInterval)ptime
                              isAoclk:(BOOL)isAoclk;

+ (NSString *)oct_logReplaceWithMacro:(NSString *__nullable)replaceString
                             adTypeId:(NSString *_Nullable)adTypeId
                        channelSlotId:(NSString *_Nullable)channelSlotId
                                buyer:(OctBuyerModel *_Nullable)buyer
                                reqId:(NSString *)reqId
                                ptime:(NSTimeInterval)ptime
                                  bid:(BOOL)bid
                              isAoclk:(BOOL)isAoclk;

@end

NS_ASSUME_NONNULL_END
