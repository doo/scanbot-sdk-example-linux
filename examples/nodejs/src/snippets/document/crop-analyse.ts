import * as ScanbotSDK from "scanbotsdk";

export class CropAndAnalyzeSnippet {
  public static async run(filePath: string, savePath?: string): Promise<void> {
    const licenseInfo = await ScanbotSDK.getLicenseInfo();
    if (licenseInfo.status !== "OKAY") {
      console.warn("License is not valid.");
      return;
    }
    const scannerParams = new ScanbotSDK.DocumentScannerParameters();
    scannerParams.acceptedAngleScore = 75;
    scannerParams.ignoreOrientationMismatch = false;

    const scannerConfig = new ScanbotSDK.DocumentScannerConfiguration();
    scannerConfig.parameters = scannerParams;

    const analyzeConfig = new ScanbotSDK.DocumentQualityAnalyzerConfiguration();
    analyzeConfig.tileSize = 300;
    analyzeConfig.minEstimatedNumberOfSymbolsForDocument = 20;
    // configure other parameters as needed

    const scanner = await ScanbotSDK.DocumentScanner.create(scannerConfig);
    const analyzer = await ScanbotSDK.DocumentQualityAnalyzer.create(
      analyzeConfig
    );

    await ScanbotSDK.autorelease(async () => {
      const image = await ScanbotSDK.ImageRef.fromPath(filePath);

      const scanResult = await scanner.scan(image);
      const detection = scanResult.detectionResult;

      console.log("Detection status: " + detection.status);
      detection.points.forEach((p) => {
        console.log(`x: ${p.x}, y: ${p.y}`);
      });

      // Process cropped image
      const cropped = scanResult.croppedImage;
      if (cropped) {
        if (savePath) {
          await cropped.saveImage(savePath, new ScanbotSDK.SaveImageOptions());
          console.log("Saved to: " + savePath);
        }

        const result = await analyzer.run(cropped);
        console.log(
          `Found: ${result.documentFound}, Quality: ${result.quality}`
        );
      }
    });
  }
}
