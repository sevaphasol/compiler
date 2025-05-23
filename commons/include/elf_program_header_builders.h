#ifndef _ELF_PROGRAM_HEADER_BUILDERS_H__
#define _ELF_PROGRAM_HEADER_BUILDERS_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang_status.h"

#include <elf.h>
#include <inttypes.h>

//——————————————————————————————————————————————————————————————————————————————

void build_exe_phdr(Elf64_Phdr* phdr, size_t code_size);

//——————————————————————————————————————————————————————————————————————————————

#endif // _ELF_PROGRAM_HEADER_BUILDERS_H__
