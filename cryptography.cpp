#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

#define KEY_LENGTH 256

// menu functions
void menu();
void GENERATE_KEY();
string LOAD_KEY();
void ENCRYPT(string);
void DECRYPT(string);

// file functions
string READ_FILE(string);
void WRITE_FILE(string, string);

// encryption and decryption functions
string ABASH_ENCRYPT(string);
string ABASH_DECRYPT(string);
string POLYALPHABETIC_ENCRYPT(string, string);
string POLYALPHABETIC_DECRYPT(string, string);
string RSA_ENCRYPT(string);
string RSA_DECRYPT(string);
string SHIFT_ENCRYPT(string, int);
string SHIFT_DECRYPT(string, int);

// helper functions
int gcd(int, int);
int int_pow(int, int);
char ch_case(char);
char key_case(char, char);
string remove_nonalpha(string);
bool file_exists(string);
int extract_first_5_integers(string);
string remove_integers(string);
string request_filename();

int main() 
{
    printf("-----------------------------------------\n");
    printf("|                                       |\n");
    printf("|    Multiple Encryption Algorithm      |\n");
    printf("|    Created by: NT                     |\n");
    printf("|                                       |\n");
    printf("-----------------------------------------\n");

    menu();

    return 0;
}

// MENU FUNCTIONS

void menu() 
{
    int selection;
    bool exit = false;
    string key = "EMPTY";
    string filename;

    while(exit == false) {
        printf("\n\n");
        printf("-----------------------------------------\n");
        printf("|                 MENU                  |\n");
        printf("|   Keys required to encrypt/decrypt!   |\n");
        printf("-----------------------------------------\n");
        printf("|   1. Generate key                     |\n");
        printf("|   2. Load key                         |\n");
        printf("|   3. Encryption                       |\n");
        printf("|   4. Decryption                       |\n");
        printf("|   5. Exit                             |\n");
        printf("-----------------------------------------\n");
        
        cout << "\nEnter a selection: ";
        cin.clear();
        cin >> selection;

        if(cin.fail()) {
            cout << "\nERROR: You did not enter an integer. Please enter again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            if(selection == 5) {
                exit = true;
            }
            else if(selection == 1) {
                GENERATE_KEY();
            }
            else if(selection == 2) {
                key = LOAD_KEY();
            }
            else if(key == "EMPTY") {
                cout << "\nMISSING KEY: Please load key.\n";
            }
            else if(selection == 3) {
                ENCRYPT(key);
            }
            else if(selection == 4) {
                DECRYPT(key);
            }
            else {
                cout << "\nERROR: Selection out of range. Please enter a selection again.\n";
            }
        }
    }
}

void GENERATE_KEY() 
{
    srand(time(0));
    string key_characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    string key;

    int size = key_characters.size();
    for(int x = 0; x < KEY_LENGTH; x++) {
        key.push_back(key_characters[rand() % size]);
    }

    WRITE_FILE("key.pem", key);

    cout << "Keys generated.\n";
}

string LOAD_KEY() 
{
    string key = READ_FILE("key.pem");
    cout << "Keys loaded.\n";
    return key;
}

void ENCRYPT(string key) 
{
    string filename = request_filename();
    string plaintext = remove_nonalpha(READ_FILE(filename));

    string enc_1 = SHIFT_ENCRYPT(plaintext, extract_first_5_integers(key));
    string enc_2 = ABASH_ENCRYPT(enc_1);
    string enc_3 = RSA_ENCRYPT(enc_2);
    string enc_4 = POLYALPHABETIC_ENCRYPT(enc_3, remove_integers(key));

    WRITE_FILE("encrypted.txt", enc_4);
}

void DECRYPT(string key) 
{
    string filename = request_filename();
    string ciphertext = remove_nonalpha(READ_FILE(filename));

    string dec_1 = POLYALPHABETIC_DECRYPT(ciphertext, remove_integers(key));
    string dec_2 = RSA_DECRYPT(dec_1);
    string dec_3 = ABASH_DECRYPT(dec_2);
    string dec_4 = SHIFT_DECRYPT(dec_3, extract_first_5_integers(key));

    WRITE_FILE("decrypted.txt", dec_4);
}

// FILE FUNCTIONS

string READ_FILE(string file_name) 
{
    while(file_exists(file_name) == false) {
        cout << "\nFile does not exist.\n";
        cout << "Please enter the file name again: ";
        cin >> file_name;
    }

    string line, file_content;
    ifstream File(file_name);

    while(getline(File, line)) {
        file_content.append(line);
    }

    File.close();

    return file_content;
}

void WRITE_FILE(string file_name, string text) 
{
    ofstream textFile(file_name);
    
    textFile << text;
    textFile.close();
    cout << "\nSuccessfully outputted file to " << file_name << "\n";
}

// ENCRYPTION AND DECRYPTION FUNCTIONS

