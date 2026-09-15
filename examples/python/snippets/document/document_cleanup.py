import os
from typing import Optional

from scanbotsdk import *
from utils import create_image_ref


def cleanup_document(image_path: str, mask_path: str, save_path: Optional[str] = None):
    with create_image_ref(image_path) as image, create_image_ref(mask_path) as mask:
        cleanup = DocumentCleanup(
            image=image,
            configuration=DocumentCleanupConfiguration()
        )
        result = cleanup.run(mask=mask)

        print(f"Document cleanup status: {result.status.name}")

        if result.status == DocumentCleanupStatus.OK:
            original_info = image.info()
            cleaned_info = result.image.info()
            print(f"Original WxH: {original_info.width}x{original_info.height}")
            print(f"Cleaned  WxH: {cleaned_info.width}x{cleaned_info.height}")

            if save_path:
                result.image.save_image(save_path, SaveImageOptions())
                print(f"Saved cleaned image to: {os.path.abspath(save_path)}")
