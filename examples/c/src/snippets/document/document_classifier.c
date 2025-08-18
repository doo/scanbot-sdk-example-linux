#include <stdio.h>
#include <stdlib.h>

#include <snippets/document/document_classifier.h>
#include <utils/utils.h>

scanbotsdk_error_code_t classify_document(scanbotsdk_image_t *image) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;

    scanbotsdk_document_classifier_configuration_t *config = NULL;
    scanbotsdk_document_classifier_result_t *result = NULL;
    scanbotsdk_document_classifier_t *classifier = NULL;
    scanbotsdk_document_scanning_result_t *scanning_result = NULL;
    scanbotsdk_document_detection_result_t *detection_result = NULL;
    scanbotsdk_document_classifier_status_t status;
    scanbotsdk_document_type_t doc_type;
    
    ec = scanbotsdk_document_classifier_configuration_create_with_defaults(&config);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "config_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    // Configure other parameters as needed.

    ec = scanbotsdk_document_classifier_create(config, &classifier);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "classifier_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    
    ec = scanbotsdk_document_classifier_run(classifier, image, &result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "classifier_run: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_document_classifier_result_get_status(result, &status);
    printf("Classifier Status: %d\n", (int)status);

    ec = scanbotsdk_document_classifier_result_get_document_type(result, &doc_type);
    printf("Type: %d\n", (int)doc_type);

cleanup:
    scanbotsdk_document_classifier_free(classifier);
    scanbotsdk_document_scanning_result_free(scanning_result);
    scanbotsdk_document_detection_result_free(detection_result);
    scanbotsdk_document_classifier_result_free(result);
    scanbotsdk_document_classifier_configuration_free(config);
    return ec;
}

