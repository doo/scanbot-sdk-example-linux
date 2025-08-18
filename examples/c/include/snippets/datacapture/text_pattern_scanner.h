#ifndef TEXT_PATTERN_SCANNER_H
#define TEXT_PATTERN_SCANNER_H

#include <ScanbotSDK.h>

scanbotsdk_error_code_t detect_text_pattern(scanbotsdk_image_t *image);
scanbotsdk_error_code_t print_text_pattern_result(scanbotsdk_text_pattern_scanner_result_t *result);

#endif
