//
//  OctClassInfo.h
//  TestView
//
//  Created by guantou on 2020/1/6.
//  Copyright © 2020 北京市吕俊学. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <objc/runtime.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, OctEncodingType) {
    OctEncodingTypeMask       = 0xFF, ///< mask of type value
    OctEncodingTypeUnknown    = 0, ///< unknown
    OctEncodingTypeVoid       = 1, ///< void
    OctEncodingTypeBool       = 2, ///< bool
    OctEncodingTypeInt8       = 3, ///< char / BOOL
    OctEncodingTypeUInt8      = 4, ///< unsigned char
    OctEncodingTypeInt16      = 5, ///< short
    OctEncodingTypeUInt16     = 6, ///< unsigned short
    OctEncodingTypeInt32      = 7, ///< int
    OctEncodingTypeUInt32     = 8, ///< unsigned int
    OctEncodingTypeInt64      = 9, ///< long long
    OctEncodingTypeUInt64     = 10, ///< unsigned long long
    OctEncodingTypeFloat      = 11, ///< float
    OctEncodingTypeDouble     = 12, ///< double
    OctEncodingTypeLongDouble = 13, ///< long double
    OctEncodingTypeObject     = 14, ///< id
    OctEncodingTypeClass      = 15, ///< Class
    OctEncodingTypeSEL        = 16, ///< SEL
    OctEncodingTypeBlock      = 17, ///< block
    OctEncodingTypePointer    = 18, ///< void*
    OctEncodingTypeStruct     = 19, ///< struct
    OctEncodingTypeUnion      = 20, ///< union
    OctEncodingTypeCString    = 21, ///< char*
    OctEncodingTypeCArray     = 22, ///< char[10] (for example)
    
    OctEncodingTypeQualifierMask   = 0xFF00,   ///< mask of qualifier
    OctEncodingTypeQualifierConst  = 1 << 8,  ///< const
    OctEncodingTypeQualifierIn     = 1 << 9,  ///< in
    OctEncodingTypeQualifierInout  = 1 << 10, ///< inout
    OctEncodingTypeQualifierOut    = 1 << 11, ///< out
    OctEncodingTypeQualifierBycopy = 1 << 12, ///< bycopy
    OctEncodingTypeQualifierByref  = 1 << 13, ///< byref
    OctEncodingTypeQualifierOneway = 1 << 14, ///< oneway
    
    OctEncodingTypePropertyMask         = 0xFF0000, ///< mask of property
    OctEncodingTypePropertyReadonly     = 1 << 16, ///< readonly
    OctEncodingTypePropertyCopy         = 1 << 17, ///< copy
    OctEncodingTypePropertyRetain       = 1 << 18, ///< retain
    OctEncodingTypePropertyNonatomic    = 1 << 19, ///< nonatomic
    OctEncodingTypePropertyWeak         = 1 << 20, ///< weak
    OctEncodingTypePropertyCustomGetter = 1 << 21, ///< getter=
    OctEncodingTypePropertyCustomSetter = 1 << 22, ///< setter=
    OctEncodingTypePropertyDynamic      = 1 << 23, ///< @dynamic
};

OctEncodingType OctEncodingGetType(const char *typeEncoding);

@interface OctClassIvarInfo : NSObject

@property (nonatomic, assign, readonly) Ivar ivar;              ///< ivar opaque struct
@property (nonatomic, strong, readonly) NSString *name;         ///< Ivar's name
@property (nonatomic, assign, readonly) ptrdiff_t offset;       ///< Ivar's offset
@property (nonatomic, strong, readonly) NSString *typeEncoding; ///< Ivar's type encoding
@property (nonatomic, assign, readonly) OctEncodingType type;    ///< Ivar's type

- (instancetype)initWithIvar:(Ivar)ivar;

@end

@interface OctClassMethodInfo : NSObject

@property (nonatomic, assign, readonly) Method method;                  ///< method opaque struct
@property (nonatomic, strong, readonly) NSString *name;                 ///< method name
@property (nonatomic, assign, readonly) SEL sel;                        ///< method's selector
@property (nonatomic, assign, readonly) IMP imp;                        ///< method's implementation
@property (nonatomic, strong, readonly) NSString *typeEncoding;         ///< method's parameter and return types
@property (nonatomic, strong, readonly) NSString *returnTypeEncoding;   ///< return value's type
@property (nullable, nonatomic, strong, readonly) NSArray<NSString *> *argumentTypeEncodings; ///< array of arguments' type

- (instancetype)initWithMethod:(Method)method;

@end

@interface OctClassPropertyInfo : NSObject

@property (nonatomic, assign, readonly) objc_property_t property; ///< property's opaque struct
@property (nonatomic, strong, readonly) NSString *name;           ///< property's name
@property (nonatomic, assign, readonly) OctEncodingType type;      ///< property's type
@property (nonatomic, strong, readonly) NSString *typeEncoding;   ///< property's encoding value
@property (nonatomic, strong, readonly) NSString *ivarName;       ///< property's ivar name
@property (nullable, nonatomic, assign, readonly) Class cls;      ///< may be nil
@property (nullable, nonatomic, strong, readonly) NSArray<NSString *> *protocols; ///< may nil
@property (nonatomic, assign, readonly) SEL getter;               ///< getter (nonnull)
@property (nonatomic, assign, readonly) SEL setter;               ///< setter (nonnull)

- (instancetype)initWithProperty:(objc_property_t)property;

@end

@interface OctClassInfo : NSObject

@property (nonatomic, assign, readonly) Class cls; ///< class object
@property (nullable, nonatomic, assign, readonly) Class superCls; ///< super class object
@property (nullable, nonatomic, assign, readonly) Class metaCls;  ///< class's meta class object
@property (nonatomic, readonly) BOOL isMeta; ///< whether this class is meta class
@property (nonatomic, strong, readonly) NSString *name; ///< class name
@property (nullable, nonatomic, strong, readonly) OctClassInfo *superClassInfo; ///< super class's class info
@property (nullable, nonatomic, strong, readonly) NSDictionary<NSString *, OctClassIvarInfo *> *ivarInfos; ///< ivars
@property (nullable, nonatomic, strong, readonly) NSDictionary<NSString *, OctClassMethodInfo *> *methodInfos; ///< methods
@property (nullable, nonatomic, strong, readonly) NSDictionary<NSString *, OctClassPropertyInfo *> *propertyInfos; ///< properties

- (void)setNeedUpdate;

- (BOOL)needUpdate;

+ (nullable instancetype)classInfoWithClass:(Class)cls;

+ (nullable instancetype)classInfoWithClassName:(NSString *)className;

@end

NS_ASSUME_NONNULL_END

