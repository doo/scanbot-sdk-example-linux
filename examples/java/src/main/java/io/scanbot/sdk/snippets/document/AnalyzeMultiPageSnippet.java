package io.scanbot.sdk.snippets.document;

import java.util.List;

import io.scanbot.sdk.ScanbotSDK;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzer;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzerConfiguration;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzerResult;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.licensing.LicenseStatus;
import io.scanbot.sdk.multipageimageextractor.ExtractedImage;
import io.scanbot.sdk.multipageimageextractor.ExtractedPage;
import io.scanbot.sdk.multipageimageextractor.PageExtractionResult;
import io.scanbot.sdk.utils.Utils;

public class AnalyzeMultiPageSnippet {
    public static void run(String filePath, String resourcePath) throws Exception {
        // Make sure you have a valid license
        if(ScanbotSDK.getLicenseInfo().getStatus() != LicenseStatus.OKAY)
            return;

        DocumentQualityAnalyzerConfiguration analyse_config = new DocumentQualityAnalyzerConfiguration();
        analyse_config.setTileSize(300);
        analyse_config.setDetectOrientation(true);
        analyse_config.setMinEstimatedNumberOfSymbolsForDocument(20);
        // Configure other parameters as needed.

        try (
            PageExtractionResult extractionResult = Utils.extractImages(filePath, resourcePath);
            DocumentQualityAnalyzer analyzer = new DocumentQualityAnalyzer(analyse_config)
        ) {
            List<ExtractedPage> pages = extractionResult.getPages();
            System.out.println("Pages in document: " + pages.size());

            for (int pageIndex = 0; pageIndex < pages.size(); pageIndex++) {
                ExtractedPage page = pages.get(pageIndex);
                List<ExtractedImage> images = page.getImages();

                for (int imageIndex = 0; imageIndex < images.size(); imageIndex++) {
                    try (ImageRef image = images.get(imageIndex).getImage()) {
                        DocumentQualityAnalyzerResult result = analyzer.run(image);
                        System.out.printf("Page %d, Image %d -> Found: %b, Quality: %s%n",
                                pageIndex + 1, imageIndex + 1,
                                result.getDocumentFound(), result.getQuality());
                    }
                }
            }
        }
    }
}
