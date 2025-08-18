package io.scanbot.sdk.snippets.document;

import io.scanbot.sdk.ScanbotSDK;
import io.scanbot.sdk.documentscanner.*;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.licensing.LicenseStatus;

public class DetectDocumentSnippet {
    public static void run(ImageRef image) throws Exception {
        // Make sure you have a valid license
        if(ScanbotSDK.getLicenseInfo().getStatus() != LicenseStatus.OKAY)
            return;
        
        DocumentScannerParameters scanner_params = new DocumentScannerParameters();
        scanner_params.setAcceptedAngleScore(75);
        scanner_params.setIgnoreOrientationMismatch(false);

        DocumentScannerConfiguration config = new DocumentScannerConfiguration();
        config.setParameters(scanner_params);
        // Configure other parameters as needed.
        
        try (
            DocumentScanner scanner = new DocumentScanner(config)
        ) {
            DocumentDetectionResult result = scanner.run(image);
            
            System.out.println("Detection status: " + result.getStatus());
            result.getPoints().forEach(p ->
                System.out.printf("x: %d, y: %d%n", p.getX(), p.getY())
            );
        }
    }
}
