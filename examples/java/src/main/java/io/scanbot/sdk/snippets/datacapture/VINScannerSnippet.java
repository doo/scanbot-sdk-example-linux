package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.vin.*;
import io.scanbot.sdk.image.ImageRef;

public class VINScannerSnippet {
    public static void run(ImageRef image) throws Exception {
        try(
            var scanner = new VinScanner(new VinScannerConfiguration())
        ) {
            var result = scanner.run(image);
            printResult(result);
        }
    }

    private static void printResult(VinScannerResult result) {

    }
}
