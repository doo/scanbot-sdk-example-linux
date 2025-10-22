from scanbotsdk import *

def analyze_multi_page(file_path: str):
    configuration = DocumentQualityAnalyzerConfiguration(
        tile_size=300,
        detect_orientation=True,
        min_estimated_number_of_symbols_for_document=20,
    )
    # Configure other parameters as needed.

    extractor = MultiPageImageExtractor()
    analyser = DocumentQualityAnalyzer(configuration=configuration)

    with RandomAccessSource(file_path=file_path) as file_access_source:
        extraction_result: PageExtractionResult = extractor.run(source=file_access_source)

        for page_index, page in enumerate(extraction_result.pages):
            for image_index, extracted_image in enumerate(page.images):
                # Important: wrap each extracted image in a context manager.
                # This ensures memory is released immediately after processing,
                # instead of waiting for the whole extraction_result to be freed.
                with extracted_image.image:
                    analysis_result = analyser.run(image=extracted_image.image)
                    print(
                        f"Page {page_index + 1}, Image {image_index + 1} "
                        f"-> Found: {analysis_result.document_found}, "
                        f"Quality: {analysis_result.quality}"
                    )
