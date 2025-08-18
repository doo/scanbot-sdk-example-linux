#ifndef DDE_EXTRACTOR_H
#define DDE_EXTRACTOR_H

#include <ScanbotSDK.h>

scanbotsdk_error_code_t extract_document_data(scanbotsdk_image_t *image);
scanbotsdk_error_code_t print_dde_result(scanbotsdk_document_data_extraction_result_t *result);

#endif
