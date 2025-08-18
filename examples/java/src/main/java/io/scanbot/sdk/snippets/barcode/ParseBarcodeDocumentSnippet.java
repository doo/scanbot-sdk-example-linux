package io.scanbot.sdk.snippets.barcode;

import io.scanbot.sdk.ScanbotSDK;
import io.scanbot.sdk.barcode.BarcodeDocumentFormats;
import io.scanbot.sdk.barcode.BarcodeDocumentParser;
import io.scanbot.sdk.barcode.BarcodeDocumentParserResult;
import io.scanbot.sdk.licensing.LicenseStatus;
import io.scanbot.sdk.utils.Utils;

public class ParseBarcodeDocumentSnippet {
    public static void run(String str) throws Exception {
        // Make sure you have a valid license
        if(ScanbotSDK.getLicenseInfo().getStatus() != LicenseStatus.OKAY)
            return;

        try (
            BarcodeDocumentParser parser =  new BarcodeDocumentParser(BarcodeDocumentFormats.all)
        ) {
            BarcodeDocumentParserResult result = parser.parse(str);
            
            System.out.println("  Success: " + result.getSuccess());
            Utils.printGenericDocument(result.getParsedDocument());
        }
    }
}
