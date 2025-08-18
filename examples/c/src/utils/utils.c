#include <stdio.h>
#include <stdlib.h>
#include <utils/utils.h>

scanbotsdk_error_code_t load_image_from_path(const char *path, scanbotsdk_image_t **out_image) {
    scanbotsdk_path_image_load_options_t *load_options = NULL;
    scanbotsdk_error_code_t ec = scanbotsdk_path_image_load_options_create_with_defaults(&load_options);
    if (ec != SCANBOTSDK_OK) return ec;

    ec = scanbotsdk_image_create_from_path(path, load_options, out_image);
    scanbotsdk_path_image_load_options_free(load_options);

    return ec;
}

scanbotsdk_error_code_t print_generic_document_fields(scanbotsdk_generic_document_t *doc) {
    size_t fields_count = 0;
    scanbotsdk_error_code_t ec = scanbotsdk_generic_document_get_fields_size(doc, &fields_count);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_fields_size: %d\n", ec); return ec; }

    printf("Fields count: %zu\n", fields_count);

    scanbotsdk_field_t **fields = calloc(fields_count, sizeof(*fields));
    if (!fields) { fprintf(stderr, "alloc fields failed\n"); return SCANBOTSDK_ERROR_UNKNOWN_ERROR; }

    ec = scanbotsdk_generic_document_get_fields(doc, fields, fields_count);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_fields: %d\n", ec); goto cleanup; }
    
    for (size_t i = 0; i < fields_count; i++) {
        scanbotsdk_field_type_t *field_type = NULL;
        scanbotsdk_field_get_type(fields[i], &field_type);

        const char *type_name = NULL;
        scanbotsdk_field_type_get_name(field_type, &type_name);

        scanbotsdk_ocr_result_t *ocr = NULL;
        if (scanbotsdk_field_get_value(fields[i], &ocr) == SCANBOTSDK_OK && ocr) {
            const char *text = NULL; 
            double confidence = 0.0;
            scanbotsdk_ocr_result_get_text(ocr, &text);
            scanbotsdk_ocr_result_get_confidence(ocr, &confidence);
            printf("Field[%zu]: type=%s, value=\"%s\", confidence=%f\n",
                i, type_name, text ? text : "", confidence);
        } else {
            printf("Field[%zu]: type=%s, value=(n/a)\n", i, type_name);
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


char* get_flag(int argc, char *argv[], const char *flag) {
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], flag) == 0 && i + 1 < argc) {
            return argv[i+1];
        }
        if (strncmp(argv[i], flag, strlen(flag)) == 0 && argv[i][strlen(flag)] == '=') {
            return argv[i] + strlen(flag) + 1;
        }
    }
    return NULL;
}

void print_usage(const char *prog) {
    printf("Scanbot SDK Example\n\n");
    printf("Usage:\n");
    printf("  %s scan <command> --file <path/to/file.jpg>\n", prog);
    printf("or\n");
    printf("  %s classify <command> --file <path/to/file.jpg>\n", prog);
    printf("or\n");
    printf("  %s analyse <command> --file <path/to/file.jpg> --save <path/to/save.jpg>\n", prog);
    printf("or\n");
    printf("  %s parse <command> --text \"<input>\"\n\n", prog);

    printf("Available scan commands:\n");
    printf("  barcode | document | check | credit_card | document_extractor |\n");
    printf("  medical_certificate | mrz | ocr | text_pattern | vin\n\n");

    printf("Available classify commands:\n");
    printf("  document \n\n");

    printf("Available analyse commands:\n");
    printf("  analyse_multi_page | crop_analyze\n\n");

    printf("Available parse commands:\n");
    printf("  mrz | barcode_doc\n\n");

    printf("Note:\n");
    printf("  The --save argument is optional and only used with analyse/crop_analyze.\n\n");

    printf("Examples:\n");
    printf("  %s scan barcode --file images/example.jpg\n", prog);
    printf("  %s analyse analyse_multi_page --file files/doc.pdf\n", prog);
    printf("  %s analyse crop_analyze --file images/doc.jpg --save out/crop.jpg\n", prog);
    printf("  %s parse mrz --text \"P<UTOERIKSSON<<ANNA<MARIA<<<<<<\"\n", prog);
    printf("\n");
}