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

from snippets.live.camera import open_camera
from snippets.live.barcode_scanner import *
from snippets.live.document_scanner import *
from snippets.live.runner import run_scanner

SCANBOTSDK_LICENSE_CHECK_TIMEOUT_MS = 15000
DEREGISTER_DEVICE_TIMEOUT_MS = 15000

def main():
    if len(sys.argv) < 3:
        print_usage()
        return
    
    flags = parse_flags(sys.argv[3:])
    
    # TODO Add your Scanbot SDK trial license key here.
    license_key = flags.get("--license", "<SCANBOTSDK-LICENSE>")

    # Setup and initialize the Scanbot SDK
    print(f"Initializing Scanbot SDK...")
    
    scanbotsdk.initialize(license_key)
    scanbotsdk.wait_for_online_license_check_completion(SCANBOTSDK_LICENSE_CHECK_TIMEOUT_MS)
    
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
    with scanbotsdk.DeviceSession(deregister_timeout_ms=DEREGISTER_DEVICE_TIMEOUT_MS):
        category   = sys.argv[1].lower()
        subcommand = sys.argv[2].lower()

        file_path     = flags.get("--file")
        save_path     = flags.get("--save")
        text_input    = flags.get("--text")
        device_input  = flags.get("--device") # live only
        show_preview  = bool(flags.get("--preview")) # live only
        
        if category == "scan":
            if not file_path: print_usage(); return
            with create_image_ref(file_path) as image:
                if   subcommand == "barcode":             scan_barcode(image)
                elif subcommand == "document":            scan_document(image)
                elif subcommand == "check":               scan_check(image)
                elif subcommand == "credit_card":         scan_credit_card(image)
                elif subcommand == "document_data_extractor": extract_document_data(image)
                elif subcommand == "medical_certificate": scan_medical_certificate(image)
                elif subcommand == "mrz":                 scan_mrz(image)
                elif subcommand == "ocr":                 run_ocr(image)
                elif subcommand == "text_pattern":        scan_text_pattern(image)
                elif subcommand == "vin":                 scan_vin(image)
                else: print_usage()

        if category == "classify":
            if not file_path: print_usage(); return
            with create_image_ref(file_path) as image:
                if   subcommand == "document":            classify_document(image)
                else: print_usage()
                
        elif category == "analyse":
            if not file_path: print_usage(); return
            if   subcommand == "analyse_multi_page":      analyse_multi_page(file_path)
            elif subcommand == "crop_analyze":            crop_and_analyse(file_path, save_path)
            else: print_usage()
            
        elif category == "parse":
            if not text_input or not text_input.strip(): print_usage(); return
            if   subcommand == "mrz":           parse_mrz(text_input)
            elif subcommand == "barcode_doc":   parse_barcode_document(text_input)
            else: print_usage()
            
        elif category == "live":
            if not device_input: print_usage(); return
            if subcommand == "barcode":             barcode_live(device_input, show_preview)
            if subcommand == "document":            document_live(device_input, show_preview)
            else: print_usage()

if __name__ == "__main__":
    main()
    
def barcode_live(device_input: str, show_preview: bool):
    cap = open_camera(device_input)
    scanner = create_barcode_scanner(use_tensorrt=False)
    if show_preview:
        run_scanner(cap, scanner.run, on_result=draw_barcodes_frame)
    else:
        run_scanner(cap, scanner.run, on_result=print_barcodes_text)


def document_live(device_input: str, show_preview: bool):
    cap = open_camera(device_input)
    scanner = create_document_scanner()
    if show_preview:
        run_scanner(cap, scanner.run, on_result=draw_documents_frame)
    else:
        run_scanner(cap, scanner.run, on_result=print_documents_text)

