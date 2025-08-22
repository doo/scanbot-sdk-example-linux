package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.ScanbotSDK;
import io.scanbot.sdk.licensing.LicenseStatus;
import io.scanbot.sdk.mrz.MrzIncompleteResultHandling;
import io.scanbot.sdk.mrz.MrzParser;
import io.scanbot.sdk.mrz.MrzParserConfiguration;
import io.scanbot.sdk.mrz.MrzScannerResult;
import io.scanbot.sdk.utils.Utils;

public class MrzParserSnippet {
    public static void run(String str) throws Exception {
        // Make sure you have a valid license
        if(ScanbotSDK.getLicenseInfo().getStatus() != LicenseStatus.OKAY)
            return;

        MrzParserConfiguration config = new MrzParserConfiguration();
        config.setIncompleteResultHandling(MrzIncompleteResultHandling.ACCEPT);
        
        try (
            MrzParser parser =  new MrzParser(config)
        ) {
            MrzScannerResult result = parser.parse(str);
            
            System.out.println("  Success: " + result.getSuccess());
            System.out.println("  Raw MRZ: " + result.getRawMRZ());
            Utils.printGenericDocument(result.getDocument());
        }
    }
}
