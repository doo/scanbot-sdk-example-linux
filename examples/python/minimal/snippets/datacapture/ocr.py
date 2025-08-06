import scanbotsdk
import cv2 as cv
from timeit import default_timer as timer

def run_ocr(image_path: str):
    npimage = cv.imread(image_path)
    image = scanbotsdk.ImageRef.from_ndarray(npimage, scanbotsdk.RawImageLoadOptions(live_source=True))

    engine = scanbotsdk.OcrEngine()
    start_time = timer()
    page = engine.run(image=image)
    end_time = timer()
    print(f"OCR processing time: {1000 * (end_time - start_time):.2f} ms")

    for block in page.blocks:
        print(f'Block: "{block.text}"')
        for line in block.lines:
            print(f'  Line: "{line.text}"')
            for word in line.words:
                print(f'    Word: "{word.text}" (Confidence: {word.confidence})')
