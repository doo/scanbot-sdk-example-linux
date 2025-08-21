import * as ScanbotSDK from "scanbotsdk";

export class VinScannerSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    const licenseInfo = await ScanbotSDK.getLicenseInfo();
    if (licenseInfo.status !== "OKAY") {
      console.warn("License is not valid.");
      return;
    }

    await ScanbotSDK.autorelease(async () => {
      const config = new ScanbotSDK.VinScannerConfiguration();
      config.extractVINFromBarcode = true;
      // configure other parameters as needed

      const scanner = await ScanbotSDK.VinScanner.create(config);
      const result = await scanner.run(image);

      this.printResult(result);
    });
  }

  private static printResult(result: ScanbotSDK.VinScannerResult): void {
    console.log("Text: " +  result.textResult.rawText);
    console.log("Extracted VIN: " + result.barcodeResult.extractedVIN);``
  }
}
