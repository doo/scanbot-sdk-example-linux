from scanbotsdk import *

from utils import print_generic_document

def parse_mrz(text: str):
    mrz_parser = MrzParser(
        configuration=MrzParserConfiguration(
            incomplete_result_handling=MrzIncompleteResultHandling.ACCEPT
        )
    )
    
    result: MrzScannerResult = mrz_parser.parse(text=text)
    
    print(f" Success:  {result.success}")
    print(f" Raw MRZ:  {result.raw_mrz}")
    print_generic_document(result.document)

