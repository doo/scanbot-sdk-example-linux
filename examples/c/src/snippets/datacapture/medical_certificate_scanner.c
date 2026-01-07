#include <stdio.h>
#include <stdlib.h>

#include <snippets/datacapture/medical_certificate_scanner.h>
#include <utils/utils.h>

scanbotsdk_error_code_t print_medical_certificate_result(scanbotsdk_medical_certificate_scanning_result_t* result) {
    scanbotsdk_error_code_t ec;
    scanbotsdk_medical_certificate_patient_info_box_t* patient_info = NULL;
    bool status;
    scanbotsdk_medical_certificate_patient_info_field_t **fields = NULL;
    
    ec = scanbotsdk_medical_certificate_scanning_result_get_scanning_successful(result, &status);
    printf("Medical Certificate scanning status = %d\n", status);

    ec = scanbotsdk_medical_certificate_scanning_result_get_patient_info_box(result, &patient_info);

    size_t count = 0;
    ec = scanbotsdk_medical_certificate_patient_info_box_get_fields_size(patient_info, &count);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_fields_size: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    fields = calloc(count, sizeof(*fields));
    ec = scanbotsdk_medical_certificate_patient_info_box_get_fields(patient_info, fields, count);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_patient_info_box_get_fields: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    for (size_t i = 0; i < count; ++i) {
        scanbotsdk_medical_certificate_patient_info_field_type_t type;
        const char* value = NULL;

        scanbotsdk_medical_certificate_patient_info_field_get_type(fields[i], &type);
        scanbotsdk_medical_certificate_patient_info_field_get_value(fields[i], &value);

        fprintf(stderr, "Type: %d Value: %s\n", type, value);
    }
    
cleanup:
    free(fields);
    return ec;
}

scanbotsdk_error_code_t detect_medical_certificate(scanbotsdk_image_t *image) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    
    scanbotsdk_medical_certificate_scanning_parameters_t *params = NULL;
    scanbotsdk_medical_certificate_scanner_t *scanner = NULL;
    scanbotsdk_medical_certificate_scanning_result_t *result = NULL;

    ec = scanbotsdk_medical_certificate_scanning_parameters_create_with_defaults(&params);
    // Configure other parameters as needed.

    ec = scanbotsdk_medical_certificate_scanner_create(&scanner);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_medical_certificate_scanner_run(scanner, image, params, &result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "scanner_run: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = print_medical_certificate_result(result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "print_result: %d: %s\n", ec, error_message(ec)); goto cleanup; }

cleanup:
    scanbotsdk_medical_certificate_scanning_result_free(result);
    scanbotsdk_medical_certificate_scanner_free(scanner);
    scanbotsdk_medical_certificate_scanning_parameters_free(params);
    return ec;
}