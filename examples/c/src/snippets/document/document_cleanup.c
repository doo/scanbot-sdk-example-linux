#include <stdio.h>

#include <snippets/document/document_cleanup.h>
#include <utils/utils.h>

scanbotsdk_error_code_t document_cleanup(
    scanbotsdk_image_t *image,
    scanbotsdk_image_t *mask,
    const char *save_path
) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_document_cleanup_configuration_t *configuration = NULL;
    scanbotsdk_document_cleanup_t *cleanup = NULL;
    scanbotsdk_document_cleanup_result_t *result = NULL;
    scanbotsdk_document_cleanup_status_t status;
    const char *status_str = NULL;

    scanbotsdk_image_t *cleaned_image = NULL;
    scanbotsdk_image_info_t *original_info = NULL;
    scanbotsdk_image_info_t *cleaned_info = NULL;
    int original_width = 0;
    int original_height = 0;
    int cleaned_width = 0;
    int cleaned_height = 0;

    scanbotsdk_save_image_options_t *save_options = NULL;

    ec = scanbotsdk_document_cleanup_configuration_create_with_defaults(&configuration);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "document_cleanup_configuration_create_with_defaults: %d: %s\n", ec, error_message(ec));
        goto cleanup;
    }

    ec = scanbotsdk_document_cleanup_create(image, configuration, &cleanup);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "document_cleanup_create: %d: %s\n", ec, error_message(ec));
        goto cleanup;
    }

    ec = scanbotsdk_document_cleanup_run(cleanup, mask, &result);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "document_cleanup_run: %d: %s\n", ec, error_message(ec));
        goto cleanup;
    }

    ec = scanbotsdk_document_cleanup_result_get_status(result, &status);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "document_cleanup_result_get_status: %d: %s\n", ec, error_message(ec));
        goto cleanup;
    }

    ec = scanbotsdk_document_cleanup_status_t_to_string(status, &status_str);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "document_cleanup_status_t_to_string: %d: %s\n", ec, error_message(ec));
        goto cleanup;
    }

    printf("Document cleanup status: %s\n", status_str);

    ec = scanbotsdk_image_info(image, &original_info);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "image_info(original): %d: %s\n", ec, error_message(ec));
        goto cleanup;
    }

    ec = scanbotsdk_document_cleanup_result_get_image(result, &cleaned_image);
    if (ec != SCANBOTSDK_OK || cleaned_image == NULL) {
        fprintf(stderr, "document_cleanup_result_get_image: %d: %s\n", ec, error_message(ec));
        goto cleanup;
    }

    ec = scanbotsdk_image_info(cleaned_image, &cleaned_info);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "image_info(cleaned): %d: %s\n", ec, error_message(ec));
        goto cleanup;
    }

    ec = scanbotsdk_image_info_get_width(original_info, &original_width);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "image_info_get_width(original): %d: %s\n", ec, error_message(ec)); goto cleanup; }
    ec = scanbotsdk_image_info_get_height(original_info, &original_height);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "image_info_get_height(original): %d: %s\n", ec, error_message(ec)); goto cleanup; }
    ec = scanbotsdk_image_info_get_width(cleaned_info, &cleaned_width);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "image_info_get_width(cleaned): %d: %s\n", ec, error_message(ec)); goto cleanup; }
    ec = scanbotsdk_image_info_get_height(cleaned_info, &cleaned_height);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "image_info_get_height(cleaned): %d: %s\n", ec, error_message(ec)); goto cleanup; }

    printf("Original WxH: %dx%d\n", original_width, original_height);
    printf("Cleaned  WxH: %dx%d\n", cleaned_width, cleaned_height);

    if (save_path && save_path[0] != '\0') {
        ec = scanbotsdk_save_image_options_create_with_defaults(&save_options);
        if (ec != SCANBOTSDK_OK) {
            fprintf(stderr, "save_image_options_create_with_defaults: %d: %s\n", ec, error_message(ec));
            goto cleanup;
        }

        ec = scanbotsdk_image_save(cleaned_image, save_path, save_options);
        if (ec != SCANBOTSDK_OK) {
            fprintf(stderr, "image_save: %d: %s\n", ec, error_message(ec));
            goto cleanup;
        }
        printf("Saved cleaned image to: %s\n", save_path);
    }

cleanup:
    scanbotsdk_save_image_options_free(save_options);
    scanbotsdk_image_info_free(cleaned_info);
    scanbotsdk_image_info_free(original_info);
    scanbotsdk_document_cleanup_result_free(result);
    scanbotsdk_document_cleanup_free(cleanup);
    scanbotsdk_document_cleanup_configuration_free(configuration);
    return ec;
}
