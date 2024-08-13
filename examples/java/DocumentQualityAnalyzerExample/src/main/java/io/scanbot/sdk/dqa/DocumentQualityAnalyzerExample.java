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
        Config config = new Config();
        try (DocumentQualityAnalyzer dqa = new DocumentQualityAnalyzer(config)) {
            AnalysisResult result = dqa.analyzeDocumentQuality(readAllBytes("/img.png"));
            System.out.println("Document quality analysis result:");
            System.out.println("Document found: " + result.isDocumentFound());
            System.out.println("Document quality: " + result.getQuality());
        }
    }

}
