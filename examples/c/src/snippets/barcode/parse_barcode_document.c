#include <stdio.h>
#include <stdlib.h>

#include <snippets/barcode/parse_barcode_document.h>
#include <utils/utils.h>

scanbotsdk_error_code_t parse_barcode_document(char* raw_string) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;

    scanbotsdk_barcode_document_format_t formats[] = {
        SCANBOTSDK_BARCODE_DOCUMENT_FORMAT_AAMVA,
        SCANBOTSDK_BARCODE_DOCUMENT_FORMAT_BOARDING_PASS,
        SCANBOTSDK_BARCODE_DOCUMENT_FORMAT_DE_MEDICAL_PLAN,
        SCANBOTSDK_BARCODE_DOCUMENT_FORMAT_MEDICAL_CERTIFICATE,
        SCANBOTSDK_BARCODE_DOCUMENT_FORMAT_ID_CARD_PDF_417,
        SCANBOTSDK_BARCODE_DOCUMENT_FORMAT_SEPA,
        SCANBOTSDK_BARCODE_DOCUMENT_FORMAT_SWISS_QR,
        SCANBOTSDK_BARCODE_DOCUMENT_FORMAT_VCARD,
        SCANBOTSDK_BARCODE_DOCUMENT_FORMAT_GS1,
        SCANBOTSDK_BARCODE_DOCUMENT_FORMAT_HIBC,
        SCANBOTSDK_BARCODE_DOCUMENT_FORMAT_BRITISH_COLUMBIA_DRIVER_LICENSE,
    };

    scanbotsdk_barcode_document_parser_t* parser = NULL;
    scanbotsdk_barcode_document_parser_result_t* result = NULL;
    scanbotsdk_generic_document_t *parsed_doc = NULL;
    bool success;

    size_t formats_count = sizeof(formats) / sizeof(formats[0]);
    ec = scanbotsdk_barcode_document_parser_create(formats, formats_count, &parser);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "create_parser: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    
    ec = scanbotsdk_barcode_document_parser_parse(parser, raw_string, &result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "parser_parse: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_barcode_document_parser_result_get_parsed_document(result, &parsed_doc);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_parsed_document: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_barcode_document_parser_result_get_success(result, &success);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_success: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    printf("Success: %d\n", success);
    if(parsed_doc != NULL) { 
        print_generic_document_fields(parsed_doc); 
    }
    
cleanup:
    scanbotsdk_barcode_document_parser_result_free(result);
    scanbotsdk_barcode_document_parser_free(parser);
    return ec;
}

