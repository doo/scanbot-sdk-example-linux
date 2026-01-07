#ifndef UTILS_H
#define UTILS_H

#include <ScanbotSDK.h>

const char *error_message(scanbotsdk_error_code_t ec);

scanbotsdk_error_code_t load_image_from_path(const char *path, scanbotsdk_image_t **out_image);

scanbotsdk_error_code_t print_generic_document_fields(scanbotsdk_generic_document_t *doc);

void print_usage(const char *prog);

const char* get_flag(int argc, char *argv[], const char *flag);

#endif
