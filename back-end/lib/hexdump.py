import sys

def main():
    if len(sys.argv) != 2:
        print("Invalid argv")
        return 1

    input = sys.argv[1]

    with open(input, 'rb') as f:
        byte = f.read(1)
        while byte:
            print(f"0x{byte.hex():>02}")
            byte = f.read(1)

    return 0

if __name__ == "__main__":
    main()
