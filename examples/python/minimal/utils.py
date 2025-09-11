from typing import Dict, Optional
from scanbotsdk import *
from pathlib import Path
import sys

def create_image_ref(path: str) -> ImageRef:
    p = Path(path).expanduser().resolve()
    if not p.is_file():
        raise FileNotFoundError(f"Image file not found: {p}")
    return ImageRef.from_path(str(p))

def print_generic_document(doc: Optional[GenericDocument]):
    if doc is None:
        print("Document is None")
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

def parse_flags(tokens) -> Dict:
    flags = {}
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
    print("  python main.py scan <command> --file <path/to/file.jpg> [--license <KEY>]")
    print("  python main.py classify <command> --file <path/to/file.jpg> [--license <KEY>]")
    print("  python main.py analyse <command> --file <path/to/file> --save <path/to/save> [--license <KEY>]")
    print("  python main.py parse <command> --text \"<input>\" [--license <KEY>]")
    print("  python main.py live <command> --device <device_name> [--license <KEY>]\n")

    print("Available scan commands:")
    print("  barcode | document | check | credit_card | document_data_extractor | medical_certificate | mrz | ocr | text_pattern | vin\n")
    print("Available analyse commands:")
    print("  analyse_multi_page | crop_analyze\n")
    print("Available classify commands:")
    print("  document\n")
    print("Available parse commands:")
    print("  mrz | barcode_doc\n")
    print("Available live commands:")
    print("  barcode\n")

    print("Note:")
    print("  --save is optional, only used with analyse/crop_analyze.")
    print("  --license is optional, default is \"<SCANBOTSDK-LICENSE>\".")
    print("  --device is required for live command (e.g. libcamera or jetson_csi).\n")

    print("Examples:")
    print("  python main.py scan barcode --file images/example.jpg --license <KEY>")
    print("  python main.py analyse analyse_multi_page --file files/doc.pdf --license <KEY>")
    print("  python main.py analyse crop_analyze --file images/doc.jpg --save out/crop.jpg --license <KEY>")
    print("  python main.py parse mrz --text \"P<UTOERIKSSON<<ANNA<MARIA<<<<<<\" --license <KEY>")
    print("  python main.py live barcode --device jetson_csi --license <KEY>")
    print()