package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.ScanbotSDK;
import io.scanbot.sdk.creditcard.*;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.licensing.LicenseStatus;
import io.scanbot.sdk.utils.Utils;

public class CreditCardScannerSnippet {
     public static void run(ImageRef image) throws Exception {
        // Make sure you have a valid license
        if(ScanbotSDK.getLicenseInfo().getStatus() != LicenseStatus.OKAY)
            return;

        CreditCardScannerConfiguration config = new CreditCardScannerConfiguration();
        config.setRequireCardholderName(false);
        config.setRequireExpiryDate(true);
        // Configure other parameters as needed.

        try(
            CreditCardScanner scanner = new CreditCardScanner(config)
        ) {
            CreditCardScanningResult result = scanner.run(image);
            printResult(result);
        }
    }

    private static void printResult(CreditCardScanningResult result) {
        System.out.println("Credit Card Scanning Result:");
        System.out.println("  Detection status: " + result.getDetectionStatus());
        System.out.println("  Scanning status: " + result.getScanningStatus());

        Utils.printGenericDocument(result.getCreditCard());
    }
}
