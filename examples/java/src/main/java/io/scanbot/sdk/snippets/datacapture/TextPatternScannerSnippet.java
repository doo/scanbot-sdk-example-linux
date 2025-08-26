package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.textpattern.*;
import io.scanbot.sdk.image.ImageRef;

public class TextPatternScannerSnippet {
    public static void run(ImageRef image) throws Exception {
        TextPatternScannerConfiguration config = new TextPatternScannerConfiguration();
        config.setMinimumNumberOfRequiredFramesWithEqualScanningResult(1);
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
