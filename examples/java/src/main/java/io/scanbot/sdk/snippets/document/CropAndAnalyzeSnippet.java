package io.scanbot.sdk.snippets.document;

import io.scanbot.sdk.ScanbotSDK;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzer;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzerConfiguration;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzerResult;
import io.scanbot.sdk.documentscanner.DocumentDetectionResult;
import io.scanbot.sdk.documentscanner.DocumentScanner;
import io.scanbot.sdk.documentscanner.DocumentScannerConfiguration;
import io.scanbot.sdk.documentscanner.DocumentScannerParameters;
import io.scanbot.sdk.documentscanner.DocumentScanningResult;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.image.SaveImageOptions;
import io.scanbot.sdk.licensing.LicenseStatus;
import io.scanbot.sdk.utils.Utils;

public class CropAndAnalyzeSnippet {

    public static void run(String filePath, String resourcePath, String savePath) throws Exception {
        // Make sure you have a valid license
        if(ScanbotSDK.getLicenseInfo().getStatus() != LicenseStatus.OKAY)
            return;

        DocumentScannerParameters scanner_params = new DocumentScannerParameters();
        scanner_params.setAcceptedAngleScore(75);
        scanner_params.setIgnoreOrientationMismatch(false);

        DocumentScannerConfiguration config = new DocumentScannerConfiguration();
        config.setParameters(scanner_params);

        DocumentQualityAnalyzerConfiguration analyse_config = new DocumentQualityAnalyzerConfiguration();
        analyse_config.setTileSize(300);
        analyse_config.setMinEstimatedNumberOfSymbolsForDocument(20);
        // Configure other parameters as needed.

        try (
            ImageRef image = Utils.createImageRef(filePath, resourcePath);
            DocumentScanner scanner = new DocumentScanner(config);
            DocumentQualityAnalyzer analyzer = new DocumentQualityAnalyzer(analyse_config)
        ) {
            DocumentScanningResult scanResult = scanner.scan(image);
            DocumentDetectionResult detection = scanResult.getDetectionResult();
            System.out.println("Detection status: " + detection.getStatus());
            detection.getPoints().forEach(p ->
                System.out.printf("x: %d, y: %d%n", p.getX(), p.getY())
            );

            ImageRef cropped = scanResult.getCroppedImage();
            if (cropped != null) {
                if (savePath != null) {
                    cropped.saveImage(savePath, new SaveImageOptions());
                    System.out.println("Saved to: " + savePath);
                }

                DocumentQualityAnalyzerResult result = analyzer.run(cropped);
                System.out.printf("Found: %b, Quality: %s%n",
                        result.getDocumentFound(), result.getQuality());
            }
        }
    }
}
