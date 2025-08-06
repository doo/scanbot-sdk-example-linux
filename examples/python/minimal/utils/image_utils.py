import scanbotsdk
from pathlib import Path


def create_image_ref(path):
    if not isinstance(path, (str, Path)):
        raise TypeError(f"path must be str or Path, got: {type(path)}")

    p = Path(path)
    if not p.exists():
        raise FileNotFoundError(f"Image file not found: {p.resolve()}")

    return scanbotsdk.ImageRef.from_path(str(p.resolve()))
