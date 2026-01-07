import * as ScanbotSDK from "scanbotsdk";

export class VinScannerSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    const config = new ScanbotSDK.VinScannerConfiguration();
    config.extractVINFromBarcode = true;
    // configure other parameters as needed

    // `await using` ensures the scanner is properly disposed
    // when the scope ends, as it holds unmanaged resources.
    await using scanner = await ScanbotSDK.VinScanner.create(config);
    const result = await scanner.run(image);

    this.printResult(result);
  }

  private static printResult(result: ScanbotSDK.VinScannerResult): void {
    console.log("Text: " +  result.textResult.rawText);
    console.log("Extracted VIN: " + result.barcodeResult.extractedVIN);``
  }
}
