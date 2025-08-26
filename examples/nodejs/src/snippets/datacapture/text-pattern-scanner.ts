import * as ScanbotSDK from "scanbotsdk";

export class TextPatternScannerSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    const config = new ScanbotSDK.TextPatternScannerConfiguration();
    config.minimumNumberOfRequiredFramesWithEqualScanningResult = 2;
    config.validator = new ScanbotSDK.DefaultContentValidator();
    // configure other parameters as needed

    // `await using` ensures the scanner is properly disposed
    // when the scope ends, as it holds unmanaged resources.  
    await using scanner = await ScanbotSDK.TextPatternScanner.create(config);
    const result = await scanner.run(image);

    console.log("Raw Text: " + result.rawText);
  }
}
