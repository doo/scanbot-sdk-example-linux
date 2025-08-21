#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <snippets/document/detect_document.h>
#include <utils/utils.h>

void print_analyzer_result(scanbotsdk_document_quality_analyzer_result_t *result) {
    char* quality_str[] = { "Very poor", "Poor", "Reasonable", "Good", "Excellent"};

    bool document_found;
    scanbotsdk_document_quality_t *quality = NULL;

    scanbotsdk_document_quality_analyzer_result_get_document_found(result, &document_found);
    scanbotsdk_document_quality_analyzer_result_get_quality(result, &quality);

    printf("Document detection: %s\n", document_found ? "Found" : "Not found");

    if (!quality)
    {
        printf("Document quality: %s\n", quality_str[*quality]);
    } else {
        printf("No document found.\n");
    }
}

scanbotsdk_error_code_t analyse_multi_page(char* path) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;

    scanbotsdk_random_access_source_t* access_source = NULL;
    scanbotsdk_multi_page_image_extractor_t *extractor = NULL;
    scanbotsdk_page_extraction_result_t *page_result = NULL;
    
    scanbotsdk_document_quality_analyzer_configuration_t *config = NULL;
    scanbotsdk_document_quality_analyzer_t *analyzer = NULL;

    scanbotsdk_extracted_page_t **pages = NULL;
    scanbotsdk_extracted_image_t **images = NULL;
    size_t page_count = 0;

    // region Create sources & extractor
    ec = scanbotsdk_random_access_source_create_from_path(path, &access_source);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "create_from_path: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_multi_page_image_extractor_create(&extractor);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "extractor_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_multi_page_image_extractor_run(extractor, access_source, &page_result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "extractor_run: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    // endregion

    // region Get pages
    ec = scanbotsdk_page_extraction_result_get_pages_size(page_result, &page_count);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_pages_size: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    if (page_count == 0) { printf("No pages found.\n"); goto cleanup; }

    pages = calloc(page_count, sizeof(*pages));
    if (!pages) { fprintf(stderr, "calloc pages failed\n"); ec = SCANBOTSDK_ERROR_OUT_OF_MEMORY; goto cleanup; }

    ec = scanbotsdk_page_extraction_result_get_pages(page_result, pages, page_count);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_pages: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    // endregion

    // region Setup analyzer
    ec = scanbotsdk_document_quality_analyzer_configuration_create_with_defaults(&config);
    ec = scanbotsdk_document_quality_analyzer_create(config, &analyzer);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "analyzer_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    // endregion

    // region Process pages
    for (size_t i = 0; i < page_count; i++) {
        size_t page_images_count = 0;

        ec = scanbotsdk_extracted_page_get_images_size(pages[i], &page_images_count);
        if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_images_size: %d: %s\n", ec, error_message(ec)); goto cleanup; }

        if (page_images_count == 0) {
            printf("Page %zu: no images.\n", i);
            continue;
        }

        images = calloc(page_images_count, sizeof(*images));
        if (!images) { fprintf(stderr, "calloc images failed\n"); ec = SCANBOTSDK_ERROR_OUT_OF_MEMORY; goto cleanup; }

        ec = scanbotsdk_extracted_page_get_images(pages[i], images, page_images_count);
        if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_images: %d: %s\n", ec, error_message(ec)); goto cleanup; }

        for (size_t j = 0; j < page_images_count; j++) {
            scanbotsdk_image_t *img = NULL;
            ec = scanbotsdk_extracted_image_get_image(images[j], &img);
            if (ec != SCANBOTSDK_OK) { fprintf(stderr, "extracted_image_get_image: %d: %s\n", ec, error_message(ec)); goto cleanup; }

            scanbotsdk_document_quality_analyzer_result_t *analyse_result = NULL;
            ec = scanbotsdk_document_quality_analyzer_run(analyzer, img, &analyse_result);
            if (ec != SCANBOTSDK_OK) { fprintf(stderr, "analyzer_run: %d: %s\n", ec, error_message(ec)); scanbotsdk_image_free(img); goto cleanup; }

            print_analyzer_result(analyse_result);

            scanbotsdk_document_quality_analyzer_result_free(analyse_result);
        }
    }
    // endregion

cleanup:
    free(pages);
    free(images);

    scanbotsdk_document_quality_analyzer_free(analyzer);
    scanbotsdk_document_quality_analyzer_configuration_free(config);

    scanbotsdk_page_extraction_result_free(page_result);
    scanbotsdk_multi_page_image_extractor_free(extractor);
    scanbotsdk_random_access_source_free(access_source);

    return ec;
}
