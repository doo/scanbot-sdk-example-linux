package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.medicalcertificate.*;
import io.scanbot.sdk.image.ImageRef;

public class MedicalCertificateScannerSnippet {
    public static void run(ImageRef image) throws Exception {
        try(
            var scanner = new MedicalCertificateScanner()
        ) {
            var result = scanner.run(image, new MedicalCertificateScanningParameters());
            printResult(result);
        }
    }

    private static void printResult(MedicalCertificateScanningResult result) {

    }
}
