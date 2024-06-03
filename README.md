# Timelock_Shield
Timelock Shield is a protocol to mitigate front running and bribery attempts in Blockchain. We have implemented the Wesolowski's VDF based on Rivest time lock puzzle. Encryption, decryption and proof generation function are implemented in cpp. Whereas the verification part is implemented as smart contract in solidity with the help of BigIntegers Library. 

To run the code
1) You need to install gmp library 
        sudo apt-get install libgmp3-dev
2) make
3) run the command to get information about the parameters to be provided
        ./vdf -h
