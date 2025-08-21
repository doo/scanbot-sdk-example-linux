import * as ScanbotSDK from "scanbotsdk";
import { extractImages } from "../utils/utils";

export class AnalyzeMultiPageSnippet {
  public static async run(filePath: string): Promise<void> {
    const licenseInfo = await ScanbotSDK.getLicenseInfo();
    if (licenseInfo.status !== "OKAY") {
      console.warn("License is not valid.");
      return;
    }

    await ScanbotSDK.autorelease(async () => {
      const analyzeConfig =
        new ScanbotSDK.DocumentQualityAnalyzerConfiguration();
      analyzeConfig.tileSize = 300;
      analyzeConfig.detectOrientation = true;
      analyzeConfig.minEstimatedNumberOfSymbolsForDocument = 20;
      // configure other parameters as needed

      // Extract images from the multi-page file
      const extractionResult = await extractImages(filePath);

      const analyzer = await ScanbotSDK.DocumentQualityAnalyzer.create(
        analyzeConfig
      );
      const pages = extractionResult.pages;
      console.log("Pages in document: " + pages.length);

      for (let pageIndex = 0; pageIndex < pages.length; pageIndex++) {
        const page = pages[pageIndex];
        const images = page?.images;

        if (images && images.length > 0) {
          for (let imageIndex = 0; imageIndex < images.length; imageIndex++) {
            const extractedImage = images[imageIndex]?.image;
            if (extractedImage) {
              const result = await analyzer.run(extractedImage);
              console.log(
                `Page ${pageIndex + 1}, Image ${imageIndex + 1} -> Found: ${
                  result.documentFound
                }, Quality: ${result.quality}`
              );
            }
          }
        }
      }
    });
  }
}
