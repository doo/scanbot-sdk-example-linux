#ifndef CHECK_SCANNER_H
#define CHECK_SCANNER_H

#include <ScanbotSDK.h>

scanbotsdk_error_code_t detect_check(scanbotsdk_image_t *image);
scanbotsdk_error_code_t print_check_result(scanbotsdk_check_scanning_result_t *result);

#endif
