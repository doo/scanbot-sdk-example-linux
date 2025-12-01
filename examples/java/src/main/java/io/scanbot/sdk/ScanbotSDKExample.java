package io.scanbot.sdk;

import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.image.ImageRefPoolSnapshot;
import io.scanbot.sdk.image.ImageRefProfiler;
import io.scanbot.sdk.licensing.LicenseInfo;
import io.scanbot.sdk.snippets.barcode.*;
import io.scanbot.sdk.snippets.datacapture.*;
import io.scanbot.sdk.snippets.document.*;
import io.scanbot.sdk.utils.*;

import java.util.Arrays;
import java.util.Locale;
import java.util.Map;

public class ScanbotSDKExample {
    public static void main(String[] args) throws Exception {
        if (args.length < 2) {
            ExampleUsage.print();
            return;
        }

        // parse category, subcommand, flags ---
        final String category   = args[0].toLowerCase(Locale.ROOT);
        final String subcommand = args[1].toLowerCase(Locale.ROOT);
        final Map<String, String> f = Utils.parseFlags(Arrays.copyOfRange(args, 2, args.length));
        final String file     = f.get("--file");
        final String resource = f.get("--resource");
        final String save     = f.get("--save");
        final String text     = f.get("--text");
        final String license  = f.get("--license");

        // TODO Add your Scanbot SDK trial license key here.
        final String licenseKey = license != null ? license : "<SCANBOTSDK-LICENSE>";
        final String writablePath = ".";

        ScanbotSDK.initialize(licenseKey, writablePath);

        LicenseInfo licenseInfo = ScanbotSDK.getLicenseInfo();
        System.out.println("License Status: " + licenseInfo.getStatus());

        switch (category) {
            case "scan": {
                if (file == null && resource == null) { ExampleUsage.print(); return; }

                // NOTE: Using try-with-resources on ImageRef is optional, since images are also
                // released when their parent container is closed. However, because images are
                // stored compressed and decompressed on first access, it’s better to close them
                // early to avoid keeping too many decompressed images in memory.
                try (ImageRef image = Utils.createImageRef(file, resource)) {
                    switch (subcommand) {
                        case "barcode":             DetectBarcodesSnippet.run(image); break;
                        case "document":            DetectDocumentSnippet.run(image); break;
                        case "check":               CheckScannerSnippet.run(image); break;
                        case "credit_card":         CreditCardScannerSnippet.run(image); break;
                        case "document_extractor":  DocumentDataExtractorSnippet.run(image); break;
                        case "medical_certificate": MedicalCertificateScannerSnippet.run(image); break;
                        case "mrz":                 MrzScannerSnippet.run(image); break;
                        case "ocr":                 OcrSnippet.run(image); break;
                        case "text_pattern":        TextPatternScannerSnippet.run(image); break;
                        case "vin":                 VINScannerSnippet.run(image); break;
                        default: ExampleUsage.print();
                    }
                }
                break;
            }
            case "analyze": {
                if (file == null && resource == null) { ExampleUsage.print(); return; }
                switch (subcommand) {
                    case "analyze_multi_page":   AnalyzeMultiPageSnippet.run(file, resource); break;
                    case "crop_analyze":        CropAndAnalyzeSnippet.run(file, resource, save); break;
                    default: ExampleUsage.print();
                }
                break;
            }
            case "classify": {
                if (file == null && resource == null) { ExampleUsage.print(); return; }
                try (ImageRef image = Utils.createImageRef(file, resource)) {
                    switch (subcommand) {
                        case "document":   DocumentClassifierSnippet.run(image); break;
                        default: ExampleUsage.print();
                    }
                    break;
                }
            }
            case "parse": {
                if (text == null || text.trim().isEmpty()) { ExampleUsage.print(); return; }
                switch (subcommand) {
                    case "mrz":         MrzParserSnippet.run(text); break;
                    case "barcode_doc": ParseBarcodeDocumentSnippet.run(text); break;
                    default: ExampleUsage.print();
                }
                break;
            }
            default: ExampleUsage.print();
        }

        // This demonstrates that no images remain in memory, after the ImageRef has been closed.
        ImageRefPoolSnapshot snapshot = ImageRefProfiler.makeSnapshot();
        System.out.println("Total memory consumption: " + snapshot.getTotalMemoryConsumption());
        System.out.println("ImageRef profiles: " + snapshot.getImageRefProfiles());
    }
}
