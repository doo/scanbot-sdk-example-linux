import os
from scanbotsdk import *
from utils import create_image_ref

def crop_and_analyze(image_path: str, save_path: str | None = None):
    with create_image_ref(image_path) as image:
        # Configure scanner
        config = DocumentScannerConfiguration()
        config.parameters.accepted_angle_score = 75
        config.parameters.ignore_orientation_mismatch = False
        # Configure other parameters as needed.

        scanner = DocumentScanner(configuration=config)
        result = scanner.scan(image=image)

        print(f"Detection status: {result.detection_result.status}")
        print(f"Total detection score: {result.detection_result.detection_scores.total_score}")

        cropped = result.cropped_image
        if cropped:
            # Optionally save the cropped image
            if save_path:
                cropped.save_image(save_path, SaveImageOptions())
                print(f"Saved cropped image to: {os.path.abspath(save_path)}")

            # Configure analyzer
            analyser_config = DocumentQualityAnalyzerConfiguration(
                tile_size=300,
                min_estimated_number_of_symbols_for_document=20
            )
            analyser = DocumentQualityAnalyzer(configuration=analyser_config)

            quality_result: DocumentQualityAnalyzerResult = analyser.run(image=cropped)
            print(f"Document Found: {quality_result.document_found}")
            print(f"Quality: {quality_result.quality}")
