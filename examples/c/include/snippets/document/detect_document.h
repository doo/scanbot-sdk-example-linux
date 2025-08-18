#ifndef DETECT_DOCUMENT_H
#define DETECT_DOCUMENT_H

#include <ScanbotSDK.h>

scanbotsdk_error_code_t detect_document(scanbotsdk_image_t *image);
scanbotsdk_error_code_t print_document_result(scanbotsdk_document_detection_result_t *result);

#endif
