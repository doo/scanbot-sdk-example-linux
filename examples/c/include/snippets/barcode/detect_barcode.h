#ifndef DETECT_BARCODE_H
#define DETECT_BARCODE_H

#include <ScanbotSDK.h>

scanbotsdk_error_code_t detect_barcode(scanbotsdk_image_t *image);
scanbotsdk_error_code_t print_barcodes_result(scanbotsdk_barcode_scanner_result_t *result);

#endif
