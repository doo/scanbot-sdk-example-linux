#ifndef CREDIT_CARD_SCANNER_H
#define CREDIT_CARD_SCANNER_H

#include <ScanbotSDK.h>

scanbotsdk_error_code_t detect_credit_card(scanbotsdk_image_t *image);
scanbotsdk_error_code_t print_credit_card_result(scanbotsdk_credit_card_scanning_result_t *result);

#endif
