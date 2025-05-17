#ifndef _BACKEND_UTILS_H__
#define _BACKEND_UTILS_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang.h"

//——————————————————————————————————————————————————————————————————————————————

lang_status_t backend_lang_ctx_ctor   (lang_ctx_t* ctx,
                                       int         argc,
                                       char*       argv[]);

lang_status_t backend_lang_ctx_dtor   (lang_ctx_t* ctx);

//——————————————————————————————————————————————————————————————————————————————

#endif // _BACKEND_UTILS_H__
