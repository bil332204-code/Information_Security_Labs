// CS-353 Information Security - Lab 03
// Task 2: Playfair Cipher (Decryption part)
// Key: EXPLANATION
// Usage: ./task2 [ciphertext]   (if you don't pass anything it just uses the Task 1 output)

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

char matrix[5][5]; // same key square as task 1, global so every function can see it

// build the 5x5 key matrix - literally copy pasted this from task1 since key building doesn't change
void buildMatrix(const string &key) {
    bool used[26] = {false};
    string letters;

    // put the key letters in first (no dupes)
    for (char ch : key) {
        if (!isalpha(static_cast<unsigned char>(ch))) continue;
        ch = toupper(static_cast<unsigned char>(ch));
        if (ch == 'J') ch = 'I'; // I and J are basically the same box here
        if (!used[ch - 'A']) {
            used[ch - 'A'] = true;
            letters += ch;
        }
    }
    // then fill in whatever letters are left over
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue; // no separate J box, skip it
        if (!used[ch - 'A']) {
            used[ch - 'A'] = true;
            letters += ch;
        }
    }
    // dump it all into the grid
    for (int i = 0; i < 25; i++)
        matrix[i / 5][i % 5] = letters[i];
}

// find row/col of a given letter in the matrix
void findPos(char ch, int &row, int &col) {
    if (ch == 'J') ch = 'I'; // just in case, treat J as I
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 5; c++)
            if (matrix[r][c] == ch) {
                row = r;
                col = c;
                return; // got it, no need to keep looping
            }
}

// clean up the input - drop spaces/punctuation, uppercase everything, J becomes I
// (works for ciphertext too since it's just letters at this point anyway)
string stripMessage(const string &text) {
    string out;
    for (char ch : text) {
        if (!isalpha(static_cast<unsigned char>(ch))) continue;
        ch = toupper(static_cast<unsigned char>(ch));
        if (ch == 'J') ch = 'I';
        out += ch;
    }
    return out;
}

// decrypting is basically encryption backwards - go left instead of right, up instead of down
// using +4 instead of -1 so we don't deal with negative numbers messing up the %5
string decryptPair(const string &p) {
    int r1, c1, r2, c2;
    findPos(p[0], r1, c1);
    findPos(p[1], r2, c2);

    string out;
    if (r1 == r2) {                          // same row -> go LEFT this time (opposite of encrypt)
        out += matrix[r1][(c1 + 4) % 5];
        out += matrix[r2][(c2 + 4) % 5];
    } else if (c1 == c2) {                   // same column -> go UP
        out += matrix[(r1 + 4) % 5][c1];
        out += matrix[(r2 + 4) % 5][c2];
    } else {                                 // rectangle case is same as encryption, just swap cols
        out += matrix[r1][c2];
        out += matrix[r2][c1];
    }
    return out;
}

// after decrypting we get extra Q's we added during encryption, gotta clean those out
// case 1: Q stuck between two same letters, like "EQE" -> really just "EE"
// case 2: a trailing Q we added because the message had odd length
string removePadding(const string &plain) {
    string out;
    for (size_t i = 0; i < plain.size(); i++) {
        // check if this Q is sandwiched between two identical letters -> skip it, it's filler
        if (plain[i] == 'Q' && i > 0 && i + 1 < plain.size() &&
            plain[i - 1] == plain[i + 1])
            continue;
        out += plain[i];
    }
    // also chop off a trailing Q if it's just there as padding
    if (!out.empty() && out.back() == 'Q')
        out.pop_back();
    return out;
}

int main(int argc, char *argv[]) {
    string key = "EXPLANATION";
    // default ciphertext is literally the output from task 1, so I can just run this without args and check my work
    string ciphertext = "OTGISQXBOBBKNKYKOIWOZBKVMBIBAXLSIK";
    if (argc > 1) ciphertext = argv[1]; // but if user passes their own ciphertext, use that instead

    buildMatrix(key);

    cout << "=========== PLAYFAIR CIPHER - DECRYPTION ===========\n\n";
    cout << "Key: " << key << "\n\n";

    cout << "Key Matrix (I/J share one cell):\n";
    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 5; c++) {
            if (matrix[r][c] == 'I') cout << "I/J ";
            else cout << matrix[r][c] << "   ";
        }
        cout << "\n";
    }

    string cipher = stripMessage(ciphertext);
    cout << "\nCiphertext        : " << ciphertext << "\n";

    // playfair pairs must be even in count, if not something's wrong with the input
    if (cipher.size() % 2 != 0) {
        cout << "Error: ciphertext must have an even number of letters.\n";
        return 1;
    }

    vector<string> cipherPairs;
    for (size_t i = 0; i < cipher.size(); i += 2) {
        string p = cipher.substr(i, 2);
        // sanity check - a proper playfair ciphertext should never have identical letters in a pair
        if (p[0] == p[1]) {
            cout << "Error: invalid ciphertext pair \"" << p
                 << "\" (identical letters never occur in Playfair ciphertext).\n";
            return 1;
        }
        cipherPairs.push_back(p);
    }

    cout << "Cipher pairs      : ";
    for (const string &p : cipherPairs) cout << p << " ";
    cout << "\n";

    // now decrypt each pair one at a time and stick the results together
    vector<string> plainPairs;
    string plain;
    for (const string &p : cipherPairs) {
        string d = decryptPair(p);
        plainPairs.push_back(d);
        plain += d;
    }

    cout << "Decrypted pairs   : ";
    for (const string &d : plainPairs) cout << d << " ";
    cout << "\n";

    cout << "\nDecrypted (raw)   : " << plain << "\n"; // this still has the extra Q's in it
    cout << "Decrypted (clean) : " << removePadding(plain) << "\n"; // this is the actual readable message
    return 0;
}