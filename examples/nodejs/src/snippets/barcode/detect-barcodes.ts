import * as ScanbotSDK from "scanbotsdk";
import { printGenericDocument } from "../utils/utils";

export class DetectBarcodesSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    var config = new ScanbotSDK.BarcodeScannerConfiguration();
    var formatConfig = new ScanbotSDK.BarcodeFormatCommonConfiguration();
    formatConfig.gs1Handling = "DECODE_FULL"
    
    config.barcodeFormatConfigurations = [formatConfig];
    config.returnBarcodeImage = true;
    // Configure other parameters as needed.

    // `await using` ensures both scanner and result are properly disposed
    // when the scope ends, as they hold unmanaged resources.
    await using scanner = await ScanbotSDK.BarcodeScanner.create(config);
    await using result = await scanner.run(image);
    const barcodes = result.barcodes ?? [];
    if (barcodes.length === 0) {
      console.log("No barcodes found.");
      return;
    }

    barcodes.forEach((barcode, idx) => {
      console.log(`Barcode #${idx + 1}:`);
      console.log(`  Format: ${barcode.format}`);
      console.log(`  Text:   ${barcode.text}`);

      printGenericDocument(barcode.extractedDocument);
    });
  }
}
