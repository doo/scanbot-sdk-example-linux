package io.scanbot.sdk;

import io.scanbot.sdk.documentscanner.DocumentScanningResult;
import io.scanbot.sdk.documentscanner.DocumentDetectionResult;
import io.scanbot.sdk.documentscanner.DocumentScanner;
import io.scanbot.sdk.documentscanner.DocumentScannerConfiguration;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzer;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzerConfiguration;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzerResult;
import io.scanbot.sdk.image.BufferImageLoadOptions;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.image.PathImageLoadOptions;
import io.scanbot.sdk.image.SaveImageOptions;
import io.scanbot.sdk.io.RandomAccessSource;
import io.scanbot.sdk.multipageimageextractor.ExtractedPage;
import io.scanbot.sdk.multipageimageextractor.MultiPageImageExtractor;
import io.scanbot.sdk.multipageimageextractor.PageExtractionResult;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

public class ScanbotSDKExample {

    private static void printUsage() {
        System.out.println("Scanbot SDK Example");
        System.out.println("Usage:");
        System.out.println();
        System.out.println("   ./gradlew run --args='<command> --resource <path/to/bundle/resource.jpg> --save <path/to/save/image.jpg>'");
        System.out.println();
        System.out.println("or");
        System.out.println();
        System.out.println("   ./gradlew run --args='<command> --file <path/to/file.jpg> --save <path/to/save/image.jpg>'");
        System.out.println();
        System.out.println("   Available commands: detectDocument, analyzeMultiPageDocument, cropAndAnalyzeDocument");
        System.out.println();
        System.out.println("   --save argument is optional and is applicable only to cropAndAnalyzeDocument command");
        System.out.println();
    }

    private static ImageRef createImageRef(String filePath, String resourcePath) throws IOException {
        if (filePath != null) {
            return ImageRef.fromPath(filePath, new PathImageLoadOptions());
        } else {
            try (InputStream inputStream = ScanbotSDKExample.class.getResourceAsStream(resourcePath)) {
                return ImageRef.fromStream(inputStream, new BufferImageLoadOptions());
            }
        }
    }

    private static RandomAccessSource createRandomAccessSource(String filePath, String resourcePath) throws IOException {
        if (filePath != null) {
            return new RandomAccessSource(filePath);
        } else {
            try (InputStream inputStream = ScanbotSDKExample.class.getResourceAsStream(resourcePath)) {
                return new RandomAccessSource(inputStream);
            }
        }
    }

    private static PageExtractionResult extractImages(String filePath, String resourcePath) throws Exception {
        try (RandomAccessSource source = createRandomAccessSource(filePath, resourcePath);
             MultiPageImageExtractor extractor = new MultiPageImageExtractor()) {
            return extractor.run(source);
        }
    }

    private static void analyzeMultiPageDocument(String filePath, String resourcePath) throws Exception {
        try (PageExtractionResult pageExtractionResult = extractImages(filePath, resourcePath);
             DocumentQualityAnalyzer documentQualityAnalyzer = new DocumentQualityAnalyzer(new DocumentQualityAnalyzerConfiguration())) {
            System.out.println("Pages in document: " + pageExtractionResult.getPages().size());
            for (int i = 0; i < pageExtractionResult.getPages().size(); ++i) {
                ExtractedPage page = pageExtractionResult.getPages().get(i);
                System.out.println("Page " + (i + 1) + " contains " + page.getImages().size() + " image(s)");
                for (int j = 0; j < page.getImages().size(); ++j) {
                    // IMPORTANT NOTE
                    // Using try-with-resources here is not mandatory cause in the worst case all the images
                    // are closed when the pageExtractionResult is closed at the end of the outer try-with-resources block.
                    // However, the images originally are usually stored in a compressed format internally and are decompressed when accessed for the first time.
                    // To prevent storing all the decompressed images in memory at the same time, it is recommended to close the images as soon as they are not needed anymore.
                    try (ImageRef imageRef = page.getImages().get(j).getImage()) {
                        DocumentQualityAnalyzerResult result = documentQualityAnalyzer.analyzeDocumentQuality(imageRef);
                        System.out.println("Analyzing image " + (j + 1) + " on page " + (i + 1));
                        System.out.println("Document found: " + result.getDocumentFound());
                        System.out.println("Document quality: " + result.getQuality());
                    }
                }
            }
        }
    }

