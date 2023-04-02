def main():
    with(open("A", "bw") as file):
        for i in range(4 * 1024 * 1024 + 1):
            if i == 0 or i == 10000:
                file.write(b'\x01')
            else:
                file.write(b'\x00')


if __name__ == "__main__":
    main()
