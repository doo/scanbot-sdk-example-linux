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
#include <snippets/document/analyze_multi_page.h>
#include <snippets/document/crop_and_analyze.h>
#include <snippets/document/document_classifier.h>

#include <snippets/live/live_barcode.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    char *category   = argv[1];
    char *command    = argv[2];
    const char *file_path  = get_flag(argc, argv, "--file");
    const char *save_path  = get_flag(argc, argv, "--save");
    const char *text_input = get_flag(argc, argv, "--text");
    const char *license_arg = get_flag(argc, argv, "--license");
    bool use_tensor_rt = get_flag(argc, argv, "--use_tensorrt") != NULL; // live only, tensor accelerator
    
    // TODO Add your Scanbot SDK trial license key here.
    char *scanbot_license_key = license_arg ? license_arg : "<SCANBOTSDK-LICENSE>";

    scanbotsdk_init_params_t params = {0};
    params.license_key   = scanbot_license_key;
    params.writeable_path = ".";

    scanbotsdk_image_t *image = NULL;

    scanbotsdk_error_code_t ec = scanbotsdk_initialize(&params);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "initialize: %d: %s\n", ec, error_message(ec)); goto cleanup; }

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
    else if (strcmp(category, "analyze") == 0) {
        if (!file_path) { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; goto cleanup; }

        if (strcmp(command, "analyze_multi_page") == 0) {
            ec = analyze_multi_page(file_path);
        }
        else if (strcmp(command, "crop_analyze") == 0) {
            ec = load_image_from_path(file_path, &image);
            if (ec != SCANBOTSDK_OK) goto cleanup;

            ec = crop_and_analyze(image, save_path);
        }
        else { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; }
    }
    else if (strcmp(category, "parse") == 0) {
        if (!text_input) { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; goto cleanup; }

        if      (strcmp(command, "mrz")         == 0) ec = parse_mrz(text_input);
        else if (strcmp(command, "barcode_doc") == 0) ec = parse_barcode_document(text_input);
        else { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; }
    }
    else if (strcmp(category, "live") == 0) {
        if (!file_path) { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; goto cleanup; }

        if (strcmp(command, "barcode") == 0) ec = run_live_barcode_scanner(file_path, use_tensor_rt);
    }
    else { print_usage(argv[0]); ec = SCANBOTSDK_ERROR_INVALID_ARGUMENT; }

cleanup:
    scanbotsdk_image_free(image);
    return (ec == SCANBOTSDK_OK) ? 0 : 1;
}