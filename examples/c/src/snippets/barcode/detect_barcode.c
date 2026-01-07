#include <stdio.h>
#include <stdlib.h>

#include <snippets/barcode/detect_barcode.h>
#include <utils/utils.h>

scanbotsdk_error_code_t print_barcodes_result(scanbotsdk_barcode_scanner_result_t *result) {
    scanbotsdk_error_code_t ec;
    bool success = false; 

    scanbotsdk_barcode_scanner_result_get_success(result, &success);

    if (!success) return fprintf(stdout, "No barcodes found.\n"), SCANBOTSDK_OK;

    size_t count = 0;
    ec = scanbotsdk_barcode_scanner_result_get_barcodes_size(result, &count);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_barcodes_size: %d: %s\n", ec, error_message(ec)); return ec; }

    scanbotsdk_barcode_item_t **barcodes = calloc(count, sizeof(*barcodes));
    ec = scanbotsdk_barcode_scanner_result_get_barcodes(result, barcodes, count);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_barcodes: %d: %s\n", ec, error_message(ec));  goto cleanup; }

    for (size_t i = 0; i < count; ++i) {
        const char *text = NULL;
        scanbotsdk_generic_document_t *doc = NULL;
        
        scanbotsdk_barcode_item_get_text(barcodes[i], &text);
        fprintf(stdout, "  %zu) %s\n", i + 1, text);

        scanbotsdk_barcode_item_get_extracted_document(barcodes[i], &doc);
        if(doc != NULL) { 
            print_generic_document_fields(doc); 
        }
    }

cleanup:
    free(barcodes);
    return ec;
}

scanbotsdk_error_code_t detect_barcode(scanbotsdk_image_t *image) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;

    scanbotsdk_barcode_scanner_configuration_t *config = NULL;
    scanbotsdk_barcode_scanner_t *scanner = NULL;
    scanbotsdk_barcode_scanner_result_t *result = NULL;

    // region create and run barcode scanner
    ec = scanbotsdk_barcode_scanner_configuration_create_with_defaults(&config);
    // Configure other parameters as needed.

    ec = scanbotsdk_barcode_scanner_create(config, &scanner);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_barcode_scanner_run(scanner, image, &result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_run: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    // endregion

    ec = print_barcodes_result(result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "print_result: %d: %s\n", ec, error_message(ec)); goto cleanup; }

cleanup:
    scanbotsdk_barcode_scanner_result_free(result);
    scanbotsdk_barcode_scanner_free(scanner);
    scanbotsdk_barcode_scanner_configuration_free(config);
    return ec;
}

