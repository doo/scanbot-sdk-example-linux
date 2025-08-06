import scanbotsdk

def scan_mrz(image: scanbotsdk.ImageRef):
    configuration = scanbotsdk.MrzScannerConfiguration()
    scanner = scanbotsdk.MrzScanner(configuration=configuration)

    result = scanner.run(image=image)

    print(f"MRZ Scanner Result:")
    print(f"  Success:  {result.success}")
    print(f"  Raw MRZ:   {result.raw_mrz}")
