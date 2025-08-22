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

    // `await using` ensures both scanner and analyzer are properly disposed
    // when the scope ends, as they hold unmanaged resources.
    await using scanner = await ScanbotSDK.DocumentScanner.create(scannerConfig);
    await using analyzer = await ScanbotSDK.DocumentQualityAnalyzer.create(
      analyzeConfig
    );

    const image = await ScanbotSDK.ImageRef.fromPath(filePath);

    // `await using` ensures the result is properly disposed
    // when the scope ends, as it holds unmanaged resources.
    await using result = await scanner.scan(image);
    const detection = result.detectionResult;

    console.log("Detection status: " + detection.status);
    detection.points.forEach((p) => {
      console.log(`x: ${p.x}, y: ${p.y}`);
    });

    // Process cropped image
    const cropped = result.croppedImage;
    if (cropped) {
      if (savePath) {
        await cropped.saveImage(savePath, new ScanbotSDK.SaveImageOptions());
        console.log("Saved to: " + savePath);
      }

      await using result = await analyzer.run(cropped);
      console.log(
        `Found: ${result.documentFound}, Quality: ${result.quality}`
      );
    }
  }
}
