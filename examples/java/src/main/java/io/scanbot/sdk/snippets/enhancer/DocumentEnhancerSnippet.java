package io.scanbot.sdk.snippets.enhancer;
import java.util.List;

import io.scanbot.sdk.documentscanner.DocumentEnhancer;
import io.scanbot.sdk.documentscanner.DocumentStraighteningMode;
import io.scanbot.sdk.documentscanner.DocumentStraighteningParameters;
import io.scanbot.sdk.documentscanner.DocumentStraighteningResult;
import io.scanbot.sdk.geometry.AspectRatio;
import io.scanbot.sdk.image.ImageRef;

public class DocumentEnhancerSnippet {
   public static void run(ImageRef image) throws Exception {
        DocumentStraighteningParameters params = new DocumentStraighteningParameters();
        params.setStraighteningMode(DocumentStraighteningMode.STRAIGHTEN);
        params.setAspectRatios(List.of(
            new AspectRatio(5.0, 7.0),
            new AspectRatio(1.0, 1.0),
            new AspectRatio(16.0, 9.0),
            new AspectRatio(3.0, 4.0)
        ));

        try (
            DocumentEnhancer enhancer = new DocumentEnhancer();
            DocumentStraighteningResult result = enhancer.straighten(image, params, List.of())
        ) {
            // The straightened image can be accessed via result.getStraightenedImage() and saved or further processed as needed.
        }
    }
}
