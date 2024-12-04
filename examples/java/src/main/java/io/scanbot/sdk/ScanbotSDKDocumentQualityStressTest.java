package io.scanbot.sdk;

import io.scanbot.sdk.documentqualityanalyzer.DocumentQuality;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzer;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzerConfiguration;
import io.scanbot.sdk.documentqualityanalyzer.DocumentQualityAnalyzerResult;
import io.scanbot.sdk.image.BufferImageLoadOptions;
import io.scanbot.sdk.image.ImageRef;

import javax.print.Doc;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class ScanbotSDKDocumentQualityStressTest {

    public static void main(String[] args) throws Exception {
        final String licenseKey = "<LICENSE_KEY>";
        ScanbotSDK.initialize(licenseKey);
        String directoryPath = args[0];
        File directory = new File(directoryPath);

        DocumentQualityAnalyzerConfiguration configuration = new DocumentQualityAnalyzerConfiguration();
        configuration.setMinProcessedFraction(1.0);
        configuration.setMaxProcessedFraction(1.0);
        configuration.setMinEstimatedNumberOfSymbolsForDocument(0);
        configuration.setTileSize(2000);


        while (System.in.available() == 0) {
            List<File> files = Arrays.asList(directory.listFiles());
            Collections.shuffle(files);
            for (File file : files) {
                if (System.in.available() > 0) {
                    break;
                }
                try (
                        InputStream inputStream = new FileInputStream(file);
                        ImageRef image = ImageRef.fromStream(inputStream, new BufferImageLoadOptions());
                        DocumentQualityAnalyzer analyzer = new DocumentQualityAnalyzer(configuration)
                ) {
                    DocumentQualityAnalyzerResult result = analyzer.analyzeDocumentQuality(image);
                    DocumentQuality quality = result.getQuality();
                    String qualityString = quality != null ? quality.toString() : "N/A";
                    System.out.println("Analyzed file " + file.getName() + ": " + qualityString);
                }
            }
        }
    }
}
