//
//  OctURLResponseSerialization.h
//  OctCoreSDK
//
//  Created by guantou on 2023/10/10.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

NS_ASSUME_NONNULL_BEGIN

@protocol OctURLResponseSerialization <NSObject, NSSecureCoding, NSCopying>

- (nullable id)responseObjectForResponse:(nullable NSURLResponse *)response
                                    data:(nullable NSData *)data
                                   error:(NSError * _Nullable __autoreleasing *)error NS_SWIFT_NOTHROW;

@end

#pragma mark -

@interface OctHTTPResponseSerializer : NSObject <OctURLResponseSerialization>

- (instancetype)init;

@property (nonatomic, assign) NSStringEncoding stringEncoding DEPRECATED_MSG_ATTRIBUTE("The string encoding is never used. OctHTTPResponseSerializer only validates status codes and content types but does not try to decode the received data in any way.");

+ (instancetype)serializer;

@property (nonatomic, copy, nullable) NSIndexSet *acceptableStatusCodes;

@property (nonatomic, copy, nullable) NSSet <NSString *> *acceptableContentTypes;

- (BOOL)validateResponse:(nullable NSHTTPURLResponse *)response
                    data:(nullable NSData *)data
                   error:(NSError * _Nullable __autoreleasing *)error;

@end

#pragma mark -

@interface OctJSONResponseSerializer : OctHTTPResponseSerializer

- (instancetype)init;

@property (nonatomic, assign) NSJSONReadingOptions readingOptions;

@property (nonatomic, assign) BOOL removesKeysWithNullValues;

+ (instancetype)serializerWithReadingOptions:(NSJSONReadingOptions)readingOptions;

@end

#pragma mark -

@interface OctXMLParserResponseSerializer : OctHTTPResponseSerializer

@end

#pragma mark -

#ifdef __MAC_OS_X_VERSION_MIN_REQUIRED

@interface OctXMLDocumentResponseSerializer : OctHTTPResponseSerializer

- (instancetype)init;

@property (nonatomic, assign) NSUInteger options;

+ (instancetype)serializerWithXMLDocumentOptions:(NSUInteger)mask;

@end

#endif

#pragma mark -

@interface OctPropertyListResponseSerializer : OctHTTPResponseSerializer

- (instancetype)init;

@property (nonatomic, assign) NSPropertyListFormat format;

@property (nonatomic, assign) NSPropertyListReadOptions readOptions;

+ (instancetype)serializerWithFormat:(NSPropertyListFormat)format
                         readOptions:(NSPropertyListReadOptions)readOptions;

@end

#pragma mark -

@interface OctImageResponseSerializer : OctHTTPResponseSerializer

#if TARGET_OS_IOS || TARGET_OS_TV || TARGET_OS_WATCH

@property (nonatomic, assign) CGFloat imageScale;

@property (nonatomic, assign) BOOL automaticallyInflatesResponseImage;
#endif

@end

#pragma mark -

@interface OctCompoundResponseSerializer : OctHTTPResponseSerializer

@property (readonly, nonatomic, copy) NSArray <id<OctURLResponseSerialization>> *responseSerializers;

+ (instancetype)compoundSerializerWithResponseSerializers:(NSArray <id<OctURLResponseSerialization>> *)responseSerializers;

@end

FOUNDATION_EXPORT NSString * const OctURLResponseSerializationErrorDomain;

FOUNDATION_EXPORT NSString * const OctNetworkingOperationFailingURLResponseErrorKey;

FOUNDATION_EXPORT NSString * const OctNetworkingOperationFailingURLResponseDataErrorKey;

NS_ASSUME_NONNULL_END

