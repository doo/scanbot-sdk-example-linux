import * as ScanbotSDK from "scanbotsdk";

export class TextPatternScannerSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    const licenseInfo = await ScanbotSDK.getLicenseInfo();
    if (licenseInfo.status !== "OKAY") {
      console.warn("License is not valid.");
      return;
    }

    await ScanbotSDK.autorelease(async () => {
      const config = new ScanbotSDK.TextPatternScannerConfiguration();
      config.minimumNumberOfRequiredFramesWithEqualScanningResult = 2;
      config.validator = new ScanbotSDK.DefaultContentValidator();
      // configure other parameters as needed

      const scanner = await ScanbotSDK.TextPatternScanner.create(config);
      const result = await scanner.run(image);

      console.log("Raw Text: " + result.rawText);
    });
  }
}
