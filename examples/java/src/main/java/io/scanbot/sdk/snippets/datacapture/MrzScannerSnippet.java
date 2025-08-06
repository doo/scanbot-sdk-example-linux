package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.mrz.*;
import io.scanbot.sdk.image.ImageRef;

public class MrzScannerSnippet {
    public static void run(ImageRef image) throws Exception {
        try(
            var scanner = new MrzScanner(new MrzScannerConfiguration())
        ) {
            var result = scanner.run(image);
            printResult(result);
        }
    }

    private static void printResult(MrzScannerResult result) {
        System.out.println("MRZ Scanner Result:");
        System.out.println("  Success: " + result.getSuccess());
        System.out.println("  Raw MRZ: " + result.getRawMRZ());

        var doc = result.getDocument();
        if (doc != null) {
            System.out.println("  Generic Document:");
            System.out.println("    Type: " + doc.getType());
            System.out.println("    Confidence: " + doc.getConfidence() + " (weight: " + doc.getConfidenceWeight() + ")");
        }
    }
}
