from scanbotsdk import *

def enhance_document(image: ImageRef):
    params = DocumentStraighteningParameters()
    params.straightening_mode = DocumentStraighteningMode.STRAIGHTEN
    params.aspect_ratios = [
        AspectRatio(width=5.0, height=7.0),
        AspectRatio(width=1.0, height=1.0),
        AspectRatio(width=16.0, height=9.0),
        AspectRatio(width=3.0, height=4.0)
    ]

    enhancer = DocumentEnhancer()
    result: DocumentStraighteningResult = enhancer.straighten(image=image, parameters=params)

    # The straightened image can be accessed via result.straightened_image and saved or further processed as needed.
