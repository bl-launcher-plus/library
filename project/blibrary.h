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

int blibrary_initialize(blmodule * info);



#ifdef __cplusplus
}
#endif //__cplusplus

#endif // BLOADER_H
