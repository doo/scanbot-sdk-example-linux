#ifndef CROP_AND_ANALYSE_H
#define CROP_AND_ANALYSE_H

#include <ScanbotSDK.h>

scanbotsdk_error_code_t save_cropped_image(scanbotsdk_image_t *cropped_image, char *save_path);
scanbotsdk_error_code_t crop_and_analyse(scanbotsdk_image_t *image, char* save_path);
scanbotsdk_error_code_t detect_document_cropped(scanbotsdk_image_t *image, scanbotsdk_image_t **cropped_image_out);
void print_result(scanbotsdk_document_quality_analyzer_result_t *result);

#endif
