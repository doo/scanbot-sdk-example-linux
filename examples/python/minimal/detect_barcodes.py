import sys
import scanbotsdk

LICENSE_KEY:str = "Put-your-license-key-here"

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
    # Load the input image
    input_path = sys.argv[1]

    image = scanbotsdk.ImageRef.from_path(input_path)
    # Setup scanning configuration
    configuration = scanbotsdk.BarcodeScannerConfiguration()

    barcode_scanner = scanbotsdk.BarcodeScanner(configuration=configuration)
    barcode_scanning_result = barcode_scanner.run(image=image)
    barcodes = barcode_scanning_result.barcodes
    if len(barcodes) > 0:
        print(f"Found {len(barcodes)} barcodes")
        for barcode in barcodes:
            print(f"\t{barcode.text}")
    else:
        print(f"No barcodes found")
