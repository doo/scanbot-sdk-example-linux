import * as ScanbotSDK from "scanbotsdk";

export class DetectDocumentSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    var scanner_params = new ScanbotSDK.DocumentScannerParameters();
    scanner_params.acceptedAngleScore = 75;
    scanner_params.ignoreOrientationMismatch = false;

    var config = new ScanbotSDK.DocumentScannerConfiguration();
    config.parameters = scanner_params
    // Configure other parameters as needed.
    
    // `await using` ensures the scanner is properly disposed
    // when the scope ends, as it holds unmanaged resources.
    await using scanner = await ScanbotSDK.DocumentScanner.create(config);
    const result = await scanner.run(image);

    console.log("Detection status: " + result.status);

    result.points.forEach((p) => {
      console.log(`x: ${p.x}, y: ${p.y}`);
    });
  }
}
