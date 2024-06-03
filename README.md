# Timelock_Shield
Timelock Shield is a protocol to mitigate front-running and bribery attempts in Blockchain. We have implemented Wesolowski's VDF based on the Rivest time lock puzzle. Encryption, decryption, and proof generation functions are implemented in cpp. Whereas the verification part is implemented as a smart contract in solidity with the help of BigIntegers Library. The Python files are the utility files to convert the sentence to numbers, as currently, our implementation only supports numbers.

To run the code
1) You need to install GMP library 
        sudo apt-get install libgmp3-dev
2) make
3) run the command to get information about the parameters to be provided
        ./vdf -h
