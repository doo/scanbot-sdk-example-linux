import * as ScanbotSDK from "scanbotsdk";
import { printGenericDocument } from "../utils/utils";

export class DocumentDataExtractorSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
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

    // `await using` ensures both extractor and result are properly disposed
    // when the scope ends, as they hold unmanaged resources.
    await using extractor = await ScanbotSDK.DocumentDataExtractor.create(config);
    await using result = await extractor.run(image);

    this.printResult(result);
  }

  private static printResult(
    result: ScanbotSDK.DocumentDataExtractionResult
  ): void {
    console.log(`Status: #${result.status}`);
    printGenericDocument(result.document);
  }
}
