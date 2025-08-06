package io.scanbot.sdk;

import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.snippets.barcode.*;
import io.scanbot.sdk.snippets.datacapture.*;
import io.scanbot.sdk.snippets.document.*;
import io.scanbot.sdk.utils.*;

import java.util.HashMap;
import java.util.Map;

public class ScanbotSDKExample {
    public static void main(String[] args) throws Exception {
        // TODO Add your Scanbot SDK trial license key here.
        // The SDK and a trial license are available on request via beta@scanbot.io
        final String licenseKey = "<YOUR_LICENSE_KEY>";

        final String writablePath = ".";
        ScanbotSDK.initialize(licenseKey, writablePath);
        ScanbotSDK.waitForOnlineLicenseCheckCompletion(15_000);

        var licenseInfo = ScanbotSDK.getLicenseInfo();
        System.out.println("License Status: " + licenseInfo.getStatus());

        boolean isFloatingLicense = licenseInfo.getDevices() != null;
        if (isFloatingLicense) {
            System.out.println("Using floating license with " + licenseInfo.getDevices() + " devices. Do not forget to call " +
                    "io.scanbot.sdk.ScanbotSDK.deregisterDevice and io.scanbot.sdk.ScanbotSDK.waitForOnlineLicenseCheckCompletion when you no " +
                    "longer need the license or use io.scanbot.sdk.ScanbotSDK.DeviceSession convenience class with try-with-resources pattern.");
        }

        // If you are not using floating license, it is not required to use io.scanbot.sdk.DeviceSession as there is no
        // need to notify server you are no longer using the license. Alternatively, you can manually call
        // io.scanbot.sdk.ScanbotSDK.deregisterDevice and io.scanbot.sdk.ScanbotSDK.waitForOnlineLicenseCheckCompletion if you need asynchronous
        // deregistration behaviour
        try (
            DeviceSession deviceSession = new DeviceSession(DeviceSession.DEFAULT_CLOSE_TIMEOUT_MS)
        ) {
            if (args.length < 1) {
                ExampleUsage.print();
                return;
            }

            Map<String, String> argsMap = new HashMap<>();
            for (int i = 1; i + 1 < args.length; i += 2) {
                argsMap.put(args[i], args[i + 1]);
            }

            String command = args[0].toLowerCase();
            String filePath = argsMap.get("--file");
            String resourcePath = argsMap.get("--resource");
            String savePath = argsMap.get("--save");

            if (filePath == null && resourcePath == null) {
                ExampleUsage.print();
                return;
            }

            try (
                ImageRef image = Utils.createImageRef(filePath, resourcePath);
            ) {
                switch (command) {
                    case "detectbarcode":
                        DetectBarcodesSnippet.run(image);
                        break;
                    case "detectdocument":
                        DetectDocumentSnippet.run(image);
                        break;
                    case "analyzemultipagedocument":
                        AnalyzeMultiPageSnippet.run(filePath, resourcePath);
                        break;
                    case "cropandanalyzedocument":
                        CropAndAnalyzeSnippet.run(image, savePath);
                        break;
                    case "check":
                        CheckScannerSnippet.run(image);
                        break;
                    case "creditcard":
                        CreditCardScannerSnippet.run(image);
                        break;
                    case "documentextractor":
                        DocumentDataExtractorSnippet.run(image);
                        break;
                    case "medicalcertificate":
                        MedicalCertificateScannerSnippet.run(image);
                        break;
                    case "mrz":
                        MrzScannerSnippet.run(image);
                        break;
                    case "ocr":
                        OcrSnippet.run(image);
                        break;
                    case "textpattern":
                        TextPatternScannerSnippet.run(image);
                        break;
                    case "vin":
                        VINScannerSnippet.run(image);
                        break;
                    default:
                        ExampleUsage.print();
                }
            }
        }
    }
}
