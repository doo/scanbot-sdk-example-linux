#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
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

scanbotsdk_error_code_t init_camera(const char *path, mock_camera **camera_out) {
    scanbotsdk_image_t *image = NULL;
    scanbotsdk_path_image_load_options_t *load_options = NULL;
    scanbotsdk_path_image_load_options_create_with_defaults(&load_options);
    scanbotsdk_error_code_t ec = scanbotsdk_image_create_from_path(path, load_options, &image);
    scanbotsdk_path_image_load_options_free(load_options);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "Failed to create image from path: %d: %s\n", ec, error_message(ec));
        return ec;
    }
    scanbotsdk_raw_image_t *raw_image = NULL;
    ec = scanbotsdk_image_to_raw_image(image, &raw_image);
    scanbotsdk_image_free(image);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "Failed to obtain raw image: %d: %s\n", ec, error_message(ec));
        return ec;
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
    *camera_out = camera;
    return SCANBOTSDK_OK;
}

void free_camera(mock_camera *camera) {
    if (!camera) {
        return;
    }
    free(camera->mock_frame.data);
    free(camera);
}

scanbotsdk_error_code_t process_frame(scanbotsdk_barcode_scanner_t *scanner, frame *frame, size_t frame_number) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_raw_image_load_options_t *image_load_options = NULL;
    scanbotsdk_image_t *image = NULL;
    scanbotsdk_barcode_scanner_result_t *result = NULL;

    // region create scanbotsdk_image_t from frame
    bool image_is_live = true;
    scanbotsdk_raw_image_load_options_create_with_defaults(image_is_live, &image_load_options);

    ec = scanbotsdk_image_create_from_raw_image(
            frame->data, frame->width, frame->height, frame->channels, frame->stride, image_load_options, &image);
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
        fprintf(stdout, "No barcodes found in frame %zu\n", frame_number);
    } else {
        size_t barcodeCount = 0;
        scanbotsdk_barcode_scanner_result_get_barcodes_size(result, &barcodeCount);
        fprintf(stdout, "%zu barcodes found in frame %zu\n", barcodeCount, frame_number);
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
    scanbotsdk_raw_image_load_options_free(image_load_options);
    scanbotsdk_image_free(image);
    scanbotsdk_barcode_scanner_result_free(result);
    return ec;
}

scanbotsdk_error_code_t create_barcode_scanner(bool use_tensor_rt, scanbotsdk_barcode_scanner_t **barcode_scanner) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;

    scanbotsdk_barcode_format_common_configuration_t *common_format_configuration = NULL;
    scanbotsdk_accelerator_t* accelerator = NULL;
    scanbotsdk_barcode_scanner_configuration_t *barcode_scanner_configuration = NULL;

    // region create barcode scanner configuration
    scanbotsdk_barcode_format_common_configuration_create_with_defaults(
            &common_format_configuration);
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
        goto exit;
    }

    scanbotsdk_barcode_format_configuration_base_t *common_format_configuration_as_base = NULL;
    scanbotsdk_barcode_format_common_configuration_as_scanbotsdk_barcode_format_configuration_base(
            common_format_configuration, &common_format_configuration_as_base);


    scanbotsdk_barcode_scanner_configuration_create_with_defaults(
            &barcode_scanner_configuration);
    ec = scanbotsdk_barcode_scanner_configuration_set_barcode_format_configurations(
            barcode_scanner_configuration, &common_format_configuration_as_base, 1);
    if (ec != SCANBOTSDK_OK) {
        goto exit;
    }

    if (use_tensor_rt) {
        scanbotsdk_tensor_rt_accelerator_t *tensor_rt_accelerator = NULL;
        ec = scanbotsdk_tensor_rt_accelerator_create("./", &tensor_rt_accelerator);
        if (ec != SCANBOTSDK_OK) {
            goto exit;
        }
        scanbotsdk_tensor_rt_accelerator_as_scanbotsdk_accelerator(tensor_rt_accelerator, &accelerator);
        ec = scanbotsdk_barcode_scanner_configuration_set_accelerator(barcode_scanner_configuration, accelerator);
        if (ec != SCANBOTSDK_OK) {
            goto exit;
        }
    }
    // endregion

    // region create barcode scanner
    ec = scanbotsdk_barcode_scanner_create(barcode_scanner_configuration, barcode_scanner);
    if (ec != SCANBOTSDK_OK) {
        goto exit;
    }
    // endregion

    exit:
    scanbotsdk_barcode_scanner_configuration_free(barcode_scanner_configuration);
    scanbotsdk_accelerator_free(accelerator);
    scanbotsdk_barcode_format_common_configuration_free(common_format_configuration);
    return ec;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <license_key> <path_to_camera_mock_image>\n", argv[0]);
        return 1;
    }

    bool use_tensor_rt = false;
    if (argc > 3) {
        if (strcmp(argv[3], "--use-tensor-rt") == 0) {
            use_tensor_rt = true;
        }
    }

    const int initialization_timeout_ms = 15000;
    const int deregister_device_timeout_ms = 15000;

    scanbotsdk_barcode_scanner_t *barcode_scanner = NULL;
    mock_camera *camera = NULL;

    const char *license_key = argv[1];
    const char *mock_frame_path = argv[2];

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
    ec = create_barcode_scanner(use_tensor_rt, &barcode_scanner);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "Failed to create barcode scanner: %d: %s\n", ec, error_message(ec));
        goto exit;
    }
    // endregion

    // region create mock camera that always returns the same frame
    ec = init_camera(mock_frame_path, &camera);
    if (ec != SCANBOTSDK_OK) {
        fprintf(stderr, "Failed to initialize mock camera with image: %s\n", mock_frame_path);
        goto exit;
    }
    // endregion

    // region frame processing loop
    for (size_t frame_number = 0; frame_number < 10; frame_number++) {
        fprintf(stdout, "Processing frame %zu\n", frame_number);

        frame *frame = next_frame(camera);
        ec = process_frame(barcode_scanner, frame, frame_number);
        if (ec != SCANBOTSDK_OK) {
            fprintf(stderr, "Failed to process frame %zu: %d: %s\n", frame_number, ec, error_message(ec));
            goto exit;
        }
    }
    // endregion

    exit:
    scanbotsdk_barcode_scanner_free(barcode_scanner);
    free_camera(camera);
    // If you are not using floating license, it is not necessary to call device deregistration.
    scanbotsdk_deregister_device();
    scanbotsdk_wait_for_device_deregistration_completion(deregister_device_timeout_ms);

    return (ec == SCANBOTSDK_OK) ? 0 : 1;
}

