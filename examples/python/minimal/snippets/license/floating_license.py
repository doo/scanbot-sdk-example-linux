from scanbotsdk import *

SCANBOTSDK_LICENSE_CHECK_TIMEOUT_MS = 15000
DEREGISTER_DEVICE_TIMEOUT_MS = 15000

def init_with_floating_license():
    scanbot_license_key = "SCANBOTSDK-LICENSE"
    initialize(scanbot_license_key)

    # BEFORE waiting for online check
    before_info = get_license_info()
    print(f"[before wait] status={before_info.status.name} ({before_info.status.value}), "
          f"online_check_in_progress={before_info.online_license_check_in_progress}")

    with DeviceSession(deregister_timeout_ms=SCANBOTSDK_LICENSE_CHECK_TIMEOUT_MS):
        # Wait for the online license check
        wait_for_online_license_check_completion(DEREGISTER_DEVICE_TIMEOUT_MS)

        # AFTER waiting for online check
        after_info = get_license_info()
        print(f"[after wait]  status={after_info.status.name} ({after_info.status.value}), "
              f"online_check_in_progress={after_info.online_license_check_in_progress}")