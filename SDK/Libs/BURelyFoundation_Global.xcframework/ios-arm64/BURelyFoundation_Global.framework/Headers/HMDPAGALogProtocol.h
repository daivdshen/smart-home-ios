//
//  HMDPAGALogProtocol.h
//  Pods
//
//  Created by bytedance on 2020/5/28.
//

#ifndef HMDPAGLogProtocol_hpp
#define HMDPAGLogProtocol_hpp

#if !HMDPAG_DISABLE_ALOG && __has_include("BDAlogProtocol/BDAlogProtocol.h")

#include <BDAlogProtocol/BDAlogProtocol.h>

#ifdef __OBJC__
#pragma mark -  OC methods. The usage is the same as NSLog
//自定义level
#define HMDPAGALOG_PROTOCOL(level, format, ...) BDALOG_PROTOCOL(level, format, ##__VA_ARGS__)

//自定义level和tag
#define HMDPAGALOG_PROTOCOL_TAG(level, tag, format, ...) BDALOG_PROTOCOL_TAG(level, tag, format, ##__VA_ARGS__)

//debug log
#define HMDPAGALOG_PROTOCOL_DEBUG(format, ...) BDALOG_PROTOCOL_DEBUG(format, ##__VA_ARGS__)

//info log
#define HMDPAGALOG_PROTOCOL_INFO(format, ...) BDALOG_PROTOCOL_INFO(format, ##__VA_ARGS__)

//warn log
#define HMDPAGALOG_PROTOCOL_WARN(format, ...) BDALOG_PROTOCOL_WARN(format, ##__VA_ARGS__)

//error log
#define HMDPAGALOG_PROTOCOL_ERROR(format, ...) BDALOG_PROTOCOL_ERROR(format, ##__VA_ARGS__)

//fatal log
#define HMDPAGALOG_PROTOCOL_FATAL(format, ...) BDALOG_PROTOCOL_FATAL(format, ##__VA_ARGS__)

/** TAG*/
//debug log
#define HMDPAGALOG_PROTOCOL_DEBUG_TAG(tag, format, ...) BDALOG_PROTOCOL_DEBUG_TAG(tag, format, ##__VA_ARGS__)

//info log
#define HMDPAGALOG_PROTOCOL_INFO_TAG(tag, format, ...) BDALOG_PROTOCOL_INFO_TAG(tag, format, ##__VA_ARGS__)

//warn log
#define HMDPAGALOG_PROTOCOL_WARN_TAG(tag, format, ...) BDALOG_PROTOCOL_WARN_TAG(tag, format, ##__VA_ARGS__)

//error log
#define HMDPAGALOG_PROTOCOL_ERROR_TAG(tag, format, ...) BDALOG_PROTOCOL_ERROR_TAG(tag, format, ##__VA_ARGS__)

//fatal log
#define HMDPAGALOG_PROTOCOL_FATAL_TAG(tag, format, ...) BDALOG_PROTOCOL_FATAL_TAG(tag, format, ##__VA_ARGS__)

#endif

#ifdef __cplusplus
extern "C" {
#endif
#pragma mark - C and C++

#define HMDPAG_ALOG_PROTOCOL_DEBUG(format, ...) ALOG_PROTOCOL_DEBUG(format, ##__VA_ARGS__);
#define HMDPAG_ALOG_PROTOCOL_INFO(format, ...) ALOG_PROTOCOL_INFO(format, ##__VA_ARGS__);
#define HMDPAG_ALOG_PROTOCOL_WARN(format, ...) ALOG_PROTOCOL_WARN(format, ##__VA_ARGS__);
#define HMDPAG_ALOG_PROTOCOL_ERROR(format, ...) ALOG_PROTOCOL_ERROR(format, ##__VA_ARGS__);
#define HMDPAG_ALOG_PROTOCOL_FATAL(format, ...) ALOG_PROTOCOL_FATAL(format, ##__VA_ARGS__);
    
#define HMDPAG_ALOG_PROTOCOL_DEBUG_TAG(tag,format, ...) ALOG_PROTOCOL_DEBUG_TAG(tag,format, ##__VA_ARGS__);
#define HMDPAG_ALOG_PROTOCOL_INFO_TAG(tag,format, ...) ALOG_PROTOCOL_INFO_TAG(tag,format, ##__VA_ARGS__);
#define HMDPAG_ALOG_PROTOCOL_WARN_TAG(tag,format, ...) ALOG_PROTOCOL_WARN_TAG(tag,format, ##__VA_ARGS__);
#define HMDPAG_ALOG_PROTOCOL_ERROR_TAG(tag,format, ...) ALOG_PROTOCOL_ERROR_TAG(tag,format, ##__VA_ARGS__);
#define HMDPAG_ALOG_PROTOCOL_FATAL_TAG(tag,format, ...) ALOG_PROTOCOL_FATAL_TAG(tag,format, ##__VA_ARGS__);

bool hmdPAG_log_enable(void);
    
#ifdef __cplusplus
} // extern "C"
#endif

#else

#ifdef __OBJC__
#pragma mark -  OC methods. The usage is the same as NSLog
//自定义level
#define HMDPAGALOG_PROTOCOL(level, format, ...)

//自定义level和tag
#define HMDPAGALOG_PROTOCOL_TAG(level, tag, format, ...)

//debug log
#define HMDPAGALOG_PROTOCOL_DEBUG(format, ...)

//info log
#define HMDPAGALOG_PROTOCOL_INFO(format, ...)

//warn log
#define HMDPAGALOG_PROTOCOL_WARN(format, ...)

//error log
#define HMDPAGALOG_PROTOCOL_ERROR(format, ...)

//fatal log
#define HMDPAGALOG_PROTOCOL_FATAL(format, ...)

/** TAG*/
//debug log
#define HMDPAGALOG_PROTOCOL_DEBUG_TAG(tag, format, ...)

//info log
#define HMDPAGALOG_PROTOCOL_INFO_TAG(tag, format, ...)

//warn log
#define HMDPAGALOG_PROTOCOL_WARN_TAG(tag, format, ...)

//error log
#define HMDPAGALOG_PROTOCOL_ERROR_TAG(tag, format, ...)

//fatal log
#define HMDPAGALOG_PROTOCOL_FATAL_TAG(tag, format, ...)

#endif

#ifdef __cplusplus
extern "C" {
#endif
#pragma mark - C and C++

#define HMDPAG_ALOG_PROTOCOL_DEBUG(format, ...)
#define HMDPAG_ALOG_PROTOCOL_INFO(format, ...)
#define HMDPAG_ALOG_PROTOCOL_WARN(format, ...)
#define HMDPAG_ALOG_PROTOCOL_ERROR(format, ...)
#define HMDPAG_ALOG_PROTOCOL_FATAL(format, ...)
    
#define HMDPAG_ALOG_PROTOCOL_DEBUG_TAG(tag,format, ...)
#define HMDPAG_ALOG_PROTOCOL_INFO_TAG(tag,format, ...)
#define HMDPAG_ALOG_PROTOCOL_WARN_TAG(tag,format, ...)
#define HMDPAG_ALOG_PROTOCOL_ERROR_TAG(tag,format, ...)
#define HMDPAG_ALOG_PROTOCOL_FATAL_TAG(tag,format, ...)

bool hmdPAG_log_enable(void);
    
#ifdef __cplusplus
} // extern "C"
#endif

#endif

#endif /* HMDPAGLogProtocol_hpp */

