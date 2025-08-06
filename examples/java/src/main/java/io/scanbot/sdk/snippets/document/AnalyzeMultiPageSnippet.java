package io.scanbot.sdk.snippets.document;

import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzer;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzerConfiguration;
import io.scanbot.sdk.utils.Utils;

public class AnalyzeMultiPageSnippet {
    public static void run(String filePath, String resourcePath) throws Exception {
        try (
            var extractionResult = Utils.extractImages(filePath, resourcePath);
            var analyzer = new DocumentQualityAnalyzer(new DocumentQualityAnalyzerConfiguration())
        ) {
            var pages = extractionResult.getPages();
            System.out.println("Pages in document: " + pages.size());

            for (int pageIndex = 0; pageIndex < pages.size(); pageIndex++) {
                var page = pages.get(pageIndex);
                var images = page.getImages();

                for (int imageIndex = 0; imageIndex < images.size(); imageIndex++) {
                    try (var image = images.get(imageIndex).getImage()) {
                        var result = analyzer.run(image);
                        System.out.printf("Page %d, Image %d -> Found: %b, Quality: %s%n",
                                pageIndex + 1, imageIndex + 1,
                                result.getDocumentFound(), result.getQuality());
                    }
                }
            }
        }
    }
}
