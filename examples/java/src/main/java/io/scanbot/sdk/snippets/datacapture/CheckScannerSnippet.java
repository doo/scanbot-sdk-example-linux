package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.check.*;
import io.scanbot.sdk.image.ImageRef;

public class CheckScannerSnippet {
     public static void run(ImageRef image) throws Exception {
        try(
            var scanner = new CheckScanner(new CheckScannerConfiguration())
        ) {
            var result = scanner.run(image);
            printResult(result);
        }
    }

    private static void printResult(CheckScanningResult result) {

    }
}