string ABASH_ENCRYPT(string text) 
{
    string cipherText;
    int size = text.size();
    for(int x = 0; x < size; x++) {
        char letterCase = ch_case(text[x]);
        cipherText.push_back((letterCase + 25) + letterCase - text[x]);
    }

    return cipherText;
}

string ABASH_DECRYPT(string text) 
{
    string plainText;
    int size = text.size();
    for(int x = 0; x < size; x++) {
        char letterCase = ch_case(text[x]);
        plainText.push_back((letterCase + 25) + letterCase - text[x]);
    }

    return plainText;
}

string POLYALPHABETIC_ENCRYPT(string text, string key) 
{
    string cipherText;
    int Tsize = text.size();
    int Ksize = key.size();

    for(int x = 0; x < Tsize; x++) {
        char letterCase = ch_case(text[x]);
        char keyCase = key_case(text[x], key[x % Ksize]);
        cipherText.push_back((text[x] + keyCase - letterCase * 2) % 26 + letterCase);
    }

    return cipherText;
}

string POLYALPHABETIC_DECRYPT(string text, string key) 
{
    string plainText;
    int Tsize = text.size();
    int Ksize = key.size();

    for(int x = 0; x < Tsize; x++) {
        char letterCase = ch_case(text[x]);
        char keyCase = key_case(text[x], key[x % Ksize]);
        plainText.push_back((text[x] - keyCase + 26) % 26 + letterCase);
    }

    return plainText;
}

string RSA_ENCRYPT(string text) 
{
    int p, q;

    p = 2;
    q = 13;

    int n, phi, e;
    n = p * q;
    phi = (p - 1) * (q - 1);
    for(e = 2; e < phi && gcd(e, phi) != 1; e++) {}

    string cipherText;
    int size = text.size();
    for(int x = 0; x < size; x++) {
        char letterCase = ch_case(text[x]);
        int cipherInt = int_pow(text[x] - letterCase + 1, e);
        char letter = cipherInt % n + letterCase - 1;
        if(letter == 'a' - 1 || letter == 'A' - 1) {
            letter += 26;
        }
        cipherText.push_back(letter);
    }

    return cipherText;
}

string RSA_DECRYPT(string text) 
{
    int p, q;

    p = 2;
    q = 13;

    int n, phi, e, d;
    n = p * q;
    phi = (p - 1) * (q - 1);
    for(e = 2; e < phi && gcd(e, phi) != 1; e++) {}
    for(d = 1; e * d % phi != 1; d++) {}

    int size = text.size();
    string plainText;
    for(int x = 0; x < size; x++) {
        char letterCase = ch_case(text[x]);

        int plainInt = int_pow(text[x] - letterCase + 1, d);
        char letter = plainInt % n + letterCase - 1;
        if(letter == 'a' - 1 || letter == 'A' - 1) {
            letter += 26;
        }
        plainText.push_back(letter);
    }

    return plainText;
}

string SHIFT_ENCRYPT(string text, int shift) 
{
    string cipherText;
    int size = text.size();
    for(int x = 0; x < size; x++) {
        char letterCase = ch_case(text[x]);
        cipherText.push_back((text[x] - letterCase + (shift % 26)) % 26 + letterCase);
    }

    return cipherText;
}

string SHIFT_DECRYPT(string text, int shift) 
{
    string plainText;
    int size = text.size();
    for(int x = 0; x < size; x++) {
        char letterCase = ch_case(text[x]);
        plainText.push_back((text[x] - letterCase - (shift % 26) + 26) % 26 + letterCase);
    }

    return plainText;
}

// HELPER FUNCTIONS

int gcd(int a, int h) 
{
    int temp;

    while(1) {
        temp = a % h;
        if (temp == 0)
            return h;
        a = h;
        h = temp;
    }
}

int int_pow(int b, int e) 
{
    return (e == 0) ? 1 : b * int_pow(b, e - 1);
}

char ch_case(char ch) 
{
    return isupper(ch) ? 'A' : 'a';
}

char key_case(char ch, char key) 
{
    return isupper(ch) ? toupper(key) : tolower(key);
}

string remove_nonalpha(string text) 
{
    text.erase(remove_if(text.begin(), text.end(), [](char c) { return !isalpha(c); } ), text.end());
    return text;
}

bool file_exists(string name) 
{
    ifstream file(name);
    return file.good();
}

int extract_first_5_integers(string text) 
{
    int num = 0;
    int size = text.size();

    for(int x = 0; num < 10000 && x < size; x++) {
        if(isdigit(text[x])) {
            num = num * 10 + text[x] - '0';
        }
    }
    
    return num;
}

string remove_integers(string text) {
    text.erase(remove_if(text.begin(), text.end(), ::isdigit), text.end());
    return text;
}

string request_filename() 
{
    string filename;
    cout << "Enter file name: ";
    cin >> filename;
    return filename;
}