import * as ScanbotSDK from "scanbotsdk";
import { printGenericDocument } from "../utils/utils";

export class CreditCardScannerSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    var config = new ScanbotSDK.CreditCardScannerConfiguration();
    config.requireExpiryDate = false;
    config.requireCardholderName = false;
    // Configure other parameters as needed.

    // `await using` ensures both scanner and result are properly disposed
    // when the scope ends, as they hold unmanaged resources.
    await using scanner = await ScanbotSDK.CreditCardScanner.create(config);
    await using result = await scanner.run(image);

    this.printResult(result);
  }

  private static printResult(
    result: ScanbotSDK.CreditCardScanningResult
  ): void {
    console.log("  Detection status: " + result.detectionStatus);
    console.log("  Scanning status: " + result.scanningStatus);

    printGenericDocument(result.creditCard);
  }
}
