//——————————————————————————————————————————————————————————————————————————————

#ifndef _ELF_BUILDER_H__
#define _ELF_BUILDER_H__

//——————————————————————————————————————————————————————————————————————————————

#include "buffer.h"
#include "elf_header_builder.h"
#include "elf_program_header_builders.h"
#include "elf_section_header_builders.h"

#include <elf.h>
#include <inttypes.h>

//——————————————————————————————————————————————————————————————————————————————

struct Elf64_Phdr_table_t {
    Elf64_Phdr* table;
    size_t      capacity;
    size_t      size;
};

//——————————————————————————————————————————————————————————————————————————————

struct Elf64_Shdr_table_t {
    Elf64_Shdr* table;
    size_t capacity;
    size_t size;
};

//——————————————————————————————————————————————————————————————————————————————

struct elf_builder_t {
    buffer_t           buf;
    Elf64_Ehdr         header;
    Elf64_Phdr_table_t phdr_table;
    Elf64_Shdr_table_t shdr_table;
};

//——————————————————————————————————————————————————————————————————————————————

lang_status_t build_elf(buffer_t* elf, const uint8_t* code, size_t code_size);
lang_status_t create_elf_file (const char* file_name, const uint8_t* code, size_t code_size);

//——————————————————————————————————————————————————————————————————————————————

#endif // _ELF_BUILDER_H__

//——————————————————————————————————————————————————————————————————————————————
