#ifndef _ELF_SECTION_HEADER_BUILDERS_H__
#define _ELF_SECTION_HEADER_BUILDERS_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang_status.h"
#include "buffer.h"

#include <elf.h>
#include <inttypes.h>

//——————————————————————————————————————————————————————————————————————————————

lang_status_t shstrtab_init    (buffer_t* tab, size_t init_capacity);
lang_status_t add_section_name (buffer_t* tab, const char* name, uint32_t* offset);

void build_text_shdr(Elf64_Shdr* shdr, size_t code_size, uint32_t name_offset);
void build_shstrtab_shdr(Elf64_Shdr* shdr, size_t shstrtab_size, uint32_t name_offset, size_t table_offset);

//——————————————————————————————————————————————————————————————————————————————

#endif // _ELF_SECTION_HEADER_BUILDERS_H__
