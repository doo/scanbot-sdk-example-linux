from scanbotsdk import *
from utils import print_generic_document

def scan_credit_card(image: ImageRef):
    config = CreditCardScannerConfiguration(
        require_cardholder_name=False,
        require_expiry_date = True
    )
    # Configure other parameters as needed.
    
    scanner = CreditCardScanner(configuration=config)
    result: CreditCardScanningResult = scanner.run(image=image)

    print(f"Detection Status: {result.detection_status.name}")
    print(f"Scanning Status: {result.scanning_status.name}")
    print_generic_document(result.credit_card)
