package io.scanbot.sdk.snippets.document;

import io.scanbot.sdk.image.ImageInfo;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.image.SaveImageOptions;
import io.scanbot.sdk.imageprocessing.DocumentCleanup;
import io.scanbot.sdk.imageprocessing.DocumentCleanupConfiguration;
import io.scanbot.sdk.imageprocessing.DocumentCleanupResult;
import io.scanbot.sdk.utils.Utils;

public class DocumentCleanupSnippet {
    public static void run(
        String filePath,
        String resourcePath,
        String maskFilePath,
        String maskResourcePath,
        String savePath
    ) throws Exception {
        try (
            ImageRef image = Utils.createImageRef(filePath, resourcePath);
            ImageRef mask = Utils.createImageRef(maskFilePath, maskResourcePath);
            DocumentCleanup cleanup = new DocumentCleanup(image, new DocumentCleanupConfiguration());
            DocumentCleanupResult result = cleanup.run(mask)
        ) {
            System.out.println("Document cleanup status: " + result.getStatus());

            ImageInfo originalInfo = image.imageInfo();
            ImageInfo cleanedInfo = result.getImage().imageInfo();
            System.out.printf("Original WxH: %dx%d%n", originalInfo.getWidth(), originalInfo.getHeight());
            System.out.printf("Cleaned  WxH: %dx%d%n", cleanedInfo.getWidth(), cleanedInfo.getHeight());

            if (savePath != null) {
                result.getImage().saveImage(savePath, new SaveImageOptions());
                System.out.println("Saved cleaned image to: " + savePath);
            }
        }
    }
}
