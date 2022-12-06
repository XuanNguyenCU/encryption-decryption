Author: Xuan Nguyen

Description of the program:
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Program will prompt the user to indicate whether they wish to encrypt a readable message (the plaintext), or decrypt an encrypted sequence of numbers (the ciphertext) back to its readable form. 
If the user chooses to decrypt an encrypted sequence, the program reads a space-separated sequence of decimal, numeric values from the command line,
until reaching a sentinel value of -1 (breaking/ending the sequence and then printing out the decrypted message).

Source files:  encrypt_decrypt.c




Instructions for compilation and launching, including command line arguments in bash (Linux) and using GCC compiler
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

Go to the same directory where encrypt_decrypt.c is under, using the cd command.
> cd NameofDirectory
 
You might have to go through several directories depending where you put the encrypt_decrypt.c file.

Linking: turn the encrypt_decrypt.c file into executable code => secret.exe
> gcc -o secret encrypt_decrypt.c

Run/launch the executable code.
> ./secret

The program will then prompt the user to select a choice. What will be show on the terminal:

  You may:
    (1) Encrypt a message
    (2) Decrypt a message
    (0) Exit
    what is your selection:

