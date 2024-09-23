#include "include/boardinfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int read_sys_file(const char* path, char* output, size_t output_size) {
    FILE* fp = fopen(path, "r");
    if (fp == NULL) return -1;

    if (fgets(output, output_size, fp) != NULL)
        output[strcspn(output, "\n")] = '\0';
    fclose(fp);
    return 0;
}

static int read_dmi_info(board_info_t* board_info) {
    int found = 0;

    if (read_sys_file("/sys/class/dmi/id/board_serial",
                      board_info->board_serial,
                      sizeof(board_info->board_serial)) == 0)
        found++;
    if (read_sys_file("/sys/class/dmi/id/product_name", board_info->board_model,
                      sizeof(board_info->board_model)) == 0)
        found++;
    if (read_sys_file("/sys/class/dmi/id/board_vendor", board_info->vendor,
                      sizeof(board_info->vendor)) == 0)
        found++;
    if (read_sys_file("/sys/class/dmi/id/board_version",
                      board_info->board_revision,
                      sizeof(board_info->board_revision)) == 0)
        found++;

    return found;
}

static int read_device_tree_info(board_info_t* board_info) {
    int found = 0;

    if (read_sys_file("/proc/device-tree/serial-number",
                      board_info->board_serial,
                      sizeof(board_info->board_serial)) == 0) {
        found++;
    }

    // Read board model from device tree
    if (read_sys_file("/proc/device-tree/model", board_info->board_model,
                      sizeof(board_info->board_model)) == 0) {
        found++;
    }

    return found;
}

static int read_cpuinfo_serial(board_info_t* board_info) {
    FILE* fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) {
        return -1;
    }

    char buffer[256] = {0};
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, "Serial")) {
            // Extract serial number from line like "Serial      :
            // 00000000xxxxxxx"
            sscanf(buffer, "Serial : %s", board_info->board_serial);
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return -1;
}

board_info_t* get_board_info( ) {
    board_info_t* board_info = ( board_info_t* ) malloc(sizeof(board_info_t));
    if (board_info == NULL) {
        return NULL;  // Memory allocation failed
    }
    memset(board_info, 0, sizeof(board_info_t));

    // Try reading from /sys/class/dmi/id/ (mostly for x86 and some ARM devices)
    if (read_dmi_info(board_info) > 0) {
        return board_info;
    }

    // If DMI info is unavailable, try /proc/device-tree (common on ARM)
    if (read_device_tree_info(board_info) > 0) {
        return board_info;
    }

    // As a fallback, check /proc/cpuinfo for the serial number
    if (read_cpuinfo_serial(board_info) == 0) {
        // Assume model and other details are unknown
        strcpy(board_info->board_model, "Unknown");
        strcpy(board_info->vendor, "Unknown");
        strcpy(board_info->board_revision, "Unknown");
        return board_info;
    }

    // If no information was found, fill in "Unknown"
    strcpy(board_info->board_serial, "Unknown");
    strcpy(board_info->board_model, "Unknown");
    strcpy(board_info->vendor, "Unknown");
    strcpy(board_info->board_revision, "Unknown");

    return board_info;
}
