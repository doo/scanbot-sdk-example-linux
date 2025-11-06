from scanbotsdk import *

def classify_document(image: ImageRef):
    config = DocumentClassifierConfiguration(
        crop=True
    )
    
    classifier = DocumentClassifier(configuration=config)
    result: DocumentClassifierResult = classifier.run(image=image)
    
    print(f"Detection status: {result.status.name}")
    print(f"Type: {result.document_type.name}")
    
    if result.document_scanning_result:
        print("Detected points:")
        for point in result.document_scanning_result.detection_result.points:
            print(f"\tx: {point.x}, y: {point.y}")
    else:
        print("No document corners detected.")

