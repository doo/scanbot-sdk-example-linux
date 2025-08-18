#include <stdio.h>
#include <stdlib.h>

#include <snippets/datacapture/vin_scanner.h>
#include <utils/utils.h>

scanbotsdk_error_code_t print_vin_result(scanbotsdk_vin_scanner_result_t *result) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_text_pattern_scanner_result_t *text_result;
    const char* raw_text;

    scanbotsdk_vin_scanner_result_get_text_result(result, &text_result);
    scanbotsdk_text_pattern_scanner_result_get_raw_text(text_result, &raw_text);

    printf("Text: %s\n", raw_text);
    return ec;
}

scanbotsdk_error_code_t detect_vin(scanbotsdk_image_t *image) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    
    scanbotsdk_vin_scanner_configuration_t *config = NULL;
    scanbotsdk_vin_scanner_t *scanner = NULL;
    scanbotsdk_vin_scanner_result_t *result = NULL;

    ec = scanbotsdk_vin_scanner_configuration_create_with_defaults(&config);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "config_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    // Configure other parameters as needed.

    ec = scanbotsdk_vin_scanner_create(config, &scanner);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_vin_scanner_run(scanner, image, &result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_run: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = print_vin_result(result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "print_result: %d: %s\n", ec, error_message(ec)); goto cleanup; }

cleanup:
    scanbotsdk_vin_scanner_result_free(result);
    scanbotsdk_vin_scanner_free(scanner);
    scanbotsdk_vin_scanner_configuration_free(config);
    return ec;
}