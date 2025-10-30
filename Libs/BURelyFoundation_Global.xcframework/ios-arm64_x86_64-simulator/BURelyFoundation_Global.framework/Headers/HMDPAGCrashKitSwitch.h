//
//  HMDPAGCrashKitSwitch.h
//  Pods
//
//  Created by yuanzhangjing on 2019/12/1.
//

#ifndef HMDPAGCrashKitSwitch_h
#define HMDPAGCrashKitSwitch_h

#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    enum {
        HMDPAGCrashSwitchRegisterAnalysis = 0,
        HMDPAGCrashSwitchStackAnalysis,
        HMDPAGCrashSwitchVMMap,
        HMDPAGCrashSwitchCPPBacktrace,
        HMDPAGCrashSwitchContentAnalysis,
        HMDPAGCrashSwitchIgnoreExitByUser,
        HMDPAGCrashSwitchWriteImageOnCrash
    };

    void hmdPAG_crash_switch_update(unsigned int option, bool value);

    bool hmdPAG_crash_switch_state(unsigned int option);

    void hmdPAG_crash_update_stack_trace_count(uint32_t count);

    uint32_t hmdPAG_crash_stack_trace_count(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* HMDPAGCrashKitSwitch_h */
