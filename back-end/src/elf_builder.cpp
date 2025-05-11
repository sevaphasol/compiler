//——————————————————————————————————————————————————————————————————————————————

#include "elf_builder.h"
#include "custom_assert.h"
#include "elf_builder_defines.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

//——————————————————————————————————————————————————————————————————————————————

/* Main goal of this function is to build:
   Start of ELF
   ├── [ELF Header]
   ├── [Program Header Table]
   │             └── [Executable segment program header]
   ├── [Padding до 0x1000]
   ├── [Executable Code]
   ├── [.shstrtab]
   |             └── "\0.text\0.shstrtab"
   └── [Section Header Table]
                 ├── [.text section]
                 └── [.shstrtab section]
   */

#define N_PHDRS 1
#define N_SHDRS 2

lang_status_t build_elf(buffer_t* elf, const uint8_t* code, size_t code_size)
{
    ASSERT(elf);
    ASSERT(code);
    //--------------------------------------------------------------------------
    Elf64_Ehdr elf_hdr = {}; // Will be setted later
    buf_write(elf, &elf_hdr, sizeof(elf_hdr));
    //--------------------------------------------------------------------------
    Elf64_Phdr exe_phdr = {};
    build_exe_phdr(&exe_phdr, code_size);
    buf_write(elf, &exe_phdr, sizeof(exe_phdr));
    //--------------------------------------------------------------------------
    size_t padding_size = PAGE_SIZE - sizeof(elf_hdr) - sizeof(exe_phdr);
    uint8_t* padding = (uint8_t*) calloc(padding_size, sizeof(uint8_t));
    buf_write(elf, padding, padding_size);
    //--------------------------------------------------------------------------
    buf_write(elf, code, code_size);
    //--------------------------------------------------------------------------
    buffer_t shstrtab = {};
    shstrtab_init(&shstrtab, strlen("0.text0.shstrtab0"));

    size_t text_name_offset     = 0;
    size_t shstrtab_name_offset = 0;

    add_section_name(&shstrtab, ".text",     &text_name_offset);
    add_section_name(&shstrtab, ".shstrtab", &shstrtab_name_offset);

    size_t shstrtab_offset = elf->size;
    buf_write(elf, shstrtab.data, shstrtab.size);
    //--------------------------------------------------------------------------
    size_t shoff = elf->size;
    //--------------------------------------------------------------------------
    Elf64_Shdr text_shdr = {};
    build_text_shdr(&text_shdr, code_size, text_name_offset);
    buf_write(elf, &text_shdr, sizeof(text_shdr));
    //--------------------------------------------------------------------------
    Elf64_Shdr shstrtab_shdr = {};
    build_shstrtab_shdr(&shstrtab_shdr, shstrtab.size, shstrtab_name_offset, shstrtab_offset);
    buf_write(elf, &shstrtab_shdr, sizeof(shstrtab_shdr));
    //--------------------------------------------------------------------------
    buf_dtor(&shstrtab);
    //--------------------------------------------------------------------------
    build_elf_hdr(&elf_hdr, N_PHDRS, N_SHDRS, shoff, 1);
    buf_rewrite(elf, &elf_hdr, sizeof(elf_hdr), 0);
    //--------------------------------------------------------------------------
    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t create_elf_file(const char* file_name, const uint8_t* code, size_t code_size)
{
    ASSERT(file_name);
    ASSERT(code);

    buffer_t elf = {};
    buf_ctor(&elf, 10000);

    build_elf(&elf, code, code_size);

    FILE* fp = fopen(file_name, "wb");
    if (!fp) {
        fprintf(stderr, "fopen error\n");
        return LANG_ERROR;
    }

    fwrite(elf.data, sizeof(uint8_t), elf.size, fp);

    fclose(fp);
    buf_dtor(&elf);

    if (chmod(file_name, RWX_RX_RX) == -1) {
        fprintf(stderr, "chmod error\n");
        return LANG_ERROR;
    }

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

int main()
{
    const uint8_t code[] = {
        0x48, 0xC7, 0xC0, 0x3C, 0x00, 0x00, 0x00, // mov rax, 60
        0x48, 0x31, 0xFF,                         // xor rdi, rdi
        0x0F, 0x05,                               // syscall
    };

    const size_t code_size = sizeof(code);

    create_elf_file("out", code, code_size);

    return 0;
}

//——————————————————————————————————————————————————————————————————————————————
