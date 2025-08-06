package io.scanbot.sdk.snippets.datacapture;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import io.scanbot.sdk.documentdata.*;
import io.scanbot.sdk.image.ImageRef;

public class DocumentDataExtractorSnippet {
    public static void run(ImageRef image) throws Exception {
        List<DocumentDataExtractorConfigurationElement> elements = Collections.singletonList(
            new DocumentDataExtractorCommonConfiguration(
                Arrays.asList(
                    "DeIdCardFront",
                    "DeIdCardBack",
                    "DeHealthInsuranceCardFront",
                    "DePassport",
                    "DeResidencePermitFront",
                    "DeResidencePermitBack",
                    "EuropeanHealthInsuranceCard",
                    "EuropeanDriverLicenseFront",
                    "EuropeanDriverLicenseBack"
                )
            )
        );

        DocumentDataExtractorConfiguration configuration = new DocumentDataExtractorConfiguration(elements);
        try(
            var scanner = new DocumentDataExtractor(configuration)
        ) {
            var result = scanner.run(image);
            printResult(result);
        }
    }

    private static void printResult(DocumentDataExtractionResult result) {
        
    }
}
