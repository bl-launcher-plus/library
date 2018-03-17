#ifndef BLIBRARY_H
#define BLIBRARY_H

/*
 * BLibrary
 * Used to communicate to each library
 * Implement for each library
 */

#include "bloader.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Information about module
typedef struct BLinfo
{
	char name[256];
	int version;
	char description[4096];
} blinfo;

int blibrary_initialize();

void blibrary_info(blinfo * info);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // BLOADER_H
