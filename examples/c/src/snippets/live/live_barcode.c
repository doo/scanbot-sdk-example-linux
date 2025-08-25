#include <snippets/live/live_barcode.h>
#include <utils/utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------- Frame & Camera structures ----------------
struct frame
{
    int width;
    int height;
    int channels;
    int stride;
    unsigned char *data;
};
typedef struct frame frame;

struct mock_camera
{
    frame mock_frame;
};
typedef struct mock_camera mock_camera;

// ---------------- Camera helpers ----------------
frame *next_frame(mock_camera *camera)
{
    return &camera->mock_frame;
}

scanbotsdk_error_code_t init_camera(const char *path, mock_camera **camera_out)
{
    scanbotsdk_image_t *image = NULL;
    scanbotsdk_path_image_load_options_t *load_options = NULL;

    scanbotsdk_path_image_load_options_create_with_defaults(&load_options);
    scanbotsdk_error_code_t ec = scanbotsdk_image_create_from_path(path, load_options, &image);
    scanbotsdk_path_image_load_options_free(load_options);
    if (ec != SCANBOTSDK_OK)
    {
        fprintf(stderr, "Failed to create image from path: %d\n", ec);
        return ec;
    }

    scanbotsdk_raw_image_t *raw_image = NULL;
    ec = scanbotsdk_image_to_raw_image(image, &raw_image);
    scanbotsdk_image_free(image);
    if (ec != SCANBOTSDK_OK)
    {
        fprintf(stderr, "Failed to obtain raw image: %d\n", ec);
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
    return ec;
}

void free_camera(mock_camera *camera)
{
    if (!camera)
        return;
    free(camera->mock_frame.data);
    free(camera);
}

// ---------------- Frame processing ----------------
scanbotsdk_error_code_t process_frame(scanbotsdk_barcode_scanner_t *scanner, frame *frame, size_t frame_number)
{
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_raw_image_load_options_t *image_load_options = NULL;
    scanbotsdk_image_t *image = NULL;
    scanbotsdk_barcode_scanner_result_t *result = NULL;

    // region create scanbotsdk_image_t from frame

    // Setting image_is_live lets the scanner know that we're running in live mode.
    // In this mode we maintain the highest FPS because we spread the work of scanning barcodes across multiple frames.
    // If you set image_is_live = false, the scanner will run in single-shot mode which is much slower,
    // but has a much higher probability of finding all barcodes in the input image.
    // As an alternative, you can explicitly set the scanner mode,
    // by creating it with e.g. SCANBOTSDK_PROCESSING_MODE_LIVE for live mode instead of SCANBOTSDK_PROCESSING_MODE_AUTO.
    bool image_is_live = true;
    scanbotsdk_raw_image_load_options_create_with_defaults(image_is_live, &image_load_options);

    ec = scanbotsdk_image_create_from_raw_image(
        frame->data,
        frame->width,
        frame->height,
        frame->channels,
        frame->stride,
        image_load_options,
        &image);
    if (ec != SCANBOTSDK_OK) { goto exit; }
    // endregion

    ec = scanbotsdk_barcode_scanner_run(scanner, image, &result);
    if (ec != SCANBOTSDK_OK) { goto exit; }

    bool success = false;
    scanbotsdk_barcode_scanner_result_get_success(result, &success);
    if (!success)
    {
        fprintf(stdout, "No barcodes found in frame %zu\n", frame_number);
    }
    else
    {
        size_t count = 0;
        scanbotsdk_barcode_scanner_result_get_barcodes_size(result, &count);
        fprintf(stdout, "%zu barcodes found in frame %zu\n", count, frame_number);

        scanbotsdk_barcode_item_t **barcodes = malloc(count * sizeof(*barcodes));
        scanbotsdk_barcode_scanner_result_get_barcodes(result, barcodes, count);
        for (size_t i = 0; i < count; i++)
        {
            const char *text = NULL;
            scanbotsdk_barcode_item_get_text(barcodes[i], &text);
            fprintf(stdout, "  Barcode %zu: %s\n", i, text);
        }
        free(barcodes);
    }

exit:
    scanbotsdk_raw_image_load_options_free(image_load_options);
    scanbotsdk_image_free(image);
    scanbotsdk_barcode_scanner_result_free(result);
    return ec;
}

// ---------------- Scanner creation ----------------
scanbotsdk_error_code_t create_barcode_scanner(bool use_tensor_rt, scanbotsdk_barcode_scanner_t **barcode_scanner)
{
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;

    scanbotsdk_barcode_format_common_configuration_t *common_config = NULL;
    scanbotsdk_barcode_scanner_configuration_t *scanner_config = NULL;
    scanbotsdk_accelerator_t *accelerator = NULL;

    scanbotsdk_barcode_format_common_configuration_create_with_defaults(&common_config);
    scanbotsdk_barcode_format_t formats[] = {
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
        SCANBOTSDK_BARCODE_FORMAT_UPC_E};
    ec = scanbotsdk_barcode_format_common_configuration_set_formats(
        common_config, formats, sizeof(formats) / sizeof(formats[0]));
    if (ec != SCANBOTSDK_OK)
        goto exit;

    scanbotsdk_barcode_format_configuration_base_t *base_config = NULL;
    scanbotsdk_barcode_format_common_configuration_as_scanbotsdk_barcode_format_configuration_base(
        common_config, &base_config);

    scanbotsdk_barcode_scanner_configuration_create_with_defaults(&scanner_config);
    scanbotsdk_barcode_scanner_configuration_set_barcode_format_configurations(scanner_config, &base_config, 1);
    scanbotsdk_barcode_scanner_configuration_set_processing_mode(scanner_config, SCANBOTSDK_PROCESSING_MODE_AUTO);

    if (use_tensor_rt)
    {
        scanbotsdk_tensor_rt_accelerator_t *trt = NULL;
        ec = scanbotsdk_tensor_rt_accelerator_create("./", &trt);
        if (ec == SCANBOTSDK_OK)
        {
            scanbotsdk_tensor_rt_accelerator_as_scanbotsdk_accelerator(trt, &accelerator);
            scanbotsdk_barcode_scanner_configuration_set_accelerator(scanner_config, accelerator);
        }
    }

    ec = scanbotsdk_barcode_scanner_create(scanner_config, barcode_scanner);

exit:
    scanbotsdk_barcode_scanner_configuration_free(scanner_config);
    scanbotsdk_accelerator_free(accelerator);
    scanbotsdk_barcode_format_common_configuration_free(common_config);
    return ec;
}

scanbotsdk_error_code_t run_live_barcode_scanner(const char *mock_frame_path, bool use_tensor_rt)
{
    scanbotsdk_barcode_scanner_t *scanner = NULL;
    mock_camera *camera = NULL;
    scanbotsdk_error_code_t ec;

    ec = create_barcode_scanner(use_tensor_rt, &scanner);
    if (ec != SCANBOTSDK_OK)
        return ec;

    ec = init_camera(mock_frame_path, &camera);
    if (ec != SCANBOTSDK_OK)
        return ec;

    for (size_t frame_number = 0; frame_number < 10; frame_number++) 
    {
        frame *f = next_frame(camera);
        ec = process_frame(scanner, f, frame_number);
        if (ec != SCANBOTSDK_OK) { fprintf(stderr, "Failed to process frame %zu: %d: %s\n", frame_number, ec, error_message(ec)); goto exit; }
    }
    

exit:
    scanbotsdk_barcode_scanner_free(scanner);
    free_camera(camera);
    return ec;
}
