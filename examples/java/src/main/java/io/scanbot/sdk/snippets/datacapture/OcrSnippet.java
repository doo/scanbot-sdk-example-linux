package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.ocr.Block;
import io.scanbot.sdk.ocr.Line;
import io.scanbot.sdk.ocr.OcrEngine;
import io.scanbot.sdk.ocr.Page;
import io.scanbot.sdk.ocr.Word;

public class OcrSnippet {
    public static void run(ImageRef image) throws Exception {
        try(
            OcrEngine scanner = new OcrEngine()
        ) {
            Page result = scanner.run(image);
            printResult(result);
        }
    }

    private static void printResult(Page page) {
        if (page == null) {
            System.out.println("Page: None");
            return;
        }

        for (Block block : page.getBlocks()) {
            System.out.printf("Block: \"%s\"%n", block.getText());
            for (Line line : block.getLines()) {
                System.out.printf("  Line: \"%s\"%n", line.getText());
                for (Word word : line.getWords()) {
                    System.out.printf(
                        "    Word: \"%s\" (Confidence: %.2f)%n",
                        word.getText(),
                        word.getConfidence()
                    );
                }
            }
        }
    }

}
