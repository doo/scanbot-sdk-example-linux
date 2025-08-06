#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ScanbotSDK.h>

const char *error_message(scanbotsdk_error_code_t ec) {
    if (ec == SCANBOTSDK_ERROR_INVALID_LICENSE_STATUS) {
        return "Invalid license status. Please check your license key.";
    }
    return scanbotsdk_error_message();
}

scanbotsdk_error_code_t process_image(scanbotsdk_barcode_scanner_t *scanner, const char *path) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_path_image_load_options_t *image_load_options = NULL;
    scanbotsdk_image_t *image = NULL;
    scanbotsdk_barcode_scanner_result_t *result = NULL;

    // region create scanbotsdk_image_t from frame
    scanbotsdk_path_image_load_options_create_with_defaults(&image_load_options);

    ec = scanbotsdk_image_create_from_path(path, image_load_options, &image);
    if (ec != SCANBOTSDK_OK) {
        goto exit;
    }
    // endregion

    // region run barcode scanner on the image
    ec = scanbotsdk_barcode_scanner_run(scanner, image, &result);

    if (ec != SCANBOTSDK_OK) {
        goto exit;
    }
    bool success = false;
    scanbotsdk_barcode_scanner_result_get_success(result, &success);
    if (!success) {
        fprintf(stdout, "No barcodes found");
    } else {
        size_t barcodeCount = 0;
        scanbotsdk_barcode_scanner_result_get_barcodes_size(result, &barcodeCount);
        fprintf(stdout, "%zu barcodes found", barcodeCount);
        scanbotsdk_barcode_item_t **barcodes = malloc(barcodeCount * sizeof(scanbotsdk_barcode_item_t *));
        scanbotsdk_barcode_scanner_result_get_barcodes(result, barcodes, barcodeCount);
        for (size_t i = 0; i < barcodeCount; i++) {
            const char *text = NULL;
            scanbotsdk_barcode_item_get_text(barcodes[i], &text);
            fprintf(stdout, "Barcode %zu: %s\n", i, text);
        }
        free(barcodes);
    }
    // endregion

    exit:
    scanbotsdk_path_image_load_options_free(image_load_options);
    scanbotsdk_image_free(image);
    scanbotsdk_barcode_scanner_result_free(result);
    return ec;
}

scanbotsdk_error_code_t create_barcode_scanner(scanbotsdk_barcode_scanner_t **barcode_scanner) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_barcode_scanner_configuration_t *barcode_scanner_configuration = NULL;

    scanbotsdk_barcode_scanner_configuration_create_with_defaults(
            &barcode_scanner_configuration);
    ec = scanbotsdk_barcode_scanner_create(barcode_scanner_configuration, barcode_scanner);
    if (ec != SCANBOTSDK_OK) {
        goto exit;
    }

    exit:
    scanbotsdk_barcode_scanner_configuration_free(barcode_scanner_configuration);
    return ec;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <license_key> <path_to_camera_mock_image>\n", argv[0]);
        return 1;
    }

    const int initialization_timeout_ms = 15000;
    const int deregister_device_timeout_ms = 15000;

    scanbotsdk_barcode_scanner_t *barcode_scanner = NULL;

    const char *license_key = argv[1];
    const char *image_path = argv[2];

    // region initialize Scanbot SDK
    scanbotsdk_init_params_t params;
    params.license_key = license_key;
    params.writeable_path = ".";

    scanbotsdk_error_code_t ec;
    ec = scanbotsdk_initialize(&params);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "Failed to initialize Scanbot SDK: %d: %s\n", ec, error_message(ec));
        goto exit;
    }
    ec = scanbotsdk_wait_for_online_license_check_completion(initialization_timeout_ms);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "Failed to wait for online license check completion: %d: %s\n", ec,
                error_message(ec));
        goto exit;
    }
    // endregion

    // region create barcode scanner
    ec = create_barcode_scanner(&barcode_scanner);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "Failed to create barcode scanner: %d: %s\n", ec, error_message(ec));
        goto exit;
    }
    // endregion

    // region process image
    process_image(barcode_scanner, image_path);
    // endregion

    exit:
    scanbotsdk_barcode_scanner_free(barcode_scanner);
    // If you are not using floating license, it is not necessary to call device deregistration.
    scanbotsdk_deregister_device();
    scanbotsdk_wait_for_device_deregistration_completion(deregister_device_timeout_ms);

    return (ec == SCANBOTSDK_OK) ? 0 : 1;
}

