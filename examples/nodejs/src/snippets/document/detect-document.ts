import * as ScanbotSDK from "scanbotsdk";

export class DetectDocumentSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    const licenseInfo = await ScanbotSDK.getLicenseInfo();
    if (licenseInfo.status !== "OKAY") {
      console.warn("License is not valid.");
      return;
    }

    ScanbotSDK.autorelease(async () => {
      var config = new ScanbotSDK.DocumentScannerConfiguration();
      const scanner = await ScanbotSDK.DocumentScanner.create(config);
      const result = await scanner.run(image);

      console.log("Detection status: " + result.status);

      result.points.forEach((p) => {
        console.log(`x: ${p.x}, y: ${p.y}`);
      });
    });
  }
}
