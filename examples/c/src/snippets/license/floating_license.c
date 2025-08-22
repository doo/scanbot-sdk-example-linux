#include <stdio.h>
#include <stdbool.h>
#include <snippets/license/floating_license.h>
#include <utils/utils.h>

static const int SCANBOTSDK_LICENSE_CHECK_TIMEOUT_MS = 15000;
static const int DEREGISTER_DEVICE_TIMEOUT_MS = 15000;

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
    scanbotsdk_license_info_t* info_before_initialization = NULL; 
    scanbotsdk_license_info_t* info_after_initialization  = NULL; 

    ec = scanbotsdk_initialize(&params);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "initialize: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    // Query license info BEFORE waiting for online check
    ec = scanbotsdk_get_license_info(&info_before_initialization);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_license_info (before wait): %d: %s\n", ec, error_message(ec)); goto cleanup; }

    // Wait for the online license check to complete.
    // scanbotsdk_initialize() is non-blocking and triggers the license validation
    // asynchronously in the background. Without waiting here, the license status
    // may still be unknown or incomplete, which can lead to incorrect assumptions
    // about license validity.
    ec = scanbotsdk_wait_for_online_license_check_completion(SCANBOTSDK_LICENSE_CHECK_TIMEOUT_MS);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "wait_for_online_license_check_completion: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    // Query license info AFTER waiting for online check
    ec = scanbotsdk_get_license_info(&info_after_initialization);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_license_info (after wait): %d: %s\n", ec, error_message(ec)); goto cleanup; }
    else {
        scanbotsdk_license_status_t status;
        bool online_check_in_progress;

        ec = scanbotsdk_license_info_get_status(info_after_initialization, &status);
        if (ec != SCANBOTSDK_OK) { fprintf(stderr, "license_info_get_status (after wait): %d: %s\n", ec, error_message(ec)); goto cleanup; }
        
        ec = scanbotsdk_license_info_get_online_license_check_in_progress(info_after_initialization, &online_check_in_progress);
        if (ec != SCANBOTSDK_OK) { fprintf(stderr, "license_info_get_online_license_check_in_progress (after wait): %d: %s\n", ec, error_message(ec)); goto cleanup; }

        printf("[after wait]  status=%s (%d), online_check_in_progress=%s\n",
               license_status_str(status), (int)status,
               online_check_in_progress ? "true" : "false");
    }

cleanup:
    scanbotsdk_license_info_free(info_before_initialization);
    scanbotsdk_license_info_free(info_after_initialization);

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
}