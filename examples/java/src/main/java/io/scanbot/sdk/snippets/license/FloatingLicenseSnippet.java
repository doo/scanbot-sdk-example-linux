package io.scanbot.sdk.snippets.license;

import io.scanbot.sdk.DeviceSession;
import io.scanbot.sdk.ScanbotSDK;
import io.scanbot.sdk.licensing.LicenseInfo;

public class FloatingLicenseSnippet {

    public static void initWithFloatingLicense() throws Exception {
        String scanbotLicenseKey = "SCANBOTSDK-LICENSE";

        ScanbotSDK.initialize(scanbotLicenseKey, System.getProperty("user.dir"));

        try (DeviceSession deviceSession = new DeviceSession(15_000)) {
            // BEFORE waiting for online check
            LicenseInfo beforeInfo = ScanbotSDK.getLicenseInfo();
            System.out.printf(
                    "[before wait] status=%s (ordinal=%d), online_check_in_progress=%b%n",
                    beforeInfo.getStatus(),
                    beforeInfo.getStatus().ordinal(),
                    beforeInfo.getOnlineLicenseCheckInProgress()
            );

            // Wait for the online license check
            ScanbotSDK.waitForOnlineLicenseCheckCompletion(15_000);

            // AFTER waiting for online check
            LicenseInfo afterInfo = ScanbotSDK.getLicenseInfo();
            System.out.printf(
                    "[after wait]  status=%s (ordinal=%d), online_check_in_progress=%b%n",
                    afterInfo.getStatus(),
                    afterInfo.getStatus().ordinal(),
                    afterInfo.getOnlineLicenseCheckInProgress()
            );
        }

        ScanbotSDK.deregisterDevice();
        ScanbotSDK.waitForDeviceDeregistrationCompletion(15000);
    }
}
