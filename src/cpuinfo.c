#include "include/cpuinfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * CPU serial is not available with modern processors -- post PIII,
 * but it may be available with some embedded systems. This func
 * may further be improved by considering
 */
static int get_cpu_serial(char *buffer, size_t buffer_size) {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) {
        perror("Failed to get cpu serial");
        return ERR_NO_CPU_SERIAL;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strcasestr(line, "Serial")) {
            sscanf(line, "Serial: %255s", buffer);
            fclose(fp);
            return SUCCESS;
        }
    }

    fclose(fp);
    return ERR_NO_CPU_SERIAL;
}

static int get_product_uuid(char *buffer, size_t buffer_size) {
    FILE *fp = fopen("/sys/class/dmi/id/product_uuid", "r");
    if (!fp) {
        return ERR_NO_PRODUCT_UUID;
    }

    if (fgets(buffer, buffer_size, fp)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        fclose(fp);
        return SUCCESS;
    }

    fclose(fp);
    return ERR_NO_PRODUCT_UUID;
}

static void get_other_cpu_details(cpu_info_t *cpu_info) {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) {
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strcasestr(line, "model name")) {
            sscanf(line, "model name      : %255[^\n]", cpu_info->cpu_model);
        } else if (strcasestr(line, "cpu cores")) {
            sscanf(line, "cpu cores       : %d", &cpu_info->cpu_cores);
        } else if (strcasestr(line, "cache size")) {
            sscanf(line, "cache size      : %ld KB", &cpu_info->cache_size);
        }
    }

    fclose(fp);

    // Check for SoC-specific info
    FILE *soc_fp = fopen("/proc/device-tree/model", "r");
    if (soc_fp &&
        fgets(cpu_info->soc_info, sizeof(cpu_info->soc_info), soc_fp)) {
        cpu_info->soc_info[strcspn(cpu_info->soc_info, "\n")] = '\0';
        fclose(soc_fp);
    }
}

cpu_info_t *get_cpu_identifier( ) {
    cpu_info_t *cpu_info = ( cpu_info_t * ) malloc(sizeof(cpu_info_t));
    memset(cpu_info, 0, sizeof(cpu_info_t));

    // Try to get CPU serial number
    get_cpu_serial(cpu_info->cpu_serial, sizeof(cpu_info->cpu_serial));

    // Try to get product UUID
    get_product_uuid(cpu_info->product_uuid, sizeof(cpu_info->product_uuid));

    // If neither serial nor UUID is found, gather general CPU information
    get_other_cpu_details(cpu_info);

    return cpu_info;
}
