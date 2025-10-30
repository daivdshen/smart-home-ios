//
//  OctCompatibilityMacros.h
//  OctCoreSDK
//
//  Created by guantou on 2023/10/10.
//

#ifndef OctCompatibilityMacros_h
#define OctCompatibilityMacros_h

#ifdef API_UNAVAILABLE
#define Oct_API_UNAVAILABLE(x) API_UNAVAILABLE(x)
#else
#define Oct_API_UNAVAILABLE(x)
#endif // API_UNAVAILABLE

#if __has_warning("-Wunguarded-availability-new")
#define Oct_CAN_USE_AT_AVAILABLE 1
#else
#define Oct_CAN_USE_AT_AVAILABLE 0
#endif

#endif /* OctCompatibilityMacros_h */
