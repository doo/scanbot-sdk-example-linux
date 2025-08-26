import * as ScanbotSDK from "scanbotsdk";

export class MedicalCertificateScannerSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    // Configure scanning parameters
    const params = new ScanbotSDK.MedicalCertificateScanningParameters();
    params.recognizePatientInfoBox = true;
    params.shouldCropDocument = true;
    // configure other parameters as needed

    // Create scanner and run
    // `await using` ensures both scanner and result are properly disposed
    // when the scope ends, as they hold unmanaged resources.
    await using scanner = await ScanbotSDK.MedicalCertificateScanner.create();
    await using result = await scanner.run(image, params);

    this.printResult(result);
  }

  private static printResult(
    result: ScanbotSDK.MedicalCertificateScanningResult
  ): void {
    console.log("Scanning successful: " + result.scanningSuccessful);
    console.log("Form type: " + result.formType);
    console.log("Clockwise rotations: " + result.clockwiseRotations);

    const patientInfoBox = result.patientInfoBox;
    if (patientInfoBox) {
      console.log("Patient info box:");
      console.log("  Has contents: " + patientInfoBox.hasContents);

      if (patientInfoBox.fields && patientInfoBox.fields.length > 0) {
        console.log("  Fields:");
        for (const field of patientInfoBox.fields) {
          console.log(`    ${field.type}: ${field.value}`);
        }
      }
    }

    if (result.checkBoxes && result.checkBoxes.length > 0) {
      console.log("Check boxes:");
      for (const cb of result.checkBoxes) {
        console.log(`  ${cb.type}: ${cb.checked ? "Checked" : "Unchecked"}`);
      }
    }

    if (result.dates && result.dates.length > 0) {
      console.log("Dates:");
      for (const dateRecord of result.dates) {
        console.log(`  ${dateRecord.type}: ${dateRecord.rawString}`);
      }
    }
  }
}
