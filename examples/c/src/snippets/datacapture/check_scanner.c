#include <stdio.h>
#include <stdlib.h>

#include <snippets/datacapture/check_scanner.h>
#include <utils/utils.h>

scanbotsdk_error_code_t print_check_result(scanbotsdk_check_scanning_result_t *result) {
    scanbotsdk_error_code_t ec;
    scanbotsdk_check_magnetic_ink_strip_scanning_status_t ink_strip_status;
    scanbotsdk_generic_document_t *check = NULL;
    
    scanbotsdk_document_detection_result_t *detect_result = NULL;
    scanbotsdk_document_detection_status_t status;

    ec = scanbotsdk_check_scanning_result_get_status(result, &ink_strip_status);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_status: %d\n", ec); return ec; }
    printf("Ink strip status: %d\n", (int)ink_strip_status);

    ec = scanbotsdk_check_scanning_result_get_check(result, &check);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_check: %d\n", ec); return ec; }

    ec = scanbotsdk_check_scanning_result_get_document_detection_result(result, &detect_result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_document_detection_result: %d: %s\n", ec, error_message(ec)); return ec; }

    scanbotsdk_document_detection_result_get_status(detect_result, &status);
    printf("Document Detection Status = %d\n", (int)status);

    if(check != NULL) { 
        print_generic_document_fields(check); 
    }
    return ec;
}

scanbotsdk_error_code_t detect_check(scanbotsdk_image_t *image) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    
    scanbotsdk_check_scanner_configuration_t *config = NULL;
    scanbotsdk_check_scanner_t *scanner = NULL;
    scanbotsdk_check_scanning_result_t *result = NULL;

    ec = scanbotsdk_check_scanner_configuration_create_with_defaults(&config);
    ec = scanbotsdk_check_scanner_configuration_set_document_detection_mode(config, SCANBOTSDK_CHECK_DOCUMENT_DETECTION_MODE_DETECT_AND_CROP_DOCUMENT);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "set_document_detection_mode: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    // Configure other parameters as needed.

    ec = scanbotsdk_check_scanner_create(config, &scanner);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_check_scanner_run(scanner, image, &result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_run: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = print_check_result(result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "print_result: %d: %s\n", ec, error_message(ec)); goto cleanup; }

cleanup:
    scanbotsdk_check_scanning_result_free(result);
    scanbotsdk_check_scanner_free(scanner);
    scanbotsdk_check_scanner_configuration_free(config);
    return ec;
}