import * as ScanbotSDK from "scanbotsdk";

export class OcrSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    const licenseInfo = await ScanbotSDK.getLicenseInfo();
    if (licenseInfo.status !== "OKAY") {
      console.warn("License is not valid.");
      return;
    }

    await ScanbotSDK.autorelease(async () => {
      const scanner = await ScanbotSDK.OcrEngine.create();
      const page = await scanner.run(image);
      this.printResult(page);
    });
  }

  private static printResult(page: ScanbotSDK.Page): void {
    if (page.blocks) {
      for (const block of page.blocks) {
        console.log(`Block: "${block.text}"`);

        if (block.lines) {
          for (const line of block.lines) {
            console.log(`  Line: "${line.text}"`);

            if (line.words) {
              for (const word of line.words) {
                console.log(`    Word: "${word.text}"`);
              }
            }
          }
        }
      }
    }
  }
}
