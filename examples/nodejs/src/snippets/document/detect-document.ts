import * as ScanbotSDK from "scanbotsdk";

export class DetectDocumentSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    var config = new ScanbotSDK.DocumentScannerConfiguration({
      parameters: {
        acceptedAngleScore: 75, 
        ignoreOrientationMismatch: false
      }
    });
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
