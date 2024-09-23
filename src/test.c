#include <stdio.h>
#include <stdlib.h>

#include "include/boardinfo.h"
#include "include/cpuinfo.h"

int main(int argc, char* argv[]) {
    cpu_info_t* cpu_info = get_cpu_identifier( );
    printf("\n - CPU Info - \n");
    if (cpu_info != NULL) {
        printf("%s\n", cpu_info->cpu_model);
        free(cpu_info);
    }

    board_info_t* board_info = get_board_info( );

    printf("\n - Board Info - \n");
    if (board_info != NULL) {
        printf("Board Serial: %s\n", board_info->board_serial);
        printf("Board Model: %s\n", board_info->board_model);
        printf("Manufacturer: %s\n", board_info->vendor);
        printf("Board Revision: %s\n", board_info->board_revision);
        free(board_info);  // Free the allocated memory
    } else {
        printf("Failed to retrieve board fingerprint.\n");
    }

    return 0;
}
