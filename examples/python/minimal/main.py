import sys
import scanbotsdk

from snippets.document.document_classifier import classify_document
from snippets.document import analyse_multi_page
from snippets.document import crop_and_analyse
from snippets.barcode.barcode_document_parser import parse_barcode_document
from snippets.datacapture.mrz_parser import parse_mrz
from snippets.datacapture.vin import scan_vin
from snippets.datacapture.text_pattern import scan_text_pattern
from snippets.datacapture.ocr import run_ocr
from snippets.datacapture.mrz import scan_mrz
from snippets.datacapture.medical_certificate import scan_medical_certificate
from snippets.datacapture.document_data_extractor import extract_document_data
from snippets.datacapture.credit_card import scan_credit_card
from snippets.barcode.detect_barcodes import scan_barcode
from snippets.document.detect_document import scan_document
from snippets.datacapture.check import scan_check

from utils import create_image_ref, parse_flags, print_usage

# TODO Add your Scanbot SDK trial license key here.
# The SDK and a trial license are available on request via beta@scanbot.io
LICENSE_KEY:str = "<SCANBOTSDK-LICENSE>"

def main():
    if len(sys.argv) < 3:
        print_usage()
        return

    # Setup and initialize the Scanbot SDK
    print(f"Initializing Scanbot SDK...")
    
    scanbotsdk.initialize(LICENSE_KEY)
    scanbotsdk.wait_for_online_license_check_completion(5000)
    
    license_info = scanbotsdk.get_license_info()
    print(f"License Status: {license_info.status}")
    
    is_floating_license = license_info.devices is not None
    if is_floating_license:
        print(
            f"Using floating license with {license_info.devices} devices. Do not forget to call "
            f"scanbotsdk.deregister_device and scanbotsdk.wait_for_device_deregistration_completion when you no "
            f"longer need the license or use scanbotsdk.DeviceSession context manager.")

    # If you are not using floating license, it is not required to use scanbotsdk.DeviceSession context manager as there
    # is no need to notify server you are no longer using the license. Alternatively, you can manually call
    # scanbotsdk.deregister_device and scanbotsdk.wait_for_device_deregistration_completion if you need asynchronous
    # deregistration behaviour
    with scanbotsdk.DeviceSession(deregister_timeout_ms=15000):
        category   = sys.argv[1].lower()
        subcommand = sys.argv[2].lower()
        flags      = parse_flags(sys.argv[3:])

        file_path     = flags.get("--file")
        save_path     = flags.get("--save")
        text_input    = flags.get("--text")
        
        if category == "scan":
            if not file_path:
                print_usage(); return
            with create_image_ref(file_path) as image:
                if   subcommand == "barcode":             scan_barcode(image)
                elif subcommand == "document":            scan_document(image)
                elif subcommand == "check":               scan_check(image)
                elif subcommand == "credit_card":         scan_credit_card(image)
                elif subcommand == "document_data_extractor": extract_document_data(image)
                elif subcommand == "medical_certificate": scan_medical_certificate(image)
                elif subcommand == "mrz":                 scan_mrz(image)
                elif subcommand == "ocr":                 run_ocr(file_path)
                elif subcommand == "text_pattern":        scan_text_pattern(image)
                elif subcommand == "vin":                 scan_vin(image)
                else: print_usage()

        if category == "classify":
            if not file_path:
                print_usage(); return
            with create_image_ref(file_path) as image:
                if   subcommand == "document":            classify_document(image)
                else: print_usage()
                
        elif category == "analyse":
            if not file_path: print_usage(); return
            if   subcommand == "analyse_multi_page":  analyse_multi_page(file_path)
            elif subcommand == "crop_analyze":        crop_and_analyse(file_path, save_path)
            else: print_usage()

        elif category == "parse":
            if not text_input or not text_input.strip(): print_usage(); return
            if   subcommand == "mrz":           parse_mrz(text_input)
            elif subcommand == "barcode_doc":   parse_barcode_document(text_input)
            else: print_usage()

if __name__ == "__main__":
    main()