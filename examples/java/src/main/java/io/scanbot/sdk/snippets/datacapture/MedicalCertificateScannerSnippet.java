package io.scanbot.sdk.snippets.datacapture;

import io.scanbot.sdk.medicalcertificate.*;

import java.util.List;

import io.scanbot.sdk.ScanbotSDK;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.licensing.LicenseStatus;

public class MedicalCertificateScannerSnippet {
    public static void run(ImageRef image) throws Exception {
        // Make sure you have a valid license
        if(ScanbotSDK.getLicenseInfo().getStatus() != LicenseStatus.OKAY)
            return;
            
        MedicalCertificateScanningParameters params = new MedicalCertificateScanningParameters();
        params.setRecognizePatientInfoBox(true);
        params.setShouldCropDocument(true);
        // Configure other parameters as needed.

        try(
            MedicalCertificateScanner scanner = new MedicalCertificateScanner()
        ) {
            MedicalCertificateScanningResult result = scanner.run(image, params);
            printResult(result);
        }
    }

    private static void printResult(MedicalCertificateScanningResult result) {
        // If you want to use the image later, call result.getCroppedImage().encodeImage(...) and save the returned buffer.
        // Otherwise, the image reference will be released once the ImageRef object is closed or garbage-collected.

        System.out.println("Scanning successful: " + result.getScanningSuccessful());
        System.out.println("Form type: " + result.getFormType().name());
        System.out.println("Clockwise rotations: " + result.getClockwiseRotations());

        MedicalCertificatePatientInfoBox patientInfoBox = result.getPatientInfoBox();
        if (patientInfoBox != null) {
            System.out.println("Patient info box:");
            System.out.println("  Has contents: " + patientInfoBox.getHasContents());

            List<MedicalCertificatePatientInfoField> fields = patientInfoBox.getFields();
            if (fields != null && !fields.isEmpty()) {
                System.out.println("  Fields:");
                for (MedicalCertificatePatientInfoField field : fields) {
                    System.out.println("    " + field.getType().name() + ": " + field.getValue());
                }
            }
        }

        List<MedicalCertificateCheckBox> checkBoxes = result.getCheckBoxes();
        if (checkBoxes != null && !checkBoxes.isEmpty()) {
            System.out.println("Check boxes:");
            for (MedicalCertificateCheckBox cb : checkBoxes) {
                System.out.println("  " + cb.getType().name() + ": " + (cb.getChecked() ? "Checked" : "Unchecked"));
            }
        }

        List<MedicalCertificateDateRecord> dates = result.getDates();
        if (dates != null && !dates.isEmpty()) {
            System.out.println("Dates:");
            for (MedicalCertificateDateRecord dateRecord : dates) {
                System.out.println("  " + dateRecord.getType().name() + ": " + dateRecord.getRawString());
            }
        }
    }
}
