//——————————————————————————————————————————————————————————————————————————————

#ifndef _ELF_HEADER_BUILDER_H__
#define _ELF_HEADER_BUILDER_H__

//——————————————————————————————————————————————————————————————————————————————

#include <elf.h>
#include <inttypes.h>

//——————————————————————————————————————————————————————————————————————————————

void build_ident   (unsigned char* e_ident);
void build_elf_hdr(Elf64_Ehdr* hdr, size_t n_phdrs, size_t n_shdrs, Elf64_Off shoff, Elf64_Half shstrndx);

//——————————————————————————————————————————————————————————————————————————————

#endif // _ELF_HEADER_BUILDER_H__

//——————————————————————————————————————————————————————————————————————————————
