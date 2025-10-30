//
//  CmpScanDevice.h
//  Pods
//
//  Created by Cornelli Fabio on 23/11/21.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface CmpHere : NSObject

@property(nonatomic, readonly) NSString *opCode;
@property(nonatomic, readonly) unsigned int devId;
@property(nonatomic, readonly) NSString *macAddress;
@property(nonatomic, readonly) unsigned char flags;

-(instancetype)init:(NSString *)opCode
              devId:(unsigned int)devId
         macAddress:(NSString *)macAddress
              flags:(unsigned char)flags;
@end

@interface CmpInfo : NSObject

@property(nonatomic, readonly) NSString *opCode;
@property(nonatomic, readonly) NSString *vipAddress;
@property(nonatomic, readonly) unsigned short subAddress;
@property(nonatomic, readonly) NSString *macAddress;
@property(nonatomic, readonly) NSString *hwID;
@property(nonatomic, readonly) NSString *appID;
@property(nonatomic, readonly) unsigned int appVersionCode;
@property(nonatomic, readonly) NSString *appVerString;
@property(nonatomic, readonly) NSString *sysID;
@property(nonatomic, readonly) NSString *infoDescription;
@property(nonatomic, readonly) unsigned char flags;
@property(nonatomic, readonly) unsigned char opStatus;
@property(nonatomic, readonly) unsigned char macStatus;
@property(nonatomic, readonly) unsigned char reserved;
@property(nonatomic, readonly) NSString *modelID;
@property(nonatomic, readonly) unsigned char videoEnc;
@property(nonatomic, readonly) unsigned char videoDec;
@property(nonatomic, readonly) unsigned char audioEnc;
@property(nonatomic, readonly) unsigned char audioDec;
@property(nonatomic, readonly) unsigned short extenderFlags;
@property(nonatomic, readonly) unsigned int jsonConfProtVersion;
@property(nonatomic, readonly) unsigned int jsonConfDescVersion;
@property(nonatomic, readonly) unsigned char jsonConfDecType;

-(instancetype)init:(NSString *)opCode
         vipAddress:(NSString *)vipAddress
         subAddress:(unsigned short)subAddress
         macAddress:(NSString *)macAddress
               hwID:(NSString *)hwID
              appID:(NSString *)appID
     appVersionCode:(unsigned int)appVersionCode
       appVerString:(NSString *)appVerString
              sysID:(NSString *)sysID
    infoDescription:(NSString *)infoDescription
              flags:(unsigned char)flags
           opStatus:(unsigned char)opStatus
          macStatus:(unsigned char)macStatus
           reserved:(unsigned char)reserved
            modelID:(NSString *)modelID
           videoEnc:(unsigned char)videoEnc
           videoDec:(unsigned char)videoDec
           audioEnc:(unsigned char)audioEnc
           audioDec:(unsigned char)audioDec
      extenderFlags:(unsigned short)extenderFlags
jsonConfProtVersion:(unsigned int)jsonConfProtVersion
jsonConfDescVersion:(unsigned int)jsonConfDescVersion
    jsonConfDecType:(unsigned char)jsonConfDecType;

@end

@interface CmpScanDevice : NSObject

@property(nonatomic, readonly) NSString *ip;
@property(nonatomic, readonly) int port;
@property(nonatomic, readonly) CmpHere *here;
@property(nonatomic, readonly) CmpInfo *info;
@property(nonatomic, readonly) int cmpSize;

-(instancetype)init:(NSString *)ip
               port:(int)port
               here:(CmpHere *)here
               info:(CmpInfo *)info
            cmpSize:(int)cmpSize;
@end

NS_ASSUME_NONNULL_END
