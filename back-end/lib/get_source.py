from elftools.elf.elffile import ELFFile
import sys
import struct

def parse_elf(input_path):
    with open(input_path, 'rb') as f:
        elffile = ELFFile(f)
        text_section = elffile.get_section_by_name('.text')

        if not text_section:
            print("Unable to locate section .text")
            return None

        return (text_section['sh_size'], text_section.data())

def main():
    if len(sys.argv) != 3:
        print("Invalid argv")
        return 1

    input  = sys.argv[1]
    output = sys.argv[2]

    size, code = parse_elf(input)

    if code is None:
        return 1

    with open(sys.argv[2], 'wb') as f:
        f.write(code)

    with open(output, 'wb') as f:
        f.write(struct.pack('<Q', size))
        f.write(code)
        print(f"Wrote .text — {size} bytes and 8-byte header to {output}")

    return 0

if __name__ == "__main__":
    main()
