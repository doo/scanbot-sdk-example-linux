package io.scanbot.sdk.snippets.document;

import io.scanbot.sdk.ScanbotSDK;
import io.scanbot.sdk.documentclassifier.DocumentClassifier;
import io.scanbot.sdk.documentclassifier.DocumentClassifierConfiguration;
import io.scanbot.sdk.documentclassifier.DocumentClassifierResult;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.licensing.LicenseStatus;

public class DocumentClassifierSnippet {
    public static void run(ImageRef image) throws Exception {
        // Make sure you have a valid license
        if(ScanbotSDK.getLicenseInfo().getStatus() != LicenseStatus.OKAY)
            return;
        
        DocumentClassifierConfiguration config = new DocumentClassifierConfiguration();
        // Configure other parameters as needed.

        try (
            DocumentClassifier classifier = new DocumentClassifier(config);
        ) {
            DocumentClassifierResult result = classifier.run(image);
            
            System.out.println("Detection status: " + result.getStatus());
            System.out.println("Type: " + result.getDocumentType());
            result.getDocumentScanningResult().getDetectionResult().getPoints().forEach(p ->
                System.out.printf("x: %d, y: %d%n", p.getX(), p.getY())
            );
        }
    }
}
