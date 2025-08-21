import * as ScanbotSDK from "scanbotsdk";
import { printGenericDocument } from "../utils/utils";

export class CreditCardScannerSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    const licenseInfo = await ScanbotSDK.getLicenseInfo();
    if (licenseInfo.status !== "OKAY") {
      console.warn("License is not valid.");
      return;
    }

    ScanbotSDK.autorelease(async () => {
      var config = new ScanbotSDK.CreditCardScannerConfiguration();
      // Configure other parameters as needed.

      const scanner = await ScanbotSDK.CreditCardScanner.create(config);
      const result = await scanner.run(image);

      this.printResult(result);
    });
  }

  private static printResult(
    result: ScanbotSDK.CreditCardScanningResult
  ): void {
    console.log("  Detection status: " + result.detectionStatus);
    console.log("  Scanning status: " + result.scanningStatus);

    printGenericDocument(result.creditCard);
  }
}
