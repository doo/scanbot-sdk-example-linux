#ifndef OCR_SCANNER_H
#define OCR_SCANNER_H

#include <ScanbotSDK.h>

scanbotsdk_error_code_t print_ocr_result(scanbotsdk_page_t *result);
scanbotsdk_error_code_t ocr_run(scanbotsdk_image_t *image);

#endif
