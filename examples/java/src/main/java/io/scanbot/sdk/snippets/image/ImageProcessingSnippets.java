package io.scanbot.sdk.snippets.image;

import java.io.IOException;

import io.scanbot.sdk.documentscanner.DocumentDetectionResult;
import io.scanbot.sdk.documentscanner.DocumentScanner;
import io.scanbot.sdk.documentscanner.DocumentScannerConfiguration;
import io.scanbot.sdk.exception.InvalidImageRefException;
import io.scanbot.sdk.image.ImageInfo;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.image.ImageRotation;
import io.scanbot.sdk.image.SaveImageOptions;
import io.scanbot.sdk.imageprocessing.BinarizationFilterPreset;
import io.scanbot.sdk.imageprocessing.CustomBinarizationFilter;
import io.scanbot.sdk.imageprocessing.ImageProcessor;

public class ImageProcessingSnippets {

    public static void saveImage(ImageRef image, String path) throws IOException, InvalidImageRefException {
        SaveImageOptions options = new SaveImageOptions();
        options.setQuality(80);
        image.saveImage(path, options);
    }

    public static void rotate90(ImageRef image) throws Exception {
        try (ImageProcessor processor = new ImageProcessor();
             ImageRef rotated = processor.rotate(image, ImageRotation.CLOCKWISE_90)) {

            ImageInfo originalInfo = image.imageInfo();
            ImageInfo rotatedInfo = rotated.imageInfo();

            System.out.println("Original WxH: " + originalInfo.getWidth() + "x" + originalInfo.getHeight());
            System.out.println("Rotated 90° WxH: " + rotatedInfo.getWidth() + "x" + rotatedInfo.getHeight());
        }
    }

    public static void rotate180(ImageRef image) throws Exception {
        try (ImageProcessor processor = new ImageProcessor();
             ImageRef rotated = processor.rotate(image, ImageRotation.CLOCKWISE_180)) {

            ImageInfo rotatedInfo = rotated.imageInfo();
            System.out.println("Rotated 180° WxH: " + rotatedInfo.getWidth() + "x" + rotatedInfo.getHeight());
        }
    }

    public static void resize(ImageRef image, int maxSize) throws Exception {
        try (ImageProcessor processor = new ImageProcessor();
             ImageRef resized = processor.resize(image, maxSize)) {

            ImageInfo resizedInfo = resized.imageInfo();
            System.out.println("Resized WxH: " + resizedInfo.getWidth() + "x" + resizedInfo.getHeight());
        }
    }

    public static void applyBinarizationFilter(ImageRef image, String path) throws Exception {
        CustomBinarizationFilter filter = new CustomBinarizationFilter();
        filter.setPreset(BinarizationFilterPreset.PRESET_4);

        try (ImageProcessor processor = new ImageProcessor();
            ImageRef filteredImage = processor.applyFilter(image, filter)) {

            filteredImage.saveImage(path, new SaveImageOptions());
        }
    }

    public static void detectAndCropDocument(ImageRef image) throws Exception {
        DocumentScannerConfiguration config = new DocumentScannerConfiguration();
        
        try (ImageProcessor processor = new ImageProcessor();
             DocumentScanner scanner = new DocumentScanner(config)) {

            DocumentDetectionResult detectionResult = scanner.run(image);
            System.out.println("Detection status: " + detectionResult.getStatus());
            System.out.println("Detected points: " + detectionResult.getPoints().size());

            try (ImageRef cropped = processor.crop(image, detectionResult.getPointsNormalized())) {
                ImageInfo croppedInfo = cropped.imageInfo();
                System.out.println("Cropped WxH: " + croppedInfo.getWidth() + "x" + croppedInfo.getHeight());
            }
        }
    }
}
