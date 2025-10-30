//
//  CMGNetUtils.h
//  ComelitCoreKit
//
//  Created by Mutti Simone on 23/10/17.
//  Copyright © 2017 ComelitGroup. All rights reserved.
//
//  Utility class for networking stuff.
//

#import <Foundation/Foundation.h>
#import <ComelitCoreKit/P2pFilterStrategy.h>

@interface CMGNetUtils : NSObject

+ (nonnull NSString *)resolveHostname:(nonnull NSString *)hostname port:(NSInteger)port;
+ (BOOL)isValidIPAddress:(nonnull NSString *)hostName;

// Network Information

// Get Current IP Address
+ (nullable NSString *)currentIPAddress;

// Get the External IP Address
+ (nullable NSString *)externalIPAddress;

// Get Cell IP Address
+ (nullable NSString *)cellIPAddress;

// Get Cell IP Address for a given interface
+ (nullable NSString *)cellIPAddressForInterfaceName:(nonnull NSString *)interfaceName;

// Get Cell IPv6 Address
+ (nullable NSString *)cellIPv6Address;

// Get Cell IPv6 Address for a given interface
+ (nullable NSString *)cellIPv6AddressForInterfaceName:(nonnull NSString *)interfaceName;

// Get Cell Netmask Address
+ (nullable NSString *)cellNetmaskAddress;

// Get Cell Netmask Address for a given interface
+ (nullable NSString *)cellNetmaskAddressForInterfaceName:(nonnull NSString *)interfaceName;

// Get Cell Broadcast Address
+ (nullable NSString *)cellBroadcastAddress;

// Get Cell Broadcast Address for a given interface
+ (nullable NSString *)cellBroadcastAddressForInterfaceName:(nonnull NSString *)interfaceName;

// Get WiFi IP Address
+ (nullable NSString *)wiFiIPAddress;

// Get WiFi IP Address for a given interface
+ (nullable NSString *)wiFiIPAddressForInterfaceName:(nonnull NSString *)interfaceName;

// Get WiFi Netmask Address
+ (nullable NSString *)wiFiNetmaskAddress;

// Get WiFi Netmask Address for a given interface
+ (nullable NSString *)wiFiNetmaskAddressForInterfaceName:(nonnull NSString *)interfaceName;

// Get WiFi Broadcast Address
+ (nullable NSString *)wiFiBroadcastAddress;

// Get WiFi Broadcast Address for a given interface
+ (nullable NSString *)wiFiBroadcastAddressForInterfaceName:(nonnull NSString *)interfaceName;

// Connected to WiFi?
+ (BOOL)connectedToWiFi;

// Connected to Cellular Network?
+ (BOOL)connectedToCellNetwork;

+ (P2pFilterStrategy)calculateStrategyForP2P:(nullable NSString *)mccMnc isP2Pv2:(BOOL)isP2Pv2;

+ (NSInteger)calculateDelayForP2P:(nullable NSString *)mccMnc;

+ (BOOL)calculateIpV6OnlyForP2P:(nullable NSString *)mccMnc;

+ (nonnull NSString *)currentMccMnc;
@end
