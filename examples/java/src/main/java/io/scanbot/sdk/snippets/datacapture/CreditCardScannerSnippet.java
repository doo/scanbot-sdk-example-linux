package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.creditcard.*;
import io.scanbot.sdk.image.ImageRef;

public class CreditCardScannerSnippet {
     public static void run(ImageRef image) throws Exception {
        try(
            var scanner = new CreditCardScanner(new CreditCardScannerConfiguration())
        ) {
            var result = scanner.run(image);
            printResult(result);
        }
    }

    private static void printResult(CreditCardScanningResult result) {
        System.out.println("Credit Card Scanning Result:");
        System.out.println("  Detection status: " + result.getDetectionStatus());
        System.out.println("  Scanning status: " + result.getScanningStatus());

        var doc = result.getCreditCard();
        if (doc != null) {
            System.out.println("  Generic Document:");
            System.out.println("    Type: " + doc.getType());
            System.out.println("    Confidence: " + doc.getConfidence() + " (weight: " + doc.getConfidenceWeight() + ")");
        }

        var quad = result.getQuad();
        var quadNormalized = result.getQuadNormalized();

        System.out.println("  Quad points: " + (quad != null ? quad.size() : 0));
        System.out.println("  Normalized quad points: " + (quadNormalized != null ? quadNormalized.size() : 0));
    }
}
