victim2:aaa
victim1:password123
victim:password123

Password list: https://raw.githubusercontent.com/danielmiessler/SecLists/refs/heads/master/Passwords/Common-Credentials/10-million-password-list-top-1000000.txt

To compile this program on Linux, you'll need the crypt library: 
```g++ -o hash_verifier password_hash_verifier.cpp -lcrypt```
