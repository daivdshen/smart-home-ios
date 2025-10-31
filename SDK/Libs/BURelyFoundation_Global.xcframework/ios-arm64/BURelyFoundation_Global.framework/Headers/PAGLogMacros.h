//
//  BULogMacros.h
//  BUFoundation
//
//  Created by Rush.D.Xzj on 2021/2/2.
//

#import <Foundation/Foundation.h>
#import "PAGLogManager.h"
#import "PAGCommonMacros.h"



/** LOG **/
#define PAG_Log_Foundation(frmt, ...) PAG_LOG_MAYBE(PAGFoundationLog, PAG_LOG_ENABLED, frmt, ##__VA_ARGS__)

#define PAG_LOG_MAYBE(BULogTypeString, flg, frmt, ...)                       \
do {                                                      \
if(flg) [PAGLogManager debugLogWithFormat:@"[BUAdSDK-%@]-[debug]-[%@]%@", PAGSDKRelyVersion, BULogTypeString, [NSString stringWithFormat:frmt,##__VA_ARGS__]];                       \
} while(0)

///外部媒体使用
#define PAG_LogServerD(BULogType, frmt, ...) PAG_LogBaseD(server, BULogType, frmt, ##__VA_ARGS__)
#define PAG_LogExternalD(BULogType, frmt, ...) PAG_LogBaseD(external, BULogType, frmt, ##__VA_ARGS__)

///内部使用
#define PAG_LogErrorD(BULogType, frmt, ...) PAG_LogBaseD(error, BULogType, frmt, ##__VA_ARGS__)
#define PAG_LogWaringD(BULogType, frmt, ...) PAG_LogBaseD(warning, BULogType, frmt, ##__VA_ARGS__)
#define PAG_LogInfoD(BULogType, frmt, ...) PAG_LogBaseD(info, BULogType, frmt, ##__VA_ARGS__)
#define PAG_LogDebugD(BULogType, frmt, ...) PAG_LogBaseD(debug, BULogType, frmt, ##__VA_ARGS__)
#define PAG_LogVerboseD(BULogType, frmt, ...) PAG_LogBaseD(verbose, BULogType, frmt, ##__VA_ARGS__)
#define PAG_LogInternalD(BULogType, frmt, ...) PAG_LogBaseD(internal, BULogType, frmt, ##__VA_ARGS__)
#define PAG_LogBaseD(level, BULogType, frmt, ...) [PAGLogManager level##LogWithFormat:@"[BUAdSDK-%@]-[%s]-[%@]%@", PAGSDKRelyVersion, #level, BULogType, [NSString stringWithFormat:frmt, ##__VA_ARGS__]];

#define PAG_LogDevInfo(BULogType, frmt, ...) PAG_LogDevBase(BULogType, @"🟠", frmt, ##__VA_ARGS__)
#define PAG_LogDevSucc(BULogType, frmt, ...) PAG_LogDevBase(BULogType, @"🟢", frmt, ##__VA_ARGS__)
#define PAG_LogDevFail(BULogType, frmt, ...) PAG_LogDevBase(BULogType, @"🔴", frmt, ##__VA_ARGS__)
#define PAG_LogDevBase(BULogType, mark, frmt, ...) [PAGLogManager internalLogWithFormat:@"[BUAdSDK-%@]-[%@ %@]-[%@ %d] %@", PAGSDKRelyVersion, BULogType, mark, [NSURL fileURLWithPath:@__FILE__].lastPathComponent, __LINE__, [NSString stringWithFormat:frmt, ##__VA_ARGS__]];

FOUNDATION_EXPORT NSString * const PAGFoundationLog;
FOUNDATION_EXPORT BOOL PAG_LOG_ENABLED;


