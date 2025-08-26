package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.mrz.*;
import io.scanbot.sdk.utils.Utils;
import io.scanbot.sdk.frameaccumulation.AccumulatedResultsVerifierConfiguration;
import io.scanbot.sdk.image.ImageRef;

public class MrzScannerSnippet {
    public static void run(ImageRef image) throws Exception {
        MrzScannerConfiguration config = new MrzScannerConfiguration();
        AccumulatedResultsVerifierConfiguration accumulated_config = new AccumulatedResultsVerifierConfiguration();
        accumulated_config.setMinimumNumberOfRequiredFramesWithEqualScanningResult(1);

        config.setFrameAccumulationConfiguration(accumulated_config);
        // Configure other parameters as needed.

        try(
            MrzScanner scanner = new MrzScanner(config);
            MrzScannerResult result = scanner.run(image);
        ) {
            printResult(result);
        }
    }

    private static void printResult(MrzScannerResult result) {
        System.out.println("MRZ Scanner Result:");
        System.out.println("  Success: " + result.getSuccess());
        System.out.println("  Raw MRZ: " + result.getRawMRZ());

        Utils.printGenericDocument(result.getDocument());
    }
}
