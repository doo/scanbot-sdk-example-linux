package io.scanbot.sdk.snippets.barcode;

import io.scanbot.sdk.barcode.*;
import io.scanbot.sdk.image.ImageRef;

public class DetectBarcodesSnippet {
    public static void run(ImageRef image) throws Exception {
        try (
            var scanner = new BarcodeScanner(new BarcodeScannerConfiguration())
        ) {
            var result = scanner.run(image);
            var barcodes = result.getBarcodes();

            for (int i = 0; i < barcodes.size(); i++) {
                var barcode = barcodes.get(i);
                System.out.printf("Barcode #%d:%n", i + 1);
                System.out.printf("  Format: %s%n", barcode.getFormat().name());
                System.out.printf("  Text:   %s%n", barcode.getText());
            }

            if (barcodes.isEmpty()) {
                System.out.println("No barcodes found.");
            }
        }
    }
}
