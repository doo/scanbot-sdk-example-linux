from scanbotsdk import *

def analyze_multi_page(file_path: str):
    configuration = DocumentQualityAnalyzerConfiguration(
        tile_size=300,
        detect_orientation=True,
        min_estimated_number_of_symbols_for_document=20,
    )
    # Configure other parameters as needed.

    file_access_source = RandomAccessSource(file_path=file_path)
    extractor = MultiPageImageExtractor()
    extraction_result: PageExtractionResult = extractor.run(source=file_access_source)

    analyser = DocumentQualityAnalyzer(configuration=configuration)

    for page_index, page in enumerate(extraction_result.pages):
        for image_index, extracted_image in enumerate(page.images):
            # Important: wrap each extracted image in a context manager.
            # This ensures that memory is released immediately after processing
            # instead of keeping all uncompressed images alive until the
            # entire extraction_result is garbage-collected.
            with extracted_image.image:
                analysis_result = analyser.run(image=extracted_image.image)
                print(
                    f"Page {page_index + 1}, Image {image_index + 1} "
                    f"-> Found: {analysis_result.document_found}, Quality: {analysis_result.quality.name}"
                )
