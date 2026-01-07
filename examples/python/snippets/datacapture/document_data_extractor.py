from scanbotsdk import *
from utils import print_generic_document

def extract_document_data(image: ImageRef):
    config = DocumentDataExtractorConfiguration()
    config.result_accumulation_config.confirmation_method = ConfirmationMethod.EXACT
    # Configure other parameters as needed.
    
    extractor = DocumentDataExtractor(configuration=config)
    result: DocumentDataExtractionResult = extractor.run(image=image)

    print(f"Extraction Status: {result.status.name}")
    print_generic_document(result.document)
