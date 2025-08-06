import sys
import scanbotsdk

from utils.image_utils import create_image_ref

from snippets.barcode.detect_barcodes import scan_barcode
from snippets.document.detect_document import scan_document
from snippets.datacapture.ocr import run_ocr
from snippets.datacapture.mrz import scan_mrz

LICENSE_KEY:str = "Put-your-license-key-here"

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
        command = sys.argv[1].lower()
        
        # Load the input image path
        image_path = sys.argv[2]

        image = create_image_ref(image_path)

        if command == "detectbarcode":
            scan_barcode(image)
        elif command == "detectdocument":
            scan_document(image)
        elif command == "ocr":
            run_ocr(image_path)
        elif command == "mrz":
            scan_mrz(image)
        else:
            print(f"Unknown command: {command}")
            print_usage()


if __name__ == "__main__":
    main()


def print_usage():
    print("Usage:")
    print("  python main.py <command> <path-to-image>")
    print("Commands:")
    print("  detectbarcode")
    print("  detectdocument")