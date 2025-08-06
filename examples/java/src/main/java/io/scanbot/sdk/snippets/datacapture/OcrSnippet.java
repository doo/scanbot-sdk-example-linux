package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.ocr.OcrEngine;
import io.scanbot.sdk.ocr.Page;

public class OcrSnippet {
    public static void run(ImageRef image) throws Exception {
        try(
            var scanner = new OcrEngine()
        ) {
            
            var result = scanner.run(image);
            printResult(result);
        }
    }

    private static void printResult(Page result) {

    }
}
