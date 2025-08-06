package io.scanbot.sdk.utils;

import java.io.IOException;
import java.io.InputStream;

import io.scanbot.sdk.image.BufferImageLoadOptions;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.image.PathImageLoadOptions;
import io.scanbot.sdk.io.RandomAccessSource;
import io.scanbot.sdk.multipageimageextractor.MultiPageImageExtractor;
import io.scanbot.sdk.multipageimageextractor.PageExtractionResult;

public class Utils {
    public static ImageRef createImageRef(String filePath, String resourcePath) throws IOException {
        if (filePath != null) {
            return ImageRef.fromPath(filePath, new PathImageLoadOptions());
        }

        try (InputStream stream = Utils.class.getResourceAsStream(resourcePath)) {
            return ImageRef.fromStream(stream, new BufferImageLoadOptions());
        }
    }

    public static PageExtractionResult extractImages(String filePath, String resourcePath) throws Exception {
        try (
            RandomAccessSource source = (filePath != null)
                ? new RandomAccessSource(filePath)
                : new RandomAccessSource(Utils.class.getResourceAsStream(resourcePath));
            var extractor = new MultiPageImageExtractor()
        ) {
            return extractor.run(source);
        }
    }
}
