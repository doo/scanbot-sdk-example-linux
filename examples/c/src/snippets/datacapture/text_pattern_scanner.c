#include <stdio.h>
#include <stdlib.h>

#include <snippets/datacapture/text_pattern_scanner.h>
#include <utils/utils.h>

scanbotsdk_error_code_t print_text_pattern_result(scanbotsdk_text_pattern_scanner_result_t *result) {
    scanbotsdk_error_code_t ec;
    const char* raw_text;

    ec = scanbotsdk_text_pattern_scanner_result_get_raw_text(result, &raw_text);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_raw_text: %d\n", ec); return ec; }

    printf("Raw Text: %s\n", raw_text);
    return ec;
}

scanbotsdk_error_code_t detect_text_pattern(scanbotsdk_image_t *image) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    
    scanbotsdk_text_pattern_scanner_configuration_t *config = NULL;
    scanbotsdk_text_pattern_scanner_t *scanner = NULL;
    scanbotsdk_text_pattern_scanner_result_t *result = NULL;

    ec = scanbotsdk_text_pattern_scanner_configuration_create_with_defaults(&config);
    // Configure other parameters as needed.

    ec = scanbotsdk_text_pattern_scanner_create(config, &scanner);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_text_pattern_scanner_run(scanner, image, &result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_run: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = print_text_pattern_result(result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "print_result: %d: %s\n", ec, error_message(ec)); goto cleanup; }

cleanup:
    scanbotsdk_text_pattern_scanner_result_free(result);
    scanbotsdk_text_pattern_scanner_free(scanner);
    scanbotsdk_text_pattern_scanner_configuration_free(config);
    return ec;
}