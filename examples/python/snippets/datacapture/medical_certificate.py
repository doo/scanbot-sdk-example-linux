from scanbotsdk import *

def print_medical_certificate_result(result: MedicalCertificateScanningResult):
    print(f"Scanning successful: {result.scanning_successful}")
    print(f"Form type: {result.form_type.name}")
    print(f"Clockwise rotations: {result.clockwise_rotations}")

    if result.patient_info_box:
        print("Patient info box:")
        print(f"  Has contents: {result.patient_info_box.has_contents}")
        if result.patient_info_box.fields:
            print("  Fields:")
            for field in result.patient_info_box.fields:
                print(f"    {field.type.name}: {field.value}")

    if result.check_boxes:
        print("Check boxes:")
        for cb in result.check_boxes:
            print(f"  {cb.type.name}: {'Checked' if cb.checked else 'Unchecked'}")

    if result.dates:
        print("Dates:")
        for date_record in result.dates:
            print(f"  {date_record.type.name}: {date_record.raw_string}")


def scan_medical_certificate(image: ImageRef):
    params = MedicalCertificateScanningParameters(
        recognize_patient_info_box = True,
        should_crop_document = True
    )
    # Configure other parameters as needed.
    
    scanner = MedicalCertificateScanner()
    result: MedicalCertificateScanningResult = scanner.run(image=image, parameters=params)

    print_medical_certificate_result(result)
