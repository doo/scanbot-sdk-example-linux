#ifndef MEDICAL_CERTIFICATE_SCANNER_H
#define MEDICAL_CERTIFICATE_SCANNER_H

#include <ScanbotSDK.h>

scanbotsdk_error_code_t detect_medical_certificate(scanbotsdk_image_t *image);
scanbotsdk_error_code_t print_medical_certificate_result(scanbotsdk_medical_certificate_scanning_result_t *result);

#endif
