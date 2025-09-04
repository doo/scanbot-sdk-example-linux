package io.scanbot.sdk.snippets.datacapture;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import io.scanbot.sdk.documentdata.*;
import io.scanbot.sdk.frameaccumulation.ConfirmationMethod;
import io.scanbot.sdk.frameaccumulation.ResultAccumulationConfiguration;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.utils.Utils;

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

        ResultAccumulationConfiguration accumulation_config = new ResultAccumulationConfiguration();
        accumulation_config.setConfirmationMethod(ConfirmationMethod.EXACT);

        DocumentDataExtractorConfiguration config = new DocumentDataExtractorConfiguration();
        config.setConfigurations(elements);
        config.setResultAccumulationConfig(accumulation_config);
        // Configure other parameters as needed.

        try(
            DocumentDataExtractor scanner = new DocumentDataExtractor(config);
            DocumentDataExtractionResult result = scanner.run(image);
        ) {
            Utils.printGenericDocument(result.getDocument());
        }
    }
}
