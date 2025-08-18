#ifndef ANALYSE_MULTI_PAGE_H
#define ANALYSE_MULTI_PAGE_H

#include <ScanbotSDK.h>

// scanbotsdk_error_code_t analyse_multi_page(scanbotsdk_image_t *image);
scanbotsdk_error_code_t analyse_multi_page(char* path);
void print_analyzer_result(scanbotsdk_document_quality_analyzer_result_t *result);

#endif
