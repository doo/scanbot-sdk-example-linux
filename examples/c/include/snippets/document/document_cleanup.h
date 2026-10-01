#ifndef DOCUMENT_CLEANUP_H
#define DOCUMENT_CLEANUP_H

#include <ScanbotSDK.h>

scanbotsdk_error_code_t document_cleanup(
    scanbotsdk_image_t *image,
    scanbotsdk_image_t *mask,
    const char *save_path
);

#endif
