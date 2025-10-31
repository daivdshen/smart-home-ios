//
//  HMDPAGCrashExtraDynamicData.hpp
//  Pods
//
//  Created by bytedance on 2020/7/29.
//

#ifndef HMDPAGCrashExtraDynamicData_hpp
#define HMDPAGCrashExtraDynamicData_hpp

#include <mach/machine.h>
#include <mach/mach_types.h>

typedef void (*hmdPAG_crash_dynamic_data_callback)(char **dynamic_key, char **dynamic_data, uint64_t crash_time, uint64_t fault_address, thread_t current_thread, thread_t crash_thread);
typedef void (*extra_dynamic_data_callback)(const char *key, const char *value, void *ctx);

#ifdef __cplusplus
extern "C" {
#endif
    
    bool hmdPAG_crash_has_extra_dynamic_data_callback(void);
    void hmdPAG_crash_extra_dynamic_data_add_callback(hmdPAG_crash_dynamic_data_callback callback);
    void hmdPAG_crash_extra_dynamic_data_remove_callback(hmdPAG_crash_dynamic_data_callback callback);

    void hmdPAG_crash_async_enumerate_extra_dynamic_data(uint64_t crash_time, uint64_t fault_address, thread_t current_thread, thread_t crash_thread, extra_dynamic_data_callback callback, void *ctx);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* HMDPAGCrashExtraDynamicData_hpp */
