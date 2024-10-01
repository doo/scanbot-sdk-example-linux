package io.scanbot.sdk;

import io.scanbot.sdk.documentdetector.DocumentDetectAndCropResult;
import io.scanbot.sdk.documentdetector.DocumentDetectionResult;
import io.scanbot.sdk.documentdetector.DocumentDetector;
import io.scanbot.sdk.documentdetector.DocumentDetectorConfiguration;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzer;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzerConfiguration;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzerResult;
import io.scanbot.sdk.image.BufferImageLoadOptions;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.image.PathImageLoadOptions;
import io.scanbot.sdk.image.SaveImageOptions;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

public class ScanbotSDKExample {

    private static byte[] readAllBytes(String resourceName) throws IOException {
        try (InputStream inputStream = ScanbotSDKExample.class.getResourceAsStream(resourceName)) {
            final int bufLen = 1024;
            byte[] buf = new byte[bufLen];
            int readLen;

            ByteArrayOutputStream outputStream = new ByteArrayOutputStream();

            while ((readLen = inputStream.read(buf, 0, bufLen)) != -1) {
                outputStream.write(buf, 0, readLen);
            }

            return outputStream.toByteArray();
        }
    }

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
        System.out.println("   Available commands: detectDocument, analyzeDocument, cropAndAnalyzeDocument");
        System.out.println();
        System.out.println("   --save argument is optional and is applicable only to cropAndAnalyzeDocument command");
        System.out.println();
    }

    private static ImageRef createImageRef(String filePath, String resourcePath) throws IOException {
        if (filePath != null) {
            return ImageRef.fromPath(filePath, new PathImageLoadOptions());
        } else {
            byte[] imageBuffer = readAllBytes(resourcePath);
            return ImageRef.fromEncodedBuffer(imageBuffer, new BufferImageLoadOptions());
        }

    }

    private static void analyzeDocument(ImageRef imageRef) throws IOException {
        try (DocumentQualityAnalyzer documentQualityAnalyzer = new DocumentQualityAnalyzer(new DocumentQualityAnalyzerConfiguration())) {
            DocumentQualityAnalyzerResult result = documentQualityAnalyzer.analyzeDocumentQuality(imageRef);
            System.out.println("Document quality analysis result:");
            System.out.println("Document found: " + result.getDocumentFound());
            System.out.println("Document quality: " + result.getQuality());
        }
    }

    private static void cropAndAnalyzeDocument(ImageRef imageRef, String savePath) {
        try (DocumentDetector detector = new DocumentDetector(new DocumentDetectorConfiguration());
             DocumentQualityAnalyzer analyzer = new DocumentQualityAnalyzer(new DocumentQualityAnalyzerConfiguration());
             DocumentDetectAndCropResult detectAndCropResult = detector.detectAndCrop(imageRef)) {

            DocumentDetectionResult detectionResult = detectAndCropResult.getDetectionResult();
            System.out.println("Document detection result:");
            System.out.println("Detection status: " + detectionResult.getStatus());
            if (!detectionResult.getPoints().isEmpty()) {
                System.out.println("Detected document coordinates:");
                for (int i = 0; i < detectionResult.getPoints().size(); i++) {
                    System.out.println("x: " + detectionResult.getPoints().get(i).getX() + ", y: " + detectionResult.getPoints().get(i).getY());
                }
            }
            ImageRef crop = detectAndCropResult.getCroppedImage();
            if (crop != null) {
                if (savePath != null) {
                    boolean saved = crop.saveImage(savePath, new SaveImageOptions());
                    if (saved) {
                        System.out.println("Cropped document saved to: " + savePath);
                    }
                }
                System.out.println("Analyzing cropped document...");
                DocumentQualityAnalyzerResult result = analyzer.analyzeDocumentQuality(crop);
                System.out.println("Document quality analysis result:");
                System.out.println("Document found: " + result.getDocumentFound());
                System.out.println("Document quality: " + result.getQuality());
            }
        }
    }

    private static void detectDocument(ImageRef imageRef) throws IOException {
        try (DocumentDetector documentDetector = new DocumentDetector(new DocumentDetectorConfiguration())) {
            DocumentDetectionResult result = documentDetector.detect(imageRef);
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

    public static void main(String[] args) throws IOException {
        final String licenseKey = "<LICENSE_KEY>";
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

        try(ImageRef imageRef = createImageRef(filePath, resourcePath)) {
            String command = args[0];
            switch (command.toLowerCase()) {
                case "detectdocument":
                    detectDocument(imageRef);
                    break;
                case "analyzedocument":
                    analyzeDocument(imageRef);
                    break;
                case "cropandanalyzedocument":
                    cropAndAnalyzeDocument(imageRef, savePath);
                    break;
                default: printUsage();
            }
        }
    }

}
