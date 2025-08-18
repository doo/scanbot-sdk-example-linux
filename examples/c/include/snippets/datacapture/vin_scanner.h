#ifndef VIN_SCANNER_H
#define VIN_SCANNER_H

#include <ScanbotSDK.h>

scanbotsdk_error_code_t detect_vin(scanbotsdk_image_t *image);
scanbotsdk_error_code_t print_vin_result(scanbotsdk_vin_scanner_result_t *result);

#endif
