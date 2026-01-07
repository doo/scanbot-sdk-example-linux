import * as ScanbotSDK from "scanbotsdk";
import { extractImages } from "../utils/utils";

export class AnalyzeMultiPageSnippet {
  public static async run(filePath: string): Promise<void> {
    const analyzeConfig = new ScanbotSDK.DocumentQualityAnalyzerConfiguration();
    analyzeConfig.processByTileConfiguration.tileSize = 300;
    analyzeConfig.detectOrientation = true;
    analyzeConfig.minEstimatedNumberOfSymbolsForDocument = 20;
    // configure other parameters as needed

    // Extract images from the multi-page file
    await using extractionResult = await extractImages(filePath);

    // `await using` ensures the analyser is properly disposed
    // when the scope ends, as it holds unmanaged resources.
    await using analyzer = await ScanbotSDK.DocumentQualityAnalyzer.create(analyzeConfig);
    const pages = extractionResult.pages;
    console.log("Pages in document: " + pages.length);

    for (let pageIndex = 0; pageIndex < pages.length; pageIndex++) {
      const images = pages[pageIndex]!.images;

      for (let imageIndex = 0; imageIndex < images.length; imageIndex++) {
        // NOTE: Using `await using` on ImageRef is optional, because images are also
        // released when their parent container is disposed. However, images are stored
        // compressed and only decompressed on first access. Disposing them early
        // prevents a large number of decompressed images from piling up in memory.
        await using extractedImage = images[imageIndex]?.image;
        if (extractedImage) {
          // `await using` ensures the result is properly disposed when the scope ends, as it holds unmanaged resources.
          await using result = await analyzer.run(extractedImage);
          console.log(
            `Page ${pageIndex + 1}, Image ${imageIndex + 1} -> Found: ${
              result.documentFound
            }, Quality: ${result.quality}`
          );
        }
      }
    }
  }
}
