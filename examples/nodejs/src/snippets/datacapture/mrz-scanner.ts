import * as ScanbotSDK from "scanbotsdk";
import { printGenericDocument } from "../utils/utils";

export class MrzScannerSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    const licenseInfo = await ScanbotSDK.getLicenseInfo();
    if (licenseInfo.status !== "OKAY") {
      console.warn("License is not valid.");
      return;
    }

    await ScanbotSDK.autorelease(async () => {
      const config = new ScanbotSDK.MrzScannerConfiguration();

      const accumulatedConfig = new ScanbotSDK.AccumulatedResultsVerifierConfiguration();
      accumulatedConfig.minimumNumberOfRequiredFramesWithEqualScanningResult = 1;

      config.frameAccumulationConfiguration = accumulatedConfig;
      // configure other parameters as needed

      const scanner = await ScanbotSDK.MrzScanner.create(config);
      const result = await scanner.run(image);

      console.log("MRZ Scanner Result:");
      console.log("  Success: " + result.success);
      console.log("  Raw MRZ: " + result.rawMRZ);

      printGenericDocument(result.document);
    });
  }
}
