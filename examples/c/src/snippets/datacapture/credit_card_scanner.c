#include <stdio.h>
#include <stdlib.h>

#include <snippets/datacapture/credit_card_scanner.h>
#include <utils/utils.h>

scanbotsdk_error_code_t print_credit_card_result(scanbotsdk_credit_card_scanning_result_t *result) {
    scanbotsdk_error_code_t ec;
    scanbotsdk_generic_document_t *credit_card = NULL;
    scanbotsdk_document_detection_status_t status;

    ec = scanbotsdk_credit_card_scanning_result_get_credit_card(result, &credit_card);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_check: %d\n", ec); return ec; }

    scanbotsdk_credit_card_scanning_result_get_detection_status(result, &status);
    printf("Document Detection Status = %d\n", (int)status);

    ec = print_generic_document_fields(credit_card);
    return ec;
}

scanbotsdk_error_code_t detect_credit_card(scanbotsdk_image_t *image) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    
    scanbotsdk_credit_card_scanner_configuration_t *config = NULL;
    scanbotsdk_credit_card_scanner_t *scanner = NULL;
    scanbotsdk_credit_card_scanning_result_t *result = NULL;

    ec = scanbotsdk_credit_card_scanner_configuration_create_with_defaults(&config);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "config_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    // Configure other parameters as needed.

    ec = scanbotsdk_credit_card_scanner_create(config, &scanner);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_credit_card_scanner_run(scanner, image, &result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_run: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = print_credit_card_result(result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "print_result: %d: %s\n", ec, error_message(ec)); goto cleanup; }

cleanup:
    scanbotsdk_credit_card_scanning_result_free(result);
    scanbotsdk_credit_card_scanner_free(scanner);
    scanbotsdk_credit_card_scanner_configuration_free(config);
    return ec;
}