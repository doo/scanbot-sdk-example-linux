import * as ScanbotSDK from "scanbotsdk";
import { printGenericDocument } from "../utils/utils";

export class DocumentDataExtractorSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    const licenseInfo = await ScanbotSDK.getLicenseInfo();
    if (licenseInfo.status !== "OKAY") {
      console.warn("License is not valid.");
      return;
    }

    ScanbotSDK.autorelease(async () => {
      var commonConfig =
        new ScanbotSDK.DocumentDataExtractorCommonConfiguration();
      commonConfig.acceptedDocumentTypes = [
        "DeIdCardFront",
        "DeIdCardBack",
        "DeHealthInsuranceCardFront",
        "DePassport",
        "DeResidencePermitFront",
        "DeResidencePermitBack",
        "EuropeanHealthInsuranceCard",
        "EuropeanDriverLicenseFront",
        "EuropeanDriverLicenseBack",
      ];
      // Configure supported document types
      const elements: ScanbotSDK.DocumentDataExtractorConfigurationElement[] = [
        commonConfig,
      ];

      const accumulationConfig =
        new ScanbotSDK.ResultAccumulationConfiguration();
      accumulationConfig.confirmationMethod = "EXACT";

      const config = new ScanbotSDK.DocumentDataExtractorConfiguration();
      config.configurations = elements;
      config.resultAccumulationConfig = accumulationConfig;
      // Configure other parameters as needed.

      const extractor = await ScanbotSDK.DocumentDataExtractor.create(config);
      const result = await extractor.run(image);

      this.printResult(result);
    });
  }

  private static printResult(
    result: ScanbotSDK.DocumentDataExtractionResult
  ): void {
    // If you want to use the image later, call result.croppedImage?.encodeImage(...) and save the returned buffer.
    // Otherwise, the image reference will be released once the ImageRef object is closed or garbage-collected.

    console.log(`Status: #${result.status}`);
    printGenericDocument(result.document);
  }
}
