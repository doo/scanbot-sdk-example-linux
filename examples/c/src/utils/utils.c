#include <stdio.h>
#include <stdlib.h>
#include <utils/utils.h>

scanbotsdk_error_code_t load_image_from_path(const char *path, scanbotsdk_image_t **out_image) {
    scanbotsdk_path_image_load_options_t *load_options = NULL;
    scanbotsdk_error_code_t ec = scanbotsdk_path_image_load_options_create_with_defaults(&load_options);

    ec = scanbotsdk_image_create_from_path(path, load_options, out_image);
    scanbotsdk_path_image_load_options_free(load_options);
    
    return ec;
}

scanbotsdk_error_code_t print_generic_document_fields(scanbotsdk_generic_document_t *doc) {
    if (doc == NULL) {
        printf("No document data available.\n");
        return SCANBOTSDK_OK;
    }
    size_t fields_count = 0;
    scanbotsdk_field_t **fields = NULL;
    scanbotsdk_error_code_t ec = scanbotsdk_generic_document_get_fields_size(doc, &fields_count);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_fields_size: %d\n", ec); goto cleanup; }

    printf("Fields count: %zu\n", fields_count);

    fields = calloc(fields_count, sizeof(*fields));
    if (!fields) { fprintf(stderr, "alloc fields failed\n"); ec = SCANBOTSDK_ERROR_OUT_OF_MEMORY; goto cleanup; }

    ec = scanbotsdk_generic_document_get_fields(doc, fields, fields_count);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_fields: %d\n", ec); goto cleanup; }
    
    for (size_t i = 0; i < fields_count; i++) {
        scanbotsdk_field_type_t *field_type = NULL;
        scanbotsdk_field_get_type(fields[i], &field_type);

        const char *type_name = NULL;
        scanbotsdk_field_type_get_name(field_type, &type_name);

      scanbotsdk_ocr_result_t *ocr_result = NULL;
        if (scanbotsdk_field_get_value(fields[i], &ocr_result) == SCANBOTSDK_OK && ocr_result) {
            const char *text = NULL;
            scanbotsdk_ocr_result_get_text(ocr_result, &text);
            printf("Field[%zu]: type=%s, value=\"%s\"\n",
                i, type_name, text ? text : "text value n/a");
        } else {
            printf("Field[%zu]: type=%s, value=text value n/a\n", i, type_name);
        }
    }

cleanup:
    free(fields);
    return ec;
}

const char *error_message(scanbotsdk_error_code_t ec) {
    if (ec == SCANBOTSDK_ERROR_INVALID_LICENSE_STATUS) {
        return "Invalid license status. Please check your license key.";
    }
    return scanbotsdk_error_message();
}


const char* get_flag(int argc, char *argv[], const char *flag) {
    size_t len = strlen(flag);

    for (int i = 1; i < argc; i++) {

        // "--flag"
        if (strcmp(argv[i], flag) == 0) {
            if (i + 1 < argc && strncmp(argv[i+1], "--", 2) != 0)
                return argv[i+1];

            return "";
        }

         // "--flag=value"
        if (strncmp(argv[i], flag, len) == 0 && argv[i][len] == '=') {
            return argv[i] + len + 1;
        }
    }

    return NULL;
}

void print_usage(const char *prog) {
    printf("Scanbot SDK Example\n\n");
    printf("Usage:\n");
    printf("  %s scan <command> --file <path/to/file.jpg> [--license <KEY>]\n", prog);
    printf("or\n");
    printf("  %s classify <command> --file <path/to/file.jpg> [--license <KEY>]\n", prog);
    printf("or\n");
    printf("  %s analyze <command> --file <path/to/file.jpg> --save <path/to/save.jpg> [--license <KEY>]\n", prog);
    printf("or\n");
    printf("  %s parse <command> --text \"<input>\" [--license <KEY>]\n\n", prog);
    printf("or\n");
    printf("  %s live <command> --file \"<input>\" [--license <KEY>] [--use_tensorrt]\n\n", prog);

    printf("Available scan commands:\n");
    printf("  barcode | document | check | credit_card | document_data_extractor |\n");
    printf("  medical_certificate | mrz | ocr | text_pattern | vin\n\n");

    printf("Available classify commands:\n");
    printf("  document \n\n");

    printf("Available analyze commands:\n");
    printf("  analyze_multi_page | crop_analyze\n\n");

    printf("Available parse commands:\n");
    printf("  mrz | barcode_doc\n\n");

    printf("Available live commands:\n");
    printf("  barcode \n\n");
    printf(
        "This code uses a mock camera implementation" 
        "The mock camera loads a single image from the given file path. \n");
    printf(
        "This allows you to test the barcode scanner "
        "pipeline with a static image without needing access "
        "to an actual camera or live video feed.\n");

    printf("Note:\n");
    printf("  The --save argument is optional and only used with analyze/crop_analyze.\n");
    printf("  The --license argument is optional. If not provided, the program will\n");
    printf("  \tcheck the placeholder <SCANBOTSDK-LICENSE> in main.c\n");

    printf("Examples:\n");
    printf("  %s scan barcode --file images/example.jpg --license <KEY>\n", prog);
    printf("  %s analyze analyze_multi_page --file files/doc.pdf --license <KEY>\n", prog);
    printf("  %s analyze crop_analyze --file images/doc.jpg --save out/crop.jpg --license <KEY>\n", prog);
    printf("  %s parse mrz --text \"P<UTOERIKSSON<<ANNA<MARIA<<<<<<\" --license <KEY>\n", prog);
    printf("  %s live barcode --file images/example.jpg --license <KEY>\n", prog);
    printf("\n");
}