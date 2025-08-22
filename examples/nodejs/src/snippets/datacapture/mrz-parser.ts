import * as ScanbotSDK from "scanbotsdk";
import { printGenericDocument } from "../utils/utils";

export class MrzParserSnippet {
  public static async run(rawMrz: string): Promise<void> {
    const licenseInfo = await ScanbotSDK.getLicenseInfo();
    if (licenseInfo.status !== "OKAY") {
      console.warn("License is not valid.");
      return;
    }

    const config = new ScanbotSDK.MrzParserConfiguration();
    config.incompleteResultHandling = "ACCEPT";
    // Configure other parameters as needed.

    // `await using` ensures both parser and result are properly disposed
    // when the scope ends, as they hold unmanaged resources.
    await using parser = await ScanbotSDK.MrzParser.create(config);
    await using result = await parser.parse(rawMrz);

    console.log("MRZ Parser Result:");
    console.log("  Success: " + result.success);
    console.log("  Raw MRZ: " + result.rawMRZ);

    printGenericDocument(result.document);
  }
}
