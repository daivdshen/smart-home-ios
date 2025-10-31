//
//  HMDPAGCrashDetect.h
//  CaptainAllred
//
//  Created by sunrunwang on 2019/7/11.
//  Copyright © 2019 sunrunwang. All rights reserved.
//
//  非线程安全 🔒

#ifdef __cplusplus
extern "C" {
#endif
    
    void HMDPAGCrashStartDetect(void);

    void HMDPAGCrashCheckHandler(void);
    
#ifdef __cplusplus
} // extern "C"
#endif

