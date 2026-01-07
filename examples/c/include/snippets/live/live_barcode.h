#ifndef LIVE_BARCODE_H
#define LIVE_BARCODE_H

#include <ScanbotSDK.h>
#include <stdbool.h>

scanbotsdk_error_code_t run_live_barcode_scanner(const char *mock_frame_path, bool use_tensor_rt);

#endif