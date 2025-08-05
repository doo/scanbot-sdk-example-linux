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


struct frame {
    int width;
    int height;
    int channels;
    int stride;
    unsigned char *data;
};
typedef struct frame frame;

struct mock_camera {
    frame mock_frame;
};
typedef struct mock_camera mock_camera;

frame *next_frame(mock_camera *camera) {
    return &camera->mock_frame;
}

mock_camera *init_camera(const char *path) {
    scanbotsdk_image_t *image = NULL;
    scanbotsdk_path_image_load_options_t *load_options = NULL;
    scanbotsdk_path_image_load_options_create_with_defaults(&load_options);
    scanbotsdk_error_code_t ec = scanbotsdk_image_create_from_path(path, load_options, &image);
    scanbotsdk_path_image_load_options_free(load_options);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "Failed to create image from path: %d: %s\n", ec, error_message(ec));
        return NULL;
    }
    scanbotsdk_raw_image_t *raw_image = NULL;
    ec = scanbotsdk_image_to_raw_image(image, &raw_image);
    scanbotsdk_image_free(image);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "Failed to obtain raw image: %d: %s\n", ec, error_message(ec));
        return NULL;
    }
    frame f;

    scanbotsdk_raw_image_get_width(raw_image, &f.width);

    scanbotsdk_raw_image_get_height(raw_image, &f.height);
    scanbotsdk_raw_image_get_channels(raw_image, &f.channels);
    scanbotsdk_raw_image_get_stride(raw_image, &f.stride);

    void *data;
    scanbotsdk_raw_image_get_data(raw_image, &data);

    size_t data_size = f.stride * f.height;
    f.data = malloc(data_size);
    memcpy(f.data, data, data_size);
    scanbotsdk_raw_image_free(raw_image);
    mock_camera *camera = malloc(sizeof(mock_camera));
    camera->mock_frame = f;
    return camera;
}

void free_camera(mock_camera *camera) {
    if (!camera) {
        return;
    }
    free(camera->mock_frame.data);
    free(camera);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <license_key> <path_to_camera_mock_image>\n", argv[0]);
        return 1;
    }

    const char *license_key = argv[1];
    const char *mock_frame_path = argv[2];

    // region Initialize Scanbot SDK
    scanbotsdk_init_params_t params;
    params.license_key = license_key;
    params.writeable_path = ".";

    scanbotsdk_error_code_t ec;
    ec = scanbotsdk_initialize(&params);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "Failed to initialize Scanbot SDK: %d: %s\n", ec, error_message(ec));
        return 1;
    }
    // endregion

    // region Create Barcode Scanner Configuration
    scanbotsdk_barcode_format_common_configuration_t *common_format_configuration = NULL;
    ec = scanbotsdk_barcode_format_common_configuration_create_with_defaults(
            &common_format_configuration);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "Failed to create common format configuration: %d: %s\n", ec,
                error_message(ec));
        return 1;
    }
    scanbotsdk_barcode_format_t common_formats[] = {
            SCANBOTSDK_BARCODE_FORMAT_AZTEC,
            SCANBOTSDK_BARCODE_FORMAT_CODABAR,
            SCANBOTSDK_BARCODE_FORMAT_CODE_39,
            SCANBOTSDK_BARCODE_FORMAT_CODE_93,
            SCANBOTSDK_BARCODE_FORMAT_CODE_128,
            SCANBOTSDK_BARCODE_FORMAT_DATA_MATRIX,
            SCANBOTSDK_BARCODE_FORMAT_DATABAR,
            SCANBOTSDK_BARCODE_FORMAT_DATABAR_EXPANDED,
            SCANBOTSDK_BARCODE_FORMAT_DATABAR_LIMITED,
            SCANBOTSDK_BARCODE_FORMAT_EAN_13,
            SCANBOTSDK_BARCODE_FORMAT_EAN_8,
            SCANBOTSDK_BARCODE_FORMAT_ITF,
            SCANBOTSDK_BARCODE_FORMAT_PDF_417,
            SCANBOTSDK_BARCODE_FORMAT_QR_CODE,
            SCANBOTSDK_BARCODE_FORMAT_UPC_A,
            SCANBOTSDK_BARCODE_FORMAT_UPC_E
    };
    ec = scanbotsdk_barcode_format_common_configuration_set_formats(common_format_configuration, common_formats,
                                                                    sizeof(common_formats) / sizeof(common_formats[0]));
    if (ec != SCANBOTSDK_OK) {
        scanbotsdk_barcode_format_common_configuration_free(common_format_configuration);
        fprintf(stderr, "Failed to set formats in common format configuration: %d: %s\n", ec,
                error_message(ec));
        return 1;
    }

    scanbotsdk_barcode_format_configuration_base_t *common_format_configuration_as_base = NULL;
    scanbotsdk_barcode_format_common_configuration_as_scanbotsdk_barcode_format_configuration_base(
            common_format_configuration, &common_format_configuration_as_base);

    scanbotsdk_barcode_scanner_configuration_t *barcode_scanner_configuration;
    ec = scanbotsdk_barcode_scanner_configuration_create_with_defaults(
            &barcode_scanner_configuration);

    if (ec != SCANBOTSDK_OK) {
        scanbotsdk_barcode_format_configuration_base_free(common_format_configuration_as_base);
        fprintf(stderr, "Failed to create barcode scanner configuration: %d: %s\n", ec,
                error_message(ec));
        return 1;
    }
    ec = scanbotsdk_barcode_scanner_configuration_set_barcode_format_configurations(
            barcode_scanner_configuration, &common_format_configuration_as_base, 1);
    scanbotsdk_barcode_format_configuration_base_free(common_format_configuration_as_base);
    if (ec != SCANBOTSDK_OK) {
        scanbotsdk_barcode_scanner_configuration_free(barcode_scanner_configuration);
        fprintf(stderr, "Failed to set barcode format configurations: %d: %s\n", ec,
                error_message(ec));
        return 1;
    }
    // endregion

    // region Create Barcode Scanner
    scanbotsdk_barcode_scanner_t *barcode_scanner = NULL;
    ec = scanbotsdk_barcode_scanner_create(barcode_scanner_configuration, &barcode_scanner);
    scanbotsdk_barcode_scanner_configuration_free(barcode_scanner_configuration);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "Failed to create barcode scanner: %d: %s\n", ec,
                error_message(ec));
        return 1;
    }
    // endregion

    // region create mock camera that always returns the same frame
    mock_camera *camera = init_camera(mock_frame_path);
    if (!camera) {
        scanbotsdk_barcode_scanner_free(barcode_scanner);
        fprintf(stderr, "Failed to initialize mock camera with path: %s\n", mock_frame_path);
        return 1;
    }
    // endregion

    for (size_t frameNumber = 0; frameNumber < 10; frameNumber++) {
        fprintf(stdout, "Processing frame %zu\n", frameNumber);

        // region create scanbotsdk_image_t from mock camera frame
        frame *frame = next_frame(camera);
        bool imageIsLive = true;
        scanbotsdk_raw_image_load_options_t *load_options = NULL;
        scanbotsdk_raw_image_load_options_create_with_defaults(imageIsLive, &load_options);
        scanbotsdk_image_t *image = NULL;
        ec = scanbotsdk_image_create_from_raw_image(
                frame->data, frame->width, frame->height, frame->channels, frame->stride, load_options, &image);
        scanbotsdk_raw_image_load_options_free(load_options);
        if (ec != SCANBOTSDK_OK) {
            scanbotsdk_barcode_scanner_free(barcode_scanner);
            free_camera(camera);
            fprintf(stderr, "Failed to create image from raw data: %d: %s\n", ec,
                    error_message(ec));
            return 1;
        }
        // endregion

        // region run barcode scanner on the image
        scanbotsdk_barcode_scanner_result_t *result = NULL;
        ec = scanbotsdk_barcode_scanner_run(barcode_scanner, image, &result);
        scanbotsdk_image_free(image);
        if (ec != SCANBOTSDK_OK) {
            scanbotsdk_barcode_scanner_free(barcode_scanner);
            free_camera(camera);
            fprintf(stderr, "Failed to run barcode scanner: %d: %s\n", ec,
                    error_message(ec));
            return 1;
        }
        bool success = false;
        scanbotsdk_barcode_scanner_result_get_success(result, &success);
        if (!success) {
            fprintf(stdout, "No barcodes found in frame %zu\n", frameNumber);
        } else {
            size_t barcodeCount = 0;
            scanbotsdk_barcode_scanner_result_get_barcodes_size(result, &barcodeCount);
            fprintf(stdout, "%zu barcodes found in frame %zu\n", barcodeCount, frameNumber);
            scanbotsdk_barcode_item_t **barcodes = malloc(barcodeCount * sizeof(scanbotsdk_barcode_item_t *));
            scanbotsdk_barcode_scanner_result_get_barcodes(result, barcodes, barcodeCount);
            for (size_t i = 0; i < barcodeCount; i++) {
                const char *text = NULL;
                scanbotsdk_barcode_item_get_text(barcodes[i], &text);
                fprintf(stdout, "Barcode %zu: %s\n", i, text);
            }
            free(barcodes);
        }
        scanbotsdk_barcode_scanner_result_free(result);
        // endregion
    }
    scanbotsdk_barcode_scanner_free(barcode_scanner);
    free_camera(camera);

    return 0;
}

