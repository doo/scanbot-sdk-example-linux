import scanbotsdk

def scan_document(image: scanbotsdk.ImageRef):
    configuration = scanbotsdk.DocumentScannerConfiguration()
    scanner = scanbotsdk.DocumentScanner(configuration=configuration)

    result = scanner.run(image=image)

    print(f"Detection status: {result.status.name}")

    if result.points:
        print("Detected points:")
        for point in result.points:
            print(f"\tx: {point.x}, y: {point.y}")
    else:
        print("No document corners detected.")
