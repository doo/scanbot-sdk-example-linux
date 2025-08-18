package io.scanbot.sdk.snippets.datacapture;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import io.scanbot.sdk.ScanbotSDK;
import io.scanbot.sdk.documentdata.*;
import io.scanbot.sdk.exception.InvalidImageRefException;
import io.scanbot.sdk.frameaccumulation.ConfirmationMethod;
import io.scanbot.sdk.frameaccumulation.ResultAccumulationConfiguration;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.licensing.LicenseStatus;
import io.scanbot.sdk.utils.Utils;

public class DocumentDataExtractorSnippet {
    public static void run(ImageRef image) throws Exception {
        // Make sure you have a valid license
        if(ScanbotSDK.getLicenseInfo().getStatus() != LicenseStatus.OKAY)
            return;

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

        DocumentDataExtractorConfiguration config = new DocumentDataExtractorConfiguration(elements);
        config.setResultAccumulationConfig(accumulation_config);
        // Configure other parameters as needed.

        try(
            DocumentDataExtractor scanner = new DocumentDataExtractor(config)
        ) {
            DocumentDataExtractionResult result = scanner.run(image);
            printResult(result);
        }
    }

    private static void printResult(DocumentDataExtractionResult result) throws InvalidImageRefException {
        // If you want to use the image later, call result.getCroppedImage().encodeImage(...) and save the returned buffer.
        // Otherwise, the image reference will be released once the ImageRef object is closed or garbage-collected.

        Utils.printGenericDocument(result.getDocument());
    }
}
