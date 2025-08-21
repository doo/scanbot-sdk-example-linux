#include <stdio.h>
#include <snippets/document/detect_document.h>
#include <utils/utils.h>

scanbotsdk_error_code_t print_document_result(scanbotsdk_document_detection_result_t *result) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_document_detection_status_t status;

    scanbotsdk_document_detection_result_get_status(result, &status);
    printf("Detection Status = %d\n", (int)status);

    scanbotsdk_document_detection_scores_t *scores = NULL;
    ec = scanbotsdk_document_detection_result_get_detection_scores(result, &scores);
    if (ec == SCANBOTSDK_OK && scores) {
        double total = 0.0;
        scanbotsdk_document_detection_scores_get_total_score(scores, &total);
        printf("Total Score: %.4f\n", total);
    } else {
        fprintf(stderr, "get_detection_scores: %d: %s\n", ec, error_message(ec));
    }

    return ec;
}

scanbotsdk_error_code_t detect_document(scanbotsdk_image_t *image) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;

    scanbotsdk_document_scanner_configuration_t *config = NULL;
    scanbotsdk_document_scanner_t *scanner = NULL;
    scanbotsdk_document_detection_result_t *result = NULL;

    ec = scanbotsdk_document_scanner_configuration_create_with_defaults(&config);
    // Configure other parameters as needed.

    ec = scanbotsdk_document_scanner_create(config, &scanner);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_document_scanner_run(scanner, image, &result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_run: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = print_document_result(result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "print_result: %d: %s\n", ec, error_message(ec)); goto cleanup; }

cleanup:
    scanbotsdk_document_detection_result_free(result);
    scanbotsdk_document_scanner_free(scanner);
    scanbotsdk_document_scanner_configuration_free(config);
    return ec;
}