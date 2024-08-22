package io.scanbot.sdk.dqa;

import io.scanbot.sdk.ScanbotSDK;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class DocumentQualityAnalyzerExample {

    public static byte[] readAllBytes(String resourceName) throws IOException {
        try (InputStream inputStream = DocumentQualityAnalyzerExample.class.getResourceAsStream(resourceName)) {
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

    public static void main(String[] args) throws IOException {
        final String licenseKey = "<LICENSE_KEY>";
        ScanbotSDK.initialize(licenseKey);

        String filePath = null;
        String resourcePath = null;

        if (args.length == 2) {
            if (args[0].equals("--resource")) {
                resourcePath = args[1];
            } else if (args[0].equals("--file")) {
                filePath = args[1];
            }
            else {
                throw new IllegalArgumentException("You must provide either path to a resource or a file.");
            }
        } else {
            throw new IllegalArgumentException("Incorrect number of arguments.");
        }


        Config config = new Config();
        try (DocumentQualityAnalyzer dqa = new DocumentQualityAnalyzer(config)) {
            AnalysisResult result;
            if (resourcePath != null) {
                byte[] imageBuffer = readAllBytes(resourcePath);
                result = dqa.analyzeDocumentQuality(imageBuffer);
            } else {
                result = dqa.analyzeDocumentQuality(filePath);
            }
            System.out.println("Document quality analysis result:");
            System.out.println("Document found: " + result.isDocumentFound());
            System.out.println("Document quality: " + result.getQuality());
        }
    }

}
