from scanbotsdk import *

from utils import print_generic_document

def parse_barcode_document(raw_string: str):
    document_parser = BarcodeDocumentParser(
        accepted_formats=BarcodeDocumentFormats.all
    )
    
    result: BarcodeDocumentParserResult = document_parser.parse(raw_string=raw_string)
    print_generic_document(result.parsed_document)

