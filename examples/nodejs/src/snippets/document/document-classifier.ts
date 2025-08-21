import * as ScanbotSDK from "scanbotsdk";

export class DocumentClassifierSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    const licenseInfo = await ScanbotSDK.getLicenseInfo();
    if (licenseInfo.status !== "OKAY") {
      console.warn("License is not valid.");
      return;
    }

    ScanbotSDK.autorelease(async () => {
      var config = new ScanbotSDK.DocumentClassifierConfiguration();
      const classifier = await ScanbotSDK.DocumentClassifier.create(config);
      const result = await classifier.run(image);

      console.log("Detection status: " + result.status);
      console.log("Type: " + result.documentType);

      result.documentScanningResult.detectionResult.points.forEach((p) => {
        console.log(`x: ${p.x}, y: ${p.y}`);
      });
    });
  }
}
