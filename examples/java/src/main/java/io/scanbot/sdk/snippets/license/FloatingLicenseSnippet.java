package io.scanbot.sdk.snippets.license;

import io.scanbot.sdk.DeviceSession;
import io.scanbot.sdk.ScanbotSDK;
import io.scanbot.sdk.licensing.LicenseInfo;

public class FloatingLicenseSnippet {
    static final int SCANBOTSDK_LICENSE_CHECK_TIMEOUT_MS = 15000;

    public static void initWithFloatingLicense() throws Exception {
        String scanbotLicenseKey = "SCANBOTSDK-LICENSE";

        ScanbotSDK.initialize(scanbotLicenseKey, System.getProperty("user.dir"));

        // Automatically handles license deregistration when it closes.
        try (DeviceSession deviceSession = new DeviceSession(SCANBOTSDK_LICENSE_CHECK_TIMEOUT_MS)) {
            // BEFORE waiting for online check
            LicenseInfo beforeInfo = ScanbotSDK.getLicenseInfo();
            System.out.printf(
                    "[before wait] status=%s (ordinal=%d), online_check_in_progress=%b%n",
                    beforeInfo.getStatus(),
                    beforeInfo.getStatus().ordinal(),
                    beforeInfo.getOnlineLicenseCheckInProgress()
            );

            // Wait for the online license check
            ScanbotSDK.waitForOnlineLicenseCheckCompletion(SCANBOTSDK_LICENSE_CHECK_TIMEOUT_MS);

            // AFTER waiting for online check
            LicenseInfo afterInfo = ScanbotSDK.getLicenseInfo();
            System.out.printf(
                    "[after wait]  status=%s (ordinal=%d), online_check_in_progress=%b%n",
                    afterInfo.getStatus(),
                    afterInfo.getStatus().ordinal(),
                    afterInfo.getOnlineLicenseCheckInProgress()
            );
        }
        // If you are not using floating license, it is not required to use io.scanbot.sdk.DeviceSession as there is no
        // need to notify server you are no longer using the license. Alternatively, you can manually call
        // io.scanbot.sdk.ScanbotSDK.deregisterDevice() and io.scanbot.sdk.ScanbotSDK.waitForDeviceDeregistrationCompletion(SCANBOTSDK_LICENSE_CHECK_TIMEOUT_MS) if you need asynchronous
        // deregistration behaviour
    }
}
