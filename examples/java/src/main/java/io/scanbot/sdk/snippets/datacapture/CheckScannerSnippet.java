package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.ScanbotSDK;
import io.scanbot.sdk.check.*;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.licensing.LicenseStatus;
import io.scanbot.sdk.utils.Utils;

public class CheckScannerSnippet {
     public static void run(ImageRef image) throws Exception {
        // Make sure you have a valid license
        if(ScanbotSDK.getLicenseInfo().getStatus() != LicenseStatus.OKAY)
            return;

        CheckScannerConfiguration config = new CheckScannerConfiguration();
        config.setDocumentDetectionMode(CheckDocumentDetectionMode.DETECT_AND_CROP_DOCUMENT);
        // Configure other parameters as needed.
        
        try(
            CheckScanner scanner = new CheckScanner(config)
        ) {
            CheckScanningResult result = scanner.run(image);
            printResult(result);
        }
    }

    private static void printResult(CheckScanningResult result) {
        // If you want to use the image later, call result.getCroppedImage().encodeImage(...) and save the returned buffer.
        // Otherwise, the image reference will be released once the ImageRef object is closed or garbage-collected.
        Utils.printGenericDocument(result.getCheck());
    }
}
