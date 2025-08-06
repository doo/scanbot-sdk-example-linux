package io.scanbot.sdk.snippets.document;

import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzer;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzerConfiguration;
import io.scanbot.sdk.documentscanner.DocumentScanner;
import io.scanbot.sdk.documentscanner.DocumentScannerConfiguration;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.image.SaveImageOptions;

public class CropAndAnalyzeSnippet {

    public static void run(ImageRef image , String savePath) throws Exception {
        try (
            var scanner = new DocumentScanner(new DocumentScannerConfiguration());
            var analyzer = new DocumentQualityAnalyzer(new DocumentQualityAnalyzerConfiguration())
        ) {
            var scanResult = scanner.scan(image);
            var detection = scanResult.getDetectionResult();
            System.out.println("Detection status: " + detection.getStatus());
            detection.getPoints().forEach(p ->
                System.out.printf("x: %.2f, y: %.2f%n", p.getX(), p.getY())
            );

            var cropped = scanResult.getCroppedImage();
            if (cropped != null) {
                if (savePath != null) {
                    cropped.saveImage(savePath, new SaveImageOptions());
                    System.out.println("Saved to: " + savePath);
                }

                var result = analyzer.run(cropped);
                System.out.printf("Found: %b, Quality: %s%n",
                        result.getDocumentFound(), result.getQuality());
            }
        }
    }
}
