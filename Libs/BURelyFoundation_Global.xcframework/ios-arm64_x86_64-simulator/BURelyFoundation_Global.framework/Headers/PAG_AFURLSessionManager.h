// PAG_AFURLSessionManager.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


#import <Foundation/Foundation.h>
#import "PAG_AFURLResponseSerialization.h"
#import "PAG_AFURLRequestSerialization.h"
#import "PAG_AFSecurityPolicy.h"

/**
 `BU_AFURLSessionManager` creates and manages an `NSURLSession` object based on a specified `NSURLSessionConfiguration` object, which conforms to `<NSURLSessionTaskDelegate>`, `<NSURLSessionDataDelegate>`, `<NSURLSessionDownloadDelegate>`, and `<NSURLSessionDelegate>`.

 ## Subclassing Notes

 This is the base class for `BU_AFHTTPSessionManager`, which adds functionality specific to making HTTP requests. If you are looking to extend `BU_AFURLSessionManager` specifically for HTTP, consider subclassing `BU_AFHTTPSessionManager` instead.

 ## NSURLSession & NSURLSessionTask Delegate Methods

 `BU_AFURLSessionManager` implements the following delegate methods:

 ### `NSURLSessionDelegate`

 - `URLSession:didBecomeInvalidWithError:`
 - `URLSession:didReceiveChallenge:completionHandler:`
 - `URLSessionDidFinishEventsForBackgroundURLSession:`

 ### `NSURLSessionTaskDelegate`

 - `URLSession:willPerformHTTPRedirection:newRequest:completionHandler:`
 - `URLSession:task:didReceiveChallenge:completionHandler:`
 - `URLSession:task:didSendBodyData:totalBytesSent:totalBytesExpectedToSend:`
 - `URLSession:task:needNewBodyStream:`
 - `URLSession:task:didCompleteWithError:`

 ### `NSURLSessionDataDelegate`

 - `URLSession:dataTask:didReceiveResponse:completionHandler:`
 - `URLSession:dataTask:didBecomeDownloadTask:`
 - `URLSession:dataTask:didReceiveData:`
 - `URLSession:dataTask:willCacheResponse:completionHandler:`

 ### `NSURLSessionDownloadDelegate`

 - `URLSession:downloadTask:didFinishDownloadingToURL:`
 - `URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesWritten:totalBytesExpectedToWrite:`
 - `URLSession:downloadTask:didResumeAtOffset:expectedTotalBytes:`

 If any of these methods are overridden in a subclass, they _must_ call the `super` implementation first.

 ## Network Reachability Monitoring

 Network reachability status and change monitoring is available through the `reachabilityManager` property. Applications may choose to monitor network reachability conditions in order to prevent or suspend any outbound requests. See `AFNetworkReachabilityManager` for more details.

 ## NSCoding Caveats

 - Encoded managers do not include any block properties. Be sure to set delegate callback blocks when using `-initWithCoder:` or `NSKeyedUnarchiver`.

 ## NSCopying Caveats

 - `-copy` and `-copyWithZone:` return a new manager with a new `NSURLSession` created from the configuration of the original.
 - Operation copies do not include any delegate callback blocks, as they often strongly captures a reference to `self`, which would otherwise have the unintuitive side-effect of pointing to the _original_ session manager when copied.

 @warning Managers for background sessions must be owned for the duration of their use. This can be accomplished by creating an application-wide or shared singleton instance.
 */

NS_ASSUME_NONNULL_BEGIN

@interface PAG_AFURLSessionManager : NSObject <NSURLSessionDelegate, NSURLSessionTaskDelegate, NSURLSessionDataDelegate, NSURLSessionDownloadDelegate, NSSecureCoding, NSCopying>

/**
 The managed session.
 */
@property (readonly, nonatomic, strong) NSURLSession *session;

/**
 The operation queue on which delegate callbacks are run.
 */
@property (readonly, nonatomic, strong) NSOperationQueue *operationQueue;

/**
 Responses sent from the server in data tasks created with `dataTaskWithRequest:success:failure:` and run using the `GET` / `POST` / et al. convenience methods are automatically validated and serialized by the response serializer. By default, this property is set to an instance of `BU_AFJSONResponseSerializer`.

 @warning `responseSerializer` must not be `nil`.
 */
@property (nonatomic, strong) id <PAG_AFURLResponseSerialization> responseSerializer;

///-------------------------------
/// @name Managing Security Policy
///-------------------------------

/**
 The security policy used by created session to evaluate server trust for secure connections. `BU_AFURLSessionManager` uses the `defaultPolicy` unless otherwise specified.
 */
@property (nonatomic, strong) PAG_AFSecurityPolicy *securityPolicy;

///----------------------------
/// @name Getting Session Tasks
///----------------------------

/**
 The data, upload, and download tasks currently run by the managed session.
 */
@property (readonly, nonatomic, strong) NSArray <NSURLSessionTask *> *tasks;

/**
 The data tasks currently run by the managed session.
 */
@property (readonly, nonatomic, strong) NSArray <NSURLSessionDataTask *> *dataTasks;

/**
 The upload tasks currently run by the managed session.
 */
@property (readonly, nonatomic, strong) NSArray <NSURLSessionUploadTask *> *uploadTasks;

/**
 The download tasks currently run by the managed session.
 */
@property (readonly, nonatomic, strong) NSArray <NSURLSessionDownloadTask *> *downloadTasks;

///-------------------------------
/// @name Managing Callback Queues
///-------------------------------

/**
 The dispatch queue for `completionBlock`. If `NULL` (default), the main queue is used.
 */
@property (nonatomic, strong, nullable) dispatch_queue_t completionQueue;

/**
 The dispatch group for `completionBlock`. If `NULL` (default), a private dispatch group is used.
 */
