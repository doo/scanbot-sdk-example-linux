package io.scanbot.sdk.snippets.document;

import io.scanbot.sdk.documentscanner.*;
import io.scanbot.sdk.image.ImageRef;

public class DetectDocumentSnippet {
    public static void run(ImageRef image) throws Exception {
        try (
            var scanner = new DocumentScanner(new DocumentScannerConfiguration())
        ) {
            var result = scanner.run(image);

            System.out.println("Detection status: " + result.getStatus());
            result.getPoints().forEach(p ->
                System.out.printf("x: %.2f, y: %.2f%n", p.getX(), p.getY())
            );
        }
    }
}