    private static void cropAndAnalyzeDocument(String filePath, String resourcePath, String savePath) throws Exception {
        try (ImageRef imageRef = createImageRef(filePath, resourcePath);
             DocumentScanner scanner = new DocumentScanner(new DocumentScannerConfiguration());
             DocumentQualityAnalyzer analyzer = new DocumentQualityAnalyzer(new DocumentQualityAnalyzerConfiguration());
             DocumentScanningResult scanningResult = scanner.scan(imageRef)) {

            DocumentDetectionResult detectionResult = scanningResult.getDetectionResult();
            System.out.println("Document detection result:");
            System.out.println("Detection status: " + detectionResult.getStatus());
            if (!detectionResult.getPoints().isEmpty()) {
                System.out.println("Detected document coordinates:");
                for (int i = 0; i < detectionResult.getPoints().size(); i++) {
                    System.out.println("x: " + detectionResult.getPoints().get(i).getX() + ", y: " + detectionResult.getPoints().get(i).getY());
                }
            }
            ImageRef crop = scanningResult.getCroppedImage();
            if (crop != null) {
                if (savePath != null) {
                    crop.saveImage(savePath, new SaveImageOptions());
                    System.out.println("Cropped document saved to: " + savePath);
                }
                System.out.println("Analyzing cropped document...");
                DocumentQualityAnalyzerResult result = analyzer.analyzeDocumentQuality(crop);
                System.out.println("Document quality analysis result:");
                System.out.println("Document found: " + result.getDocumentFound());
                System.out.println("Document quality: " + result.getQuality());
            }
        }
    }

    private static void detectDocument(String filePath, String resourcePath) throws Exception {
        try (ImageRef imageRef = createImageRef(filePath, resourcePath);
             DocumentScanner documentScanner = new DocumentScanner(new DocumentScannerConfiguration())) {
            DocumentDetectionResult result = documentScanner.detect(imageRef);
            System.out.println("Document detection result:");
            System.out.println("Detection status: " + result.getStatus());
            if (!result.getPoints().isEmpty()) {
                System.out.println("Detected document coordinates:");
                for (int i = 0; i < result.getPoints().size(); i++) {
                    System.out.println("x: " + result.getPoints().get(i).getX() + ", y: " + result.getPoints().get(i).getY());
                }
            }
        }

    }

    public static void main(String[] args) throws Exception {
        // TODO Add your Scanbot SDK trial license key here.
        // The SDK and a trial license are available on request via beta@scanbot.io
        final String licenseKey = "<YOUR_LICENSE_KEY>";

        ScanbotSDK.initialize(licenseKey);
        String filePath = null;
        String resourcePath = null;

        if (args.length < 1) {
            printUsage();
            return;
        }

        Map<String, String> argsMap = new HashMap<>();
        for (int i = 1; i + 1 < args.length; i += 2) {
            argsMap.put(args[i], args[i + 1]);
        }

        if (argsMap.containsKey("--resource")) {
            resourcePath = argsMap.get("--resource");
        } else if (argsMap.containsKey("--file")) {
            filePath = argsMap.get("--file");
        } else {
            printUsage();
            return;
        }

        String savePath = null;
        if (argsMap.containsKey("--save")) {
            savePath = argsMap.get("--save");
        }

        String command = args[0];
        switch (command.toLowerCase()) {
            case "detectdocument":
                detectDocument(filePath, resourcePath);
                break;
            case "analyzemultipagedocument":
                analyzeMultiPageDocument(filePath, resourcePath);
                break;
            case "cropandanalyzedocument":
                cropAndAnalyzeDocument(filePath, resourcePath, savePath);
                break;
            default:
                printUsage();
        }
    }

}
