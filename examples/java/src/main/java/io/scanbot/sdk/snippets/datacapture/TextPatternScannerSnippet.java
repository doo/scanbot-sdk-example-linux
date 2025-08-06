package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.textpattern.*;
import io.scanbot.sdk.image.ImageRef;

public class TextPatternScannerSnippet {
    public static void run(ImageRef image) throws Exception {
        try(
            var scanner = new TextPatternScanner(new TextPatternScannerConfiguration())
        ) {
            var result = scanner.run(image);
            printResult(result);
        }
    }

    private static void printResult(TextPatternScannerResult result) {

    }
}
