package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.textpattern.*;
import io.scanbot.sdk.ScanbotSDK;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.licensing.LicenseStatus;

public class TextPatternScannerSnippet {
    public static void run(ImageRef image) throws Exception {
        // Make sure you have a valid license
        if(ScanbotSDK.getLicenseInfo().getStatus() != LicenseStatus.OKAY)
            return;

        TextPatternScannerConfiguration config = new TextPatternScannerConfiguration();
        config.setMinimumNumberOfRequiredFramesWithEqualScanningResult(2);
        config.setValidator(new DefaultContentValidator());
        // Configure other parameters as needed.

        try(
            TextPatternScanner scanner = new TextPatternScanner(config)
        ) {
            TextPatternScannerResult result = scanner.run(image);
            System.out.println("  Raw Text: " + result.getRawText());
        }
    }
}
