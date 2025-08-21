import * as ScanbotSDK from "scanbotsdk";
import { printGenericDocument } from "../utils/utils";

export class ParseBarcodeDocumentSnippet {
  public static async run(str: string): Promise<void> {
    const licenseInfo = await ScanbotSDK.getLicenseInfo();
    if (licenseInfo.status !== "OKAY") {
      console.warn("License is not valid.");
      return;
    }

    var parser = await ScanbotSDK.BarcodeDocumentParser.create(
      ScanbotSDK.BarcodeDocumentFormats.all
    );
    var result = await parser.parse(str);

    console.log(`Success :${result.success}`);
    printGenericDocument(result.parsedDocument);
  }
}
