package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.vin.*;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.textpattern.TextPatternScannerResult;

public class VINScannerSnippet {
    public static void run(ImageRef image) throws Exception {
        VinScannerConfiguration config = new VinScannerConfiguration();
        config.setExtractVINFromBarcode(true);
        // Configure other parameters as needed.

        try(
            VinScanner scanner = new VinScanner(config);
        ) {
            VinScannerResult result = scanner.run(image);
            printResult(result);
        }
    }

    private static void printResult(VinScannerResult result) {
        TextPatternScannerResult textResult = result.getTextResult();
        if (textResult != null) {
            System.out.println("Text: " + textResult.getRawText());
        }

        VinBarcodeResult barcodeResult = result.getBarcodeResult();
        if (barcodeResult != null) {
            System.out.println("Extracted VIN: " + barcodeResult.getExtractedVIN());
        }
    }
}
