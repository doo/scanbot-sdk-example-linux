from scanbotsdk import *
from pathlib import Path
from collections import OrderedDict
import sys

def create_image_ref(path: str) -> ImageRef:
    p = Path(path).expanduser().resolve()
    if not p.is_file():
        raise FileNotFoundError(f"Image file not found: {p}")
    return ImageRef.from_path(str(p))

def print_generic_document(doc: GenericDocument):
    if not doc:
        print("Document: None")
        return

    print(f"Type: {doc.type.name} {doc.type.full_name}")
    
    if doc.fields:
        print("Fields:")
        for field in doc.fields:
            value_text = field.value.text if field.value else None
            print(f"{field.type.name}: {value_text}")

    if doc.children:
        print("Children:")
        for child in doc.children:
            print_generic_document(child)
            

def parse_flags(tokens) -> OrderedDict:
    flags = OrderedDict()
    it = iter(tokens)
    for token in it:
        if not token.startswith("--"):
            sys.stderr.write(f"Unexpected token: {token}\n")
            break
        # get next value if present and not another flag
        next_val = next(it, None)
        if next_val is None or next_val.startswith("--"):
            flags[token] = "true"
            if next_val and next_val.startswith("--"):
                # rewind iterator by one if it's another flag
                it = (v for v in [next_val] + list(it))
        else:
            flags[token] = next_val
    return flags


def print_usage():
    print("Scanbot SDK Example\n")
    print("Usage:")
    print("  python main.py scan <command> --file <path/to/file.jpg>")
    print("or")
    print("  python main.py scan <command> --file <path/to/file.jpg>\n")
    print("  python main.py classify <command> --file <path/to/file.jpg>\n")
    print("  python main.py analyse <command> --file <path/to/file> --save <path/to/save>")
    print("or")
    print("  python main.py parse <command> --text \"<input>\"\n")
    print("Available scan commands:")
    print("  barcode | document | check | credit_card | document_extractor | medical_certificate | mrz | ocr | text_pattern | vin\n")
    print("Available analyse commands:")
    print("  analyse_multi_page | crop_analyze\n")
    print("Available classify commands:")
    print("  document \n")
    print("Available parse commands:")
    print("  mrz | barcode_doc\n")
    print("Note:")
    print("  The --save argument is optional and only used with analyse/crop_analyze.\n")
    print("Examples:")
    print("  python main.py scan barcode --file images/example.jpg")
    print("  python main.py analyse analyse_multi_page --file files/doc.pdf")
    print("  python main.py analyse crop_analyze --file images/doc.jpg --save out/crop.jpg")
    print("  python main.py parse mrz --text \"P<UTOERIKSSON<<ANNA<MARIA<<<<<<\"")
    print()