from scanbotsdk import *
from utils import print_generic_document

def extract_document_data(image: ImageRef):
    # Make sure you have a valid license
    license_info = get_license_info()
    if license_info.status != LicenseStatus.OKAY:
        return
    
    config = DocumentDataExtractorConfiguration()
    config.result_accumulation_config.confirmation_method = ConfirmationMethod.EXACT
    # Configure other parameters as needed.
    
    extractor = DocumentDataExtractor(configuration=config)
    result: DocumentDataExtractionResult = extractor.run(image=image)

    # if you want to use cropped image later, call result.cropped_image.encode(), and save buffer,
    # otherwise image reference will be released when the ImageRef object is garbage collected.
    
    print(f"Extraction Status: {result.status.name}")
    print_generic_document(result.document)
