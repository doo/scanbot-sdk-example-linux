#include <stdio.h>
#include <stdbool.h>
#include <snippets/license/floating_license.h>
#include <utils/utils.h>

char* license_status_str(scanbotsdk_license_status_t s) {
    switch (s) {
        case SCANBOTSDK_LICENSE_STATUS_OKAY: return "OKAY";
        case SCANBOTSDK_LICENSE_STATUS_TRIAL: return "TRIAL";
        case SCANBOTSDK_LICENSE_STATUS_OKAY_EXPIRING_SOON: return "OKAY_EXPIRING_SOON";
        case SCANBOTSDK_LICENSE_STATUS_FAILURE_NOT_SET: return "FAILURE_NOT_SET";
        case SCANBOTSDK_LICENSE_STATUS_FAILURE_CORRUPTED: return "FAILURE_CORRUPTED";
        case SCANBOTSDK_LICENSE_STATUS_FAILURE_WRONG_OS: return "FAILURE_WRONG_OS";
        case SCANBOTSDK_LICENSE_STATUS_FAILURE_APP_ID_MISMATCH: return "FAILURE_APP_ID_MISMATCH";
        case SCANBOTSDK_LICENSE_STATUS_FAILURE_EXPIRED: return "FAILURE_EXPIRED";
        case SCANBOTSDK_LICENSE_STATUS_FAILURE_SERVER: return "FAILURE_SERVER";
        case SCANBOTSDK_LICENSE_STATUS_FAILURE_VERSION: return "FAILURE_VERSION";
        case SCANBOTSDK_LICENSE_STATUS_FAILURE_INACTIVE: return "FAILURE_INACTIVE";
        default: return "UNKNOWN";
    }
}

void init_with_floating_license() {
    const char *scanbot_license_key = "SCANBOTSDK-LICENSE";

    scanbotsdk_init_params_t params = {0};
    params.license_key    = scanbot_license_key;
    params.writeable_path = ".";

    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_license_info_t* info = NULL; 

    ec = scanbotsdk_initialize(&params);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "initialize: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    // Query license info BEFORE waiting for online check
    ec = scanbotsdk_get_license_info(&info);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_license_info (before wait): %d: %s\n", ec, error_message(ec)); goto cleanup; }

    // Wait for the online license check to complete
    ec = scanbotsdk_wait_for_online_license_check_completion(15000);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "wait_for_online_license_check_completion: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    // Query license info AFTER waiting for online check
    ec = scanbotsdk_get_license_info(&info);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_license_info (after wait): %d: %s\n", ec, error_message(ec)); goto cleanup; }
    else {
        scanbotsdk_license_status_t status;
        bool online_check_in_progress;

        ec = scanbotsdk_license_info_get_status(info, &status);
        if (ec != SCANBOTSDK_OK) { fprintf(stderr, "license_info_get_status (after wait): %d: %s\n", ec, error_message(ec)); goto cleanup; }
        
        ec = scanbotsdk_license_info_get_online_license_check_in_progress(info, &online_check_in_progress);
        if (ec != SCANBOTSDK_OK) { fprintf(stderr, "license_info_get_online_license_check_in_progress (after wait): %d: %s\n", ec, error_message(ec)); goto cleanup; }

        printf("[after wait]  status=%s (%d), online_check_in_progress=%s\n",
               license_status_str(status), (int)status,
               online_check_in_progress ? "true" : "false");

    }

cleanup:
    scanbotsdk_license_info_free(info);
    scanbotsdk_deregister_device();
    scanbotsdk_wait_for_device_deregistration_completion(15000);
}