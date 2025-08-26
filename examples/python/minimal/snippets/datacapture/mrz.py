
from scanbotsdk import *

from utils import print_generic_document

def scan_mrz(image: ImageRef):
    config = MrzScannerConfiguration(
        enable_detection=True,
        incomplete_result_handling=MrzIncompleteResultHandling.ACCEPT,
        frame_accumulation_configuration = AccumulatedResultsVerifierConfiguration(
            minimum_number_of_required_frames_with_equal_scanning_result=1    
        )
    )
    # Configure other parameters as needed.
    
    scanner = MrzScanner(configuration=config)
    result: MrzScannerResult = scanner.run(image=image)

    print(f" Success:  {result.success}")
    print(f" Raw MRZ:  {result.raw_mrz}")
    print_generic_document(result.document)
