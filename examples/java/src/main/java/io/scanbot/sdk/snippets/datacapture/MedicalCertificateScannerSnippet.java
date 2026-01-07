package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.medicalcertificate.*;

import io.scanbot.sdk.image.ImageRef;

public class MedicalCertificateScannerSnippet {
    public static void run(ImageRef image) throws Exception {
        MedicalCertificateScanningParameters params = new MedicalCertificateScanningParameters();
        params.setRecognizePatientInfoBox(true);
        params.setShouldCropDocument(true);
        // Configure other parameters as needed.

        try(
            MedicalCertificateScanner scanner = new MedicalCertificateScanner();
            MedicalCertificateScanningResult result = scanner.run(image, params);
        ) {
            printResult(result);
        }
    }

    private static void printResult(MedicalCertificateScanningResult result) {
        System.out.println("Scanning successful: " + result.getScanningSuccessful());
        System.out.println("Form type: " + result.getFormType().name());
        System.out.println("Clockwise rotations: " + result.getClockwiseRotations());

        MedicalCertificatePatientInfoBox patientInfoBox = result.getPatientInfoBox();
        if (patientInfoBox != null) {
            System.out.println("Patient info box:");
            System.out.println("  Has contents: " + patientInfoBox.getHasContents());

            System.out.println("  Fields:");
            for (MedicalCertificatePatientInfoField field : patientInfoBox.getFields()) {
                System.out.println("    " + field.getType().name() + ": " + field.getValue());
            }
        }
        
        System.out.println("Check boxes:");
        for (MedicalCertificateCheckBox cb : result.getCheckBoxes()) {
            System.out.println("  " + cb.getType().name() + ": " + (cb.getChecked() ? "Checked" : "Unchecked"));
        }

        System.out.println("Dates:");
        for (MedicalCertificateDateRecord dateRecord : result.getDates()) {
            System.out.println("  " + dateRecord.getType().name() + ": " + dateRecord.getRawString());
        }
    }
}
