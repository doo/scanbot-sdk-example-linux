from scanbotsdk import *
    
def analyse_multi_page(file_path: str):
    configuration = DocumentQualityAnalyzerConfiguration(
        tile_size = 300,
        detect_orientation = True,
        min_estimated_number_of_symbols_for_document = 20
    )
    # Configure other parameters as needed.
    
    file_access_source = RandomAccessSource(file_path=file_path)
    extractor = MultiPageImageExtractor()
    result: PageExtractionResult = extractor.run(source=file_access_source)
    
    analyser = DocumentQualityAnalyzer(configuration=configuration)
    
    for page_index, page in enumerate(result.pages):
        for image_index, extracted_image in enumerate(page.images):
            result = analyser.run(image=extracted_image.image)
            print(
                f"Page {page_index + 1}, Image {image_index + 1} "
                f"-> Found: {result.document_found}, Quality: {result.quality.name}"
            )

