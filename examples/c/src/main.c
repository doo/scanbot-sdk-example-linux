#include <stdio.h>
#include <string.h>

#include <ScanbotSDK.h>

#include <utils/utils.h>

#include <snippets/barcode/detect_barcode.h>    
#include <snippets/barcode/parse_barcode_document.h>    

#include <snippets/datacapture/check_scanner.h>
#include <snippets/datacapture/credit_card_scanner.h>
#include <snippets/datacapture/document_data_extractor_scanner.h>
#include <snippets/datacapture/medical_certificate_scanner.h>
#include <snippets/datacapture/mrz_scanner.h>
#include <snippets/datacapture/text_pattern_scanner.h>
#include <snippets/datacapture/vin_scanner.h>
#include <snippets/datacapture/ocr.h>
#include <snippets/datacapture/mrz_parser.h>

#include <snippets/document/detect_document.h>    
#include <snippets/document/analyse_multi_page.h>
#include <snippets/document/crop_and_analyse.h>
#include <snippets/document/document_classifier.h>

static const int SCANBOTSDK_LICENSE_CHECK_TIMEOUT_MS = 15000;
static const int DEREGISTER_DEVICE_TIMEOUT_MS = 15000;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    char *category   = argv[1];
    char *command    = argv[2];
    char *file_path  = get_flag(argc, argv, "--file");
    char *save_path  = get_flag(argc, argv, "--save");
    char *text_input = get_flag(argc, argv, "--text");
    char *license_arg = get_flag(argc, argv, "--license");

    // TODO Add your Scanbot SDK trial license key here.
    // The SDK and a trial license are available on request via beta@scanbot.io
    char *scanbot_license_key = license_arg ? license_arg : "<SCANBOTSDK-LICENSE>";

    scanbotsdk_init_params_t params = {0};
    params.license_key   = scanbot_license_key;
    params.writeable_path = ".";

    scanbotsdk_error_code_t ec = scanbotsdk_initialize(&params);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "initialize: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    // Wait for the online license check to complete.
    // scanbotsdk_initialize() is non-blocking and triggers the license validation
    // asynchronously in the background. Without waiting here, the license status
    // may still be unknown or incomplete, which can lead to incorrect assumptions
    // about license validity.
    ec = scanbotsdk_wait_for_online_license_check_completion(SCANBOTSDK_LICENSE_CHECK_TIMEOUT_MS);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "license_wait: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    scanbotsdk_image_t *image = NULL;

    if (strcmp(category, "scan") == 0) {
        if (!file_path) { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; goto cleanup; }

        ec = load_image_from_path(file_path, &image);
        if (ec != SCANBOTSDK_OK) goto cleanup;

        if      (strcmp(command, "barcode")            == 0) ec = detect_barcode(image);
        else if (strcmp(command, "document")           == 0) ec = detect_document(image);
        else if (strcmp(command, "check")              == 0) ec = detect_check(image);
        else if (strcmp(command, "credit_card")        == 0) ec = detect_credit_card(image);
        else if (strcmp(command, "document_extractor") == 0) ec = extract_document_data(image);
        else if (strcmp(command, "medical_certificate")== 0) ec = detect_medical_certificate(image);
        else if (strcmp(command, "mrz")                == 0) ec = detect_mrz(image);
        else if (strcmp(command, "ocr")                == 0) ec = ocr_run(image);
        else if (strcmp(command, "text_pattern")       == 0) ec = detect_text_pattern(image);
        else if (strcmp(command, "vin")                == 0) ec = detect_vin(image);
        else { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; }
    }
    else if (strcmp(category, "classify") == 0) {
        if (!file_path) { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; goto cleanup; }

        ec = load_image_from_path(file_path, &image);
        if (ec != SCANBOTSDK_OK) goto cleanup;

        if      (strcmp(command, "document") == 0) ec = classify_document(image);
        else { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; }
    }
    else if (strcmp(category, "analyse") == 0) {
        if (!file_path) { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; goto cleanup; }

        if (strcmp(command, "analyse_multi_page") == 0) {
            ec = analyse_multi_page(file_path);
        }
        else if (strcmp(command, "crop_analyze") == 0) {
            ec = load_image_from_path(file_path, &image);
            if (ec != SCANBOTSDK_OK) goto cleanup;

            ec = crop_and_analyse(image, save_path);
        }
        else { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; }
    }
    else if (strcmp(category, "parse") == 0) {
        if (!text_input) { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; goto cleanup; }

        if      (strcmp(command, "mrz")         == 0) ec = parse_mrz(text_input);
        else if (strcmp(command, "barcode_doc") == 0) ec = parse_barcode_document(text_input);
        else { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; }
    }
    else { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; }

cleanup:
    scanbotsdk_image_free(image);
    // For floating licenses it is highly recommended to call device deregistration.
    // This releases the license slot on the server, allowing other devices to activate.
    // If scanbotsdk_deregister_device() is not called, the license will remain occupied
    // by this device for some time that corresponds to the interval between license
    // checks (usually about 15 minutes). During this period other devices may not be
    // able to activate and "device slots exceeded" errors can occur.
    scanbotsdk_deregister_device();

    // Wait for deregistration to complete to ensure the slot is actually released.
    // If this call returns SCANBOTSDK_ERROR_TIMEOUT, the deregistration did not
    // finish within the timeout (e.g. due to network issues), and the license may remain
    // locked on the server.
    scanbotsdk_wait_for_device_deregistration_completion(DEREGISTER_DEVICE_TIMEOUT_MS);
    return (ec == SCANBOTSDK_OK) ? 0 : 1;
}