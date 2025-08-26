import * as ScanbotSDK from "scanbotsdk";

export class OcrSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    // `await using` ensures the scanner is properly disposed
    // when the scope ends, as it holds unmanaged resources.
    await using scanner = await ScanbotSDK.OcrEngine.create();
    const page = await scanner.run(image);
    this.printResult(page);
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
