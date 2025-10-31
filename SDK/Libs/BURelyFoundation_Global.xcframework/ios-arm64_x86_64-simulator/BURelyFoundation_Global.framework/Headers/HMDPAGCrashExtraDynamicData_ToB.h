//
//  HMDPAGCrashExtraDynamicData_ToB.h
//  Pods
//
//  Created by xuminghao.eric on 2022/8/23.
//

#ifndef HMDPAGCrashExtraDynamicData_ToB_h
#define HMDPAGCrashExtraDynamicData_ToB_h

#include <mach/machine.h>
#include <mach/mach_types.h>

typedef void (*hmdPAG_crash_dynamic_data_callback)(char **dynamic_key, char **dynamic_data, uint64_t crash_time, uint64_t fault_address, thread_t current_thread, thread_t crash_thread);
typedef void (*extra_dynamic_data_callback)(const char *key, const char *value, void *ctx);
typedef void (*extra_dynamic_data_start_callback)(const char *aid, void *ctx);
typedef void (*extra_dynamic_data_end_callback)(void);

#ifdef __cplusplus
extern "C" {
#endif
    
    bool hmdPAG_crash_has_extra_dynamic_data_callback(void);
    void hmdPAG_crash_extra_dynamic_data_add_callback_for_aid(hmdPAG_crash_dynamic_data_callback callback, const char *aid);
    void hmdPAG_crash_extra_dynamic_data_remove_callback_for_aid(hmdPAG_crash_dynamic_data_callback callback, const char *aid);
    void hmdPAG_crash_clear_extra_dynamic_except_aid(const char *aid);

    void hmdPAG_crash_async_enumerate_extra_dynamic_data_for_all_aid(uint64_t crash_time, uint64_t fault_address, thread_t current_thread, thread_t crash_thread, extra_dynamic_data_callback callback, extra_dynamic_data_start_callback aid_start, extra_dynamic_data_end_callback aid_end, void *ctx);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* HMDPAGCrashExtraDynamicData_ToB_h */
