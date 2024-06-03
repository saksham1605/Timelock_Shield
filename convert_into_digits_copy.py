from cryptography.fernet import Fernet
import argparse
import os

def generate_key():
    return Fernet.generate_key()

def encrypt_message(message, key):
    cipher_suite = Fernet(key)
    encrypted_message = cipher_suite.encrypt(message)
    return encrypted_message

def decrypt_message(encrypted_message, key):
    cipher_suite = Fernet(key)
    decrypted_message = cipher_suite.decrypt(encrypted_message).decode()
    return decrypted_message

def main():
    parser = argparse.ArgumentParser(description="Encrypt and decrypt messages")
    parser.add_argument("-i", "--input", help="Input file path", required=True)
    parser.add_argument("-o", "--output", help="Output file path", required=True)
    parser.add_argument("-f", "--format", help="Option: 1 for encryption, 2 for decryption", required=True)
    args = parser.parse_args()

    if args.format == "1":
        key = generate_key()
    else:
        print("Decryption requires a key. Please provide the key used for encryption.")
        return

    with open(args.input, "rb") as input_file:
        data = input_file.read()

    if args.format == "1":
        encrypted_data = encrypt_message(data, key)
        with open(args.output, "wb") as output_file:
            output_file.write(encrypted_data)
    elif args.format == "2":
        decrypted_data = decrypt_message(data, key)
        with open(args.output, "w") as output_file:
            output_file.write(decrypted_data)
    else:
        print("Invalid option!")

if __name__ == "__main__":
    main()
