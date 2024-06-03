import argparse

def sentence_to_digits(sentence, length=3):
    digits = []
    for char in sentence:
        ascii_value = str(ord(char))
        ascii_value = ascii_value.zfill(length)  # Pad ASCII value with zeros to make it constant length
        digits.append(ascii_value)
    return "".join(digits)

def digits_to_sentence(digits, length=3):
    sentence = ""
    for i in range(0, len(digits), length):
        digit = digits[i:i+length]
        ascii_value = int(digit)
        character = chr(ascii_value)
        sentence += character
    return sentence

def main():
    parser = argparse.ArgumentParser(description="Convert sentences to digits and vice versa")
    parser.add_argument("-i", "--input", help="Input file path", required=True)
    parser.add_argument("-o", "--output", help="Output file path", required=True)
    parser.add_argument("-f", "--format", help="Option: 1 for sentence to digits, 2 for digits to sentence", required=True)
    args = parser.parse_args()

    digit_length = 3

    with open(args.input, "r") as input_file:
        content = input_file.read().strip()

    if args.format == "1":
        digits = sentence_to_digits(content, digit_length)
        with open(args.output, "w") as output_file:
            output_file.write(digits)
    elif args.format == "2":
        sentence = digits_to_sentence(content, digit_length)
        with open(args.output, "w") as output_file:
            output_file.write(sentence)
    else:
        print("Invalid option!")

if __name__ == "__main__":
    main()
