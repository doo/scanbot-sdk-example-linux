#include <stdio.h>
#include <stdlib.h>

#include <snippets/datacapture/mrz_scanner.h>
#include <utils/utils.h>

scanbotsdk_error_code_t print_mrz_result(scanbotsdk_mrz_scanner_result_t *result) {
    scanbotsdk_error_code_t ec;
    scanbotsdk_generic_document_t *mrz = NULL;
    bool status;

    ec = scanbotsdk_mrz_scanner_result_get_document(result, &mrz);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_document: %d\n", ec); return ec; }

    scanbotsdk_mrz_scanner_result_get_success(result, &status);
    printf("Mrz detection success status = %d\n", status);

    if(mrz != NULL) { 
        print_generic_document_fields(mrz); 
    }
    return ec;
}

scanbotsdk_error_code_t detect_mrz(scanbotsdk_image_t *image) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    
    scanbotsdk_mrz_scanner_configuration_t *config = NULL;
    scanbotsdk_mrz_scanner_t *scanner = NULL;
    scanbotsdk_mrz_scanner_result_t *result = NULL;
    scanbotsdk_accumulated_results_verifier_configuration_t *frame_accumulation_config = NULL;

    ec = scanbotsdk_mrz_scanner_configuration_create_with_defaults(&config);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "config_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_accumulated_results_verifier_configuration_create_with_defaults(&frame_accumulation_config);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "frame_accumulation_config_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    scanbotsdk_accumulated_results_verifier_configuration_set_maximum_number_of_accumulated_frames(frame_accumulation_config, 1);
    scanbotsdk_mrz_scanner_configuration_set_frame_accumulation_configuration(config, frame_accumulation_config);
    // Configure other parameters as needed.
    
    ec = scanbotsdk_mrz_scanner_create(config, &scanner);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_mrz_scanner_run(scanner, image, &result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_run: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = print_mrz_result(result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "print_result: %d: %s\n", ec, error_message(ec)); goto cleanup; }

cleanup:
    scanbotsdk_accumulated_results_verifier_configuration_free(frame_accumulation_config);
    scanbotsdk_mrz_scanner_result_free(result);
    scanbotsdk_mrz_scanner_free(scanner);
    scanbotsdk_mrz_scanner_configuration_free(config);
    return ec;
}