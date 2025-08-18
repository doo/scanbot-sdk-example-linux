#include <stdio.h>
#include <stdlib.h>

#include <snippets/datacapture/document_data_extractor_scanner.h>
#include <utils/utils.h>

scanbotsdk_error_code_t print_dde_result(scanbotsdk_document_data_extraction_result_t *result) {
    scanbotsdk_error_code_t ec;
    scanbotsdk_generic_document_t *doc = NULL;
    scanbotsdk_document_data_extraction_status_t status;

    ec = scanbotsdk_document_data_extraction_result_get_document(result, &doc);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_document: %d\n", ec); return ec; }

    scanbotsdk_document_data_extraction_result_get_status(result, &status);
    printf("DDE detection status = %d\n", status);
    
    if(doc != NULL) { 
        ec = print_generic_document_fields(doc); 
    }
    return ec;
}

scanbotsdk_error_code_t extract_document_data(scanbotsdk_image_t *image) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    
    scanbotsdk_document_data_extractor_configuration_t *config = NULL;
    scanbotsdk_document_data_extractor_t *extractor = NULL;
    scanbotsdk_document_data_extraction_result_t *result = NULL;

    ec = scanbotsdk_document_data_extractor_configuration_create_with_defaults(&config);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "config_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    // Configure other parameters as needed.

    ec = scanbotsdk_document_data_extractor_create(config, &extractor);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_document_data_extractor_run(extractor, image, &result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_run: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = print_dde_result(result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "print_result: %d: %s\n", ec, error_message(ec)); goto cleanup; }

cleanup:
    scanbotsdk_document_data_extraction_result_free(result);
    scanbotsdk_document_data_extractor_free(extractor);
    scanbotsdk_document_data_extractor_configuration_free(config);
    return ec;
}