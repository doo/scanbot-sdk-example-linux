import signal
import scanbotsdk
import threading
import time
from pathlib import Path

LICENSE_KEY:str = "license key"
NUM_THREADS = 20
MAX_SCANS = -1

# measured on a c3d-highcpu-60 instance
# number of threads, max throughput (scans/s), CPU usage (average, %)
# 24, 76, 99
# 20, 103, 91
# 16, 104, 76
# 12, 94, 60
# 8, 74, 40
# 4, 36, 20


# Global flag to signal threads to stop
stop_threads = threading.Event()

def signal_handler(signum, frame):
    """Handle SIGTERM and SIGINT signals to gracefully shutdown"""
    print(f"\nReceived signal {signum}. Stopping all threads...")
    stop_threads.set()

# Register signal handlers
signal.signal(signal.SIGTERM, signal_handler)
signal.signal(signal.SIGINT, signal_handler)

# Setup and initialize the Scanbot SDK
print(f"Initializing Scanbot SDK...")
scanbotsdk.initialize(LICENSE_KEY)
scanbotsdk.wait_for_online_license_check_completion(5000)
license_info = scanbotsdk.get_license_info()
print(f"License Status: {license_info.status}")

total_scans = 0
start = time.time()
max_throughput = 0

def scan_barcode():
    image_path = Path(__file__).parent.parent.parent / 'example.png'
    image = scanbotsdk.ImageRef.from_path(image_path)
    configuration = scanbotsdk.BarcodeScannerConfiguration()
    barcode_scanner = scanbotsdk.BarcodeScanner(configuration=configuration)
    try:
        while not stop_threads.is_set():
            barcode_scanning_result = barcode_scanner.run(image=image)
            if len(barcode_scanning_result.barcodes) != 3:
                raise Exception(f"Expected 3 barcodes, but found {len(barcode_scanning_result.barcodes)}")
            global total_scans
            total_scans += 1
    except Exception as e:
        print(f"Error during barcode scanning: {e}")
        stop_threads.set()  # Signal other threads to stop
        return


def show_progress():
    while not stop_threads.is_set():
        if MAX_SCANS > 0 and total_scans >= MAX_SCANS:
            stop_threads.set()
            print(f"\nReached {MAX_SCANS} scans. Stopping all threads...")
            return
        rate = total_scans / (time.time() - start) if (time.time() - start) > 0 else 0
        global max_throughput
        max_throughput = max(max_throughput, rate)
        print(f"\rTotal scans: {total_scans}, scans/s: {rate:.0f}, ran for {time.time() - start:.0f}s", end="", flush=True)
        time.sleep(1)


threading.Thread(target=show_progress, daemon=True).start()
threads = [threading.Thread(target=scan_barcode) for _ in range(NUM_THREADS)]
for thread in threads:
    thread.start()

try:
    # Wait for all threads to complete
    for thread in threads:
        thread.join()
    print("All threads stopped gracefully.")
except KeyboardInterrupt:
    # Handle Ctrl+C as well
    print("\nKeyboard interrupt received. Stopping all threads...")
    stop_threads.set()
    for thread in threads:
        thread.join()
    print("All threads stopped gracefully.")

print("Application shutdown complete.")
print(f"Max throughput: {max_throughput:.0f} scans/s")
