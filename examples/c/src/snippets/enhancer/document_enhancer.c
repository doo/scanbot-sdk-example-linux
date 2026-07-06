#include <stdio.h>
#include <stdlib.h>

#include <snippets/enhancer/document_enhancer.h>
#include <utils/utils.h>

scanbotsdk_error_code_t enhance_document(scanbotsdk_image_t *image) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;

    scanbotsdk_document_straightening_result_t *result = NULL;
    scanbotsdk_document_straightening_parameters_t *straightening_params = NULL;
    scanbotsdk_document_enhancer_t *enhancer = NULL;
    scanbotsdk_image_t *straightened_image = NULL;

    scanbotsdk_aspect_ratio_t *aspect_ratios[4] = {0};

    ec = scanbotsdk_aspect_ratio_create(5.0, 7.0, &aspect_ratios[0]);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "aspect_ratio_create(5:7): %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_aspect_ratio_create(1.0, 1.0, &aspect_ratios[1]);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "aspect_ratio_create(1:1): %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_aspect_ratio_create(16.0, 9.0, &aspect_ratios[2]);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "aspect_ratio_create(16:9): %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_aspect_ratio_create(3.0, 4.0, &aspect_ratios[3]);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "aspect_ratio_create(3:4): %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_document_straightening_parameters_create(
        SCANBOTSDK_DOCUMENT_STRAIGHTENING_MODE_STRAIGHTEN,
        aspect_ratios,
        4,
        &straightening_params
    );
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "document_straightening_parameters_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_document_enhancer_create(&enhancer);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "document_enhancer_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_document_enhancer_straighten(
        enhancer,
        image,
        straightening_params,
        NULL,
        0,
        &result
    );
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "document_enhancer_straighten: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_document_straightening_result_get_straightened_image(result, &straightened_image);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "document_straightening_result_get_straightened_image: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    if (straightened_image == NULL) {
        fprintf(stderr, "No straightened image returned.\n");
    }

    /* straightened_image can be saved or processed further here */

cleanup:
    scanbotsdk_document_enhancer_free(enhancer);
    scanbotsdk_document_straightening_result_free(result);
    scanbotsdk_document_straightening_parameters_free(straightening_params);

    scanbotsdk_aspect_ratio_free(aspect_ratios[0]);
    scanbotsdk_aspect_ratio_free(aspect_ratios[1]);
    scanbotsdk_aspect_ratio_free(aspect_ratios[2]);
    scanbotsdk_aspect_ratio_free(aspect_ratios[3]);

    return ec;
}