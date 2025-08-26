import * as ScanbotSDK from "scanbotsdk";
import { printGenericDocument } from "../utils/utils";

export class CheckScannerSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    var config = new ScanbotSDK.CheckScannerConfiguration();
    config.documentDetectionMode = "DETECT_AND_CROP_DOCUMENT";
    // Configure other parameters as needed.

    // `await using` ensures both scanner and result are properly disposed
    // when the scope ends, as they hold unmanaged resources.
    await using scanner = await ScanbotSDK.CheckScanner.create(config);
    await using result = await scanner.run(image);

    console.log(`Status: #${result.status}`);

    printGenericDocument(result.check);
  }
}
