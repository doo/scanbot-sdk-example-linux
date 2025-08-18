#ifndef MRZ_SCANNER_H
#define MRZ_SCANNER_H

#include <ScanbotSDK.h>

scanbotsdk_error_code_t detect_mrz(scanbotsdk_image_t *image);
scanbotsdk_error_code_t print_mrz_result(scanbotsdk_mrz_scanner_result_t *result);

#endif
