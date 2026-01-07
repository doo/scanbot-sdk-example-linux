#include <stdio.h>
#include <stdlib.h>

#include <snippets/datacapture/mrz_parser.h>
#include <utils/utils.h>

scanbotsdk_error_code_t parse_mrz(const char* text) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;

    scanbotsdk_mrz_parser_t* parser = NULL;
    scanbotsdk_mrz_parser_configuration_t* config = NULL;
    scanbotsdk_mrz_scanner_result_t *result = NULL;
    scanbotsdk_generic_document_t *mrz = NULL;
    bool status;

    ec = scanbotsdk_mrz_parser_configuration_create_with_defaults(&config);
    // Configure other parameters as needed.
    
    ec = scanbotsdk_mrz_parser_create(config, &parser);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "create_parser: %d: %s\n", ec, error_message(ec)); goto cleanup; }
    
    ec = scanbotsdk_mrz_parser_parse(parser, text, &result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "parse: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_mrz_scanner_result_get_document(result, &mrz);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_document: %d\n", ec); return ec; }

    scanbotsdk_mrz_scanner_result_get_success(result, &status);
    printf("Mrz detection success status = %d\n", status);

    if(mrz != NULL) { 
        print_generic_document_fields(mrz); 
    }

cleanup:
    scanbotsdk_mrz_parser_configuration_free(config);
    scanbotsdk_mrz_parser_free(parser);
    scanbotsdk_mrz_scanner_result_free(result);
    return ec;
}

