//
//  OctHTTPRequestType.h
//  Pods
//
//  Created by guantou on 2019/12/9.
//

#ifndef OctHTTPRequestType_h
#define OctHTTPRequestType_h

NS_ASSUME_NONNULL_BEGIN

/* 定义请求类型的枚举 */
typedef NS_ENUM(NSUInteger, OctHttpRequestType) {
    /* get请求 */
    OctHttpRequestTypeGet                = 0,
    /* post请求 */
    OctHttpRequestTypePost,
    /* put请求 */
    OctHttpRequestTypePut,
    /* delete请求 */
    OctHttpRequestTypeDelete,
    /* 上传图片 */
    OctHttpRequestTypeUploadImage,
    /* 上传视频 */
    OctHttpRequestTypeUploadVideo,
    /* 上传文件 */
    OctHttpRequestTypeUploadFile,
    /* 下载图片 */
    OctHttpRequestTypeDownLoadFile
};

typedef NS_ENUM(NSUInteger, OctHttpRequestContentType) {
    /** 心跳*/
    OctHttpRequestContentTypeHearbeat    = 0,
    /** 广告*/
    OctHttpRequestContentTypeAdvertising    ,
    /** 日志*/
    OctHttpRequestContentTypeLog
};

typedef NSURLSessionTask OctURLSessionDataTask;

/* 定义请求成功的 block */
typedef void( ^ OctResponseSuccess)(OctURLSessionDataTask *dataTask, id response);
/* 定义请求失败的 block */
typedef void( ^ OctResponseFail)(OctURLSessionDataTask *dataTask, NSError *error);
/* 只返回结果请求成功的 block */
typedef void( ^ OctResponseSuccessWithModel)(id response);
/* 只返回结果请求失败的 block */
typedef void( ^ OctResponseFailWithError)(NSError *error);
/* 定义上传进度 block */
typedef void( ^ OctUploadProgress)(int64_t bytesProgress, int64_t totalBytesProgress);
/* 定义下载进度 block */
typedef void( ^ OctDownloadProgress)(int64_t bytesProgress, int64_t totalBytesProgress);

NS_ASSUME_NONNULL_END

#endif /* OctHTTPRequestType_h */
