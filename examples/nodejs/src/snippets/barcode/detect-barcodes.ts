import * as ScanbotSDK from "scanbotsdk";
import { printGenericDocument } from "../utils/utils";

export class DetectBarcodesSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    const licenseInfo = await ScanbotSDK.getLicenseInfo();
    if (licenseInfo.status !== "OKAY") {
      console.warn("License is not valid.");
      return;
    }

    ScanbotSDK.autorelease(async () => {
      var config = new ScanbotSDK.BarcodeScannerConfiguration();
      // Configure other parameters as needed.

      const scanner = await ScanbotSDK.BarcodeScanner.create(config);
      const result = await scanner.run(image);
      const barcodes = result.barcodes ?? [];
      if (barcodes.length === 0) {
        console.log("No barcodes found.");
        return;
      }

      barcodes.forEach((barcode, idx) => {
        // If you want to use the image later, call barcode.sourceImage?.encodeImage(...) and save the returned buffer.
        // Otherwise, the image reference will be released once the ImageRef object is closed or garbage-collected.

        console.log(`Barcode #${idx + 1}:`);
        console.log(`  Format: ${barcode.format}`);
        console.log(`  Text:   ${barcode.text}`);

        printGenericDocument(barcode.extractedDocument);
      });
    });
  }
}
