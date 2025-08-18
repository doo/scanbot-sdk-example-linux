package io.scanbot.sdk.snippets.barcode;

import java.util.Arrays;
import java.util.List;

import io.scanbot.sdk.ScanbotSDK;
import io.scanbot.sdk.barcode.*;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.licensing.LicenseStatus;

public class DetectBarcodesSnippet {
    public static void run(ImageRef image) throws Exception {
        // Make sure you have a valid license
        if(ScanbotSDK.getLicenseInfo().getStatus() != LicenseStatus.OKAY)
            return;

        BarcodeFormatCommonConfiguration formatConfig = new BarcodeFormatCommonConfiguration();
        formatConfig.setGs1Handling(Gs1Handling.DECODE_FULL);

        BarcodeScannerConfiguration config = new BarcodeScannerConfiguration();
        config.setReturnBarcodeImage(true);
        config.setBarcodeFormatConfigurations(Arrays.asList(new BarcodeFormatCommonConfiguration()));
        // Configure other parameters as needed.

        try (
            BarcodeScanner scanner = new BarcodeScanner(config)
        ) {
            BarcodeScannerResult result = scanner.run(image);
            List<BarcodeItem> barcodes = result.getBarcodes();

            for (int i = 0; i < barcodes.size(); i++) {
                BarcodeItem barcode = barcodes.get(i);
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
