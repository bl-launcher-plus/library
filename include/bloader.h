#ifndef BLOADER_H
#define BLOADER_H

/*
 * BLoader framework
 * Used to communicate with and through BLoader
 */

#define BLOADER_VERSION 1
#define BLOADER_VERSION_STRING "v0.0.1"

/*
 * Macros to make it easier to specify what level the text printed out to console should be.
 * Info would be a blue, warning would be a dark grey/light grey, and error would be red.
*/

#define BLOADER_CONSOLE_INFO "\x05"
#define BLOADER_CONSOLE_ERROR "\x03"
#define BLOADER_CONSOLE_WARN "\x04" 


#ifdef __cplusplus
#define BL_EXTERN extern "C"
extern "C" {
#else
#define BL_EXTERN
#endif // __cplusplus

/*
 * Structure declarations
 */

// List of possible errors
enum blerror
{
	BLOADER_OK,
	BLOADER_NO_MODULE = -1,
	BLOADER_NO_INIT = -2,
	BLOADER_NO_DEINIT = -3,
	BLOADER_FAIL_INIT = -4,
	BLOADER_FAIL_DEINIT = -5,
	BL_OK = 0,
	BL_INVALID_POINTER,
	BL_INVALID_LIBRARY,
	BL_INVALID_VERSION,
	BL_MISSING_INITIALIZE,
	BL_LIBRARY_INIT_ERROR,
	BL_LIBRARY_NO_NAME
};

// A module informations
typedef struct
{
	char name[256];
	int version;
} blmodule;

// Callback for console functions
typedef void (*bl_callback_void)(void * object, int argc, const char * argv[]);
typedef bool (*bl_callback_bool)(void * object, int argc, const char * argv[]);
typedef int (*bl_callback_int)(void * object, int argc, const char * argv[]);
typedef float (*bl_callback_float)(void * object, int argc, const char * argv[]);
typedef const char * (*bl_callback_string)(void * object, int argc, const char * argv[]);

/*
 * General functions
 */

// Get version for this library when compiled
int bloader_version();

const char * bloader_getError(int errorcode);

int bloader_load(const char * name);
int bloader_unload(const char * name);

/*
 * Module handling
 */

int bloader_module_count();
const blmodule * bloader_module_fromIndex(int i);
const blmodule * bloader_module_fromName(const char * name);
const char* bloader_version_string();
int bloader_module_exist(const blmodule * module);

/*
 * Module communication
 */

//int bloader_com_send(const blmodule * module, void * data, int size);
//int bloader_com_receive(const blmodule * module, void ** data, int * size);
//int bloader_com_poll(const blmodule * module, void ** data, int * size);

/*
 * BL console features
 */

void bloader_consolefunction_void(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_void, const char * usage, int minArgs, int maxArgs);
void bloader_consolefunction_bool(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_bool, const char * usage, int minArgs, int maxArgs);
void bloader_consolefunction_int(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_int, const char * usage, int minArgs, int maxArgs);
void bloader_consolefunction_float(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_float, const char * usage, int minArgs, int maxArgs);
void bloader_consolefunction_string(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_string, const char * usage, int minArgs, int maxArgs);

void bloader_consolevariable_bool(blmodule * module, const char * name, int * var);
void bloader_consolevariable_int(blmodule * module, const char * name, int * var);
void bloader_consolevariable_float(blmodule * module, const char * name, float * var);
void bloader_consolevariable_string(blmodule * module, const char * name, char * var);

const char * bloader_getVariable(const char * name);
void bloader_setVariable(const char * name, const char * value);

/*
 * Interface for other libraries
 */

void * bloader_symbol(const blmodule * module, const char * func);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // BLOADER_H