@property (nonatomic, strong, nullable) dispatch_group_t completionGroup;

///---------------------------------
/// @name Working Around System Bugs
///---------------------------------

/**
 Whether to attempt to retry creation of upload tasks for background sessions when initial call returns `nil`. `NO` by default.

 @bug As of iOS 7.0, there is a bug where upload tasks created for background tasks are sometimes `nil`. As a workaround, if this property is `YES`, BU_AFNetworking will follow Apple's recommendation to try creating the task again.

 @see https://github.com/BU_AFNetworking/BU_AFNetworking/issues/1675
 */
@property (nonatomic, assign) BOOL attemptsToRecreateUploadTasksForBackgroundSessions;

///---------------------
/// @name Initialization
///---------------------

/**
 Creates and returns a manager for a session created with the specified configuration. This is the designated initializer.

 @param configuration The configuration used to create the managed session.

 @return A manager for a newly-created session.
 */
- (instancetype)initWithSessionConfiguration:(nullable NSURLSessionConfiguration *)configuration NS_DESIGNATED_INITIALIZER;

///-------------------------
/// @name Running Data Tasks
///-------------------------

/**
 Creates an `NSURLSessionDataTask` with the specified request.

 @param request The HTTP request for the request.
 @param completionHandler A block object to be executed when the task finishes. This block has no return value and takes three arguments: the server response, the response object created by that serializer, and the error that occurred, if any.
 */
- (NSURLSessionDataTask *)dataTaskWithRequest:(NSURLRequest *)request
                            completionHandler:(nullable void (^)(NSURLResponse *response, id _Nullable responseObject,  NSError * _Nullable error))completionHandler DEPRECATED_ATTRIBUTE;

/**
 Creates an `NSURLSessionDataTask` with the specified request.

 @param request The HTTP request for the request.
 @param uploadProgressBlock A block object to be executed when the upload progress is updated. Note this block is called on the session queue, not the main queue.
 @param downloadProgressBlock A block object to be executed when the download progress is updated. Note this block is called on the session queue, not the main queue.
 @param completionHandler A block object to be executed when the task finishes. This block has no return value and takes three arguments: the server response, the response object created by that serializer, and the error that occurred, if any.
 */
- (NSURLSessionDataTask *)dataTaskWithRequest:(NSURLRequest *)request
                               uploadProgress:(nullable void (^)(NSProgress *uploadProgress))uploadProgressBlock
                             downloadProgress:(nullable void (^)(NSProgress *downloadProgress))downloadProgressBlock
                            completionHandler:(nullable void (^)(NSURLResponse *response, id _Nullable responseObject,  NSError * _Nullable error))completionHandler;

/**
 Sets a block to be executed when a connection level authentication challenge has occurred, as handled by the `NSURLSessionDelegate` method `URLSession:didReceiveChallenge:completionHandler:`.

 @param block A block object to be executed when a connection level authentication challenge has occurred. The block returns the disposition of the authentication challenge, and takes three arguments: the session, the authentication challenge, and a pointer to the credential that should be used to resolve the challenge.
 */
- (void)setSessionDidReceiveAuthenticationChallengeBlock:(nullable NSURLSessionAuthChallengeDisposition (^)(NSURLSession *session, NSURLAuthenticationChallenge *challenge, NSURLCredential * _Nullable __autoreleasing * _Nullable credential))block;

@end

///--------------------
/// @name Notifications
///--------------------

/**
 Posted when a task resumes.
 */
FOUNDATION_EXPORT NSString * const PAG_AFNetworkingTaskDidResumeNotification;

/**
 Posted when a task finishes executing. Includes a userInfo dictionary with additional information about the task.
 */
FOUNDATION_EXPORT NSString * const PAG_AFNetworkingTaskDidCompleteNotification;

/**
 Posted when a task suspends its execution.
 */
FOUNDATION_EXPORT NSString * const PAG_AFNetworkingTaskDidSuspendNotification;

/**
 Posted when a session is invalidated.
 */
FOUNDATION_EXPORT NSString * const PAG_AFURLSessionDidInvalidateNotification;

/**
 Posted when a session download task encountered an error when moving the temporary download file to a specified destination.
 */
FOUNDATION_EXPORT NSString * const PAG_AFURLSessionDownloadTaskDidFailToMoveFileNotification;

/**
 The raw response data of the task. Included in the userInfo dictionary of the `PAG_AFNetworkingTaskDidCompleteNotification` if response data exists for the task.
 */
FOUNDATION_EXPORT NSString * const PAG_AFNetworkingTaskDidCompleteResponseDataKey;

/**
 The serialized response object of the task. Included in the userInfo dictionary of the `PAG_AFNetworkingTaskDidCompleteNotification` if the response was serialized.
 */
FOUNDATION_EXPORT NSString * const PAG_AFNetworkingTaskDidCompleteSerializedResponseKey;

/**
 The response serializer used to serialize the response. Included in the userInfo dictionary of the `PAG_AFNetworkingTaskDidCompleteNotification` if the task has an associated response serializer.
 */
FOUNDATION_EXPORT NSString * const PAG_AFNetworkingTaskDidCompleteResponseSerializerKey;

/**
 The file path associated with the download task. Included in the userInfo dictionary of the `PAG_AFNetworkingTaskDidCompleteNotification` if an the response data has been stored directly to disk.
 */
FOUNDATION_EXPORT NSString * const PAG_AFNetworkingTaskDidCompleteAssetPathKey;

/**
 Any error associated with the task, or the serialization of the response. Included in the userInfo dictionary of the `PAG_AFNetworkingTaskDidCompleteNotification` if an error exists.
 */
FOUNDATION_EXPORT NSString * const PAG_AFNetworkingTaskDidCompleteErrorKey;

NS_ASSUME_NONNULL_END
