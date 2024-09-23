#ifndef __CPUINFO_H__
#define __CPUINFO_H__

#include <stddef.h>

typedef enum {
    SUCCESS = 0,
    ERR_NO_CPU_SERIAL,
    ERR_NO_PRODUCT_UUID,
    ERR_UNKNOWN
} cpu_status_t;

typedef struct {
    char cpu_serial[256];
    char product_uuid[256];
    char cpu_model[256];
    int  cpu_cores;
    long cache_size;     // in KB
    char soc_info[256];  // Specific info of SoC-based platforms
} cpu_info_t;

cpu_info_t* get_cpu_identifier( );

#endif /* __CPUINFO_H__ */
