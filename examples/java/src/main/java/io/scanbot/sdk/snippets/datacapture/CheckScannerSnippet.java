package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.check.*;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.utils.Utils;

public class CheckScannerSnippet {
     public static void run(ImageRef image) throws Exception {
        CheckScannerConfiguration config = new CheckScannerConfiguration();
        config.setDocumentDetectionMode(CheckDocumentDetectionMode.DETECT_AND_CROP_DOCUMENT);
        // Configure other parameters as needed.
        
        try(
            CheckScanner scanner = new CheckScanner(config);
            CheckScanningResult result = scanner.run(image);
        ) {
            Utils.printGenericDocument(result.getCheck());
        }
    }
}
