#ifndef FLOATING_LICENSE_H
#define FLOATING_LICENSE_H

#include <ScanbotSDK.h>

void init_with_floating_license();
char* license_status_str(scanbotsdk_license_status_t s);

#endif
