#include <stdio.h>
#include <snippets/document/crop_and_analyse.h>
#include <utils/utils.h>

scanbotsdk_error_code_t detect_document_cropped(
    scanbotsdk_image_t *image,
    scanbotsdk_image_t **cropped_image_out
) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_document_scanner_configuration_t *config = NULL;
    scanbotsdk_document_scanner_t *scanner = NULL;
    scanbotsdk_document_scanning_result_t *result = NULL;
    scanbotsdk_document_detection_result_t *detect_result = NULL;
    scanbotsdk_document_detection_status_t status;

    scanbotsdk_image_t *cropped_image = NULL;
    scanbotsdk_point_t *points = NULL;
    
    // region Setup scanner
    ec = scanbotsdk_document_scanner_configuration_create_with_defaults(&config);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "config_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    // Configure other parameters as needed.

    ec = scanbotsdk_document_scanner_create(config, &scanner);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    // endregion

    // region Scan document
    ec = scanbotsdk_document_scanner_scan(scanner, image, &result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_run: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_document_scanning_result_get_detection_result(result, &detect_result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_detection_result: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    scanbotsdk_document_detection_result_get_status(detect_result, &status);
    printf("Document Detection Status = %d\n", (int)status);

    scanbotsdk_document_detection_result_get_points(detect_result, &points);
    printf("Points x:%d  y:%d\n", points->x, points->y);
    // endregion

    // region Crop and acquire image
    ec = scanbotsdk_document_scanning_result_get_cropped_image(result, &cropped_image);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "get_cropped_image: %d: %s\n", ec, error_message(ec));
        goto cleanup;
    }

    const char *unique_id = NULL;
    ec = scanbotsdk_image_get_unique_id(cropped_image, &unique_id);
    if (ec != SCANBOTSDK_OK || unique_id == NULL) { fprintf(stderr, "Failed to get cropped image id: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    printf("Cropped image unique_id: %s\n", unique_id);

    /**
     * You must acquire a reference to the cropped image here.
     * Otherwise, when `scanbotsdk_document_scanning_result_free(result)` is called
     * in the cleanup section, it will release the internal image object and
     * invalidate the pointer created from `unique_id`.
     */
    ec = scanbotsdk_image_acquire_reference(unique_id);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "image_acquire_reference: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_image_create_from_id(unique_id, cropped_image_out);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "image_create_from_id: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    // endregion

cleanup:
    scanbotsdk_document_scanner_configuration_free(config);
    scanbotsdk_document_scanner_free(scanner);
    scanbotsdk_document_scanning_result_free(result);
    return ec;
}

scanbotsdk_error_code_t save_cropped_image(
    scanbotsdk_image_t *cropped_image,
    char *save_path
) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_save_image_options_t *save_options = NULL;

    ec = scanbotsdk_save_image_options_create_with_defaults(&save_options);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "save_image_options: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    printf("Saving cropped image to: %s\n", save_path);

    scanbotsdk_image_save(cropped_image, save_path, save_options);

cleanup:
    scanbotsdk_save_image_options_free(save_options);
    return ec;
}

void print_result(scanbotsdk_document_quality_analyzer_result_t *result) {
    const char* quality_str[] = { "Very poor", "Poor", "Reasonable", "Good", "Excellent" };

    bool document_found = false;
    scanbotsdk_document_quality_t *quality = NULL;

    scanbotsdk_document_quality_analyzer_result_get_document_found(result, &document_found);
    scanbotsdk_document_quality_analyzer_result_get_quality(result, &quality);

    printf("Document detection: %s\n", document_found ? "Found" : "Not found");

    if (quality && *quality >= SCANBOTSDK_DOCUMENT_QUALITY_VERY_POOR && *quality <= SCANBOTSDK_DOCUMENT_QUALITY_EXCELLENT) {
        printf("Document quality: %s (%d)\n", quality_str[*quality], *quality);
    } else {
        printf("Document quality: Unknown\n");
    }
}

static scanbotsdk_error_code_t analyse_document_quality(
    scanbotsdk_image_t *cropped_image
) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_document_quality_analyzer_configuration_t *analyse_config = NULL;
    scanbotsdk_document_quality_analyzer_t *analyzer = NULL;
    scanbotsdk_document_quality_analyzer_result_t *analyse_result = NULL;

    // region Setup analyzer
    ec = scanbotsdk_document_quality_analyzer_configuration_create_with_defaults(&analyse_config);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "analyse_config_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    // Configure other parameters as needed.

    ec = scanbotsdk_document_quality_analyzer_create(analyse_config, &analyzer);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "analyzer_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    // endregion

    // region Analyse quality
    ec = scanbotsdk_document_quality_analyzer_run(analyzer, cropped_image, &analyse_result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "analyzer_run: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    print_result(analyse_result);
    // endregion

cleanup:
    scanbotsdk_document_quality_analyzer_result_free(analyse_result);
    scanbotsdk_document_quality_analyzer_free(analyzer);
    scanbotsdk_document_quality_analyzer_configuration_free(analyse_config);
    return ec;
}

scanbotsdk_error_code_t crop_and_analyse(
    scanbotsdk_image_t *image,
    char *save_path
) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_image_t *cropped_image = NULL;

    ec = detect_document_cropped(image, &cropped_image);
    if (ec != SCANBOTSDK_OK) goto cleanup;

    // Save if a non-empty path is provided
    if (save_path && save_path[0] != '\0') {
        ec = save_cropped_image(cropped_image, save_path);
        if (ec != SCANBOTSDK_OK) goto cleanup;
    }

    ec = analyse_document_quality(cropped_image);
    if (ec != SCANBOTSDK_OK) goto cleanup;

cleanup:
    scanbotsdk_image_free(cropped_image);
    return ec;
}
