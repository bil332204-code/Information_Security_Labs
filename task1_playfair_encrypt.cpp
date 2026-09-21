// CS-353 Information Security - Lab 03
// Task 1: Playfair Cipher (Encryption part)
// key = EXPLANATION, plaintext = Information security is a "MUST" to learn.
// ref: standard playfair algo from class  + some youtube video I watched 

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

char matrix[5][5]; // the 5x5 key square, global so I don't have to pass it everywhere

// builds the key matrix. I and J go in the same box (just doing I everywhere, easier)
void buildMatrix(const string &key) {
    bool used[26] = {false}; // keeping track of which letters already placed
    string letters;

    // first put in the key letters (no repeats obviously)
    for (char ch : key) {
        if (!isalpha(static_cast<unsigned char>(ch))) continue; // skip weird chars just in case
        ch = toupper(static_cast<unsigned char>(ch));
        if (ch == 'J') ch = 'I'; // merge J into I like the rule says
        if (!used[ch - 'A']) {
            used[ch - 'A'] = true;
            letters += ch;
        }
    }

    // now dump the rest of the alphabet in (skip J since it's merged)
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue; // no J, already handled above
        if (!used[ch - 'A']) {
            used[ch - 'A'] = true;
            letters += ch;
        }
    }

    // now just fill it row by row into the matrix
    for (int i = 0; i < 25; i++)
        matrix[i / 5][i % 5] = letters[i];
}

// finds where a letter sits in the matrix (row, col) - passed by reference so we get both back
void findPos(char ch, int &row, int &col) {
    if (ch == 'J') ch = 'I'; // again, J doesn't exist here
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 5; c++)
            if (matrix[r][c] == ch) {
                row = r;
                col = c;
                return; // found it, bail out early
            }
}

// cleans up the message - strips spaces/punctuation, makes everything uppercase, fixes J->I
string stripMessage(const string &text) {
    string out;
    for (char ch : text) {
        if (!isalpha(static_cast<unsigned char>(ch))) continue; // ignore quotes, spaces, periods etc
        ch = toupper(static_cast<unsigned char>(ch));
        if (ch == 'J') ch = 'I';
        out += ch;
    }
    return out;
}

// chops the cleaned text into pairs of 2 letters
// rule: if both letters in a pair are same, stick a Q in between (could've used X but prof used Q in example)
// if the last letter is left alone at the end, pad it with a Q too
vector<string> makePairs(const string &text) {
    vector<string> pairs;
    size_t i = 0;
    while (i < text.size()) {
        char a = text[i];
        if (i + 1 == text.size()) {          // odd one out at the end
            pairs.push_back(string(1, a) + 'Q');
            break;
        }
        char b = text[i + 1];
        if (a == b) {                        // same letter twice in a row, gotta break it up
            pairs.push_back(string(1, a) + 'Q');
            i += 1; // only move forward by 1, not 2, since we "used" the second one as filler
        } else {
            pairs.push_back(string(1, a) + b);
            i += 2;
        }
    }
    return pairs;
}

// does the actual encryption for ONE pair of letters
string encryptPair(const string &p) {
    int r1, c1, r2, c2;
    findPos(p[0], r1, c1);
    findPos(p[1], r2, c2);

    string out;
    if (r1 == r2) {                          // same row -> shift both right (wrap around with %5)
        out += matrix[r1][(c1 + 1) % 5];
        out += matrix[r2][(c2 + 1) % 5];
    } else if (c1 == c2) {                   // same column -> shift both down
        out += matrix[(r1 + 1) % 5][c1];
        out += matrix[(r2 + 1) % 5][c2];
    } else {                                 // forms a rectangle -> swap the columns, keep same row
        out += matrix[r1][c2];
        out += matrix[r2][c1];
    }
    return out;
}

int main() {
    // hardcoding these since the lab gave us a fixed key + plaintext
    string key = "EXPLANATION";
    string plaintext = "Information security is a \"MUST\" to learn.";

    buildMatrix(key); // build the 5x5 square first, need it for everything else

    cout << "=========== PLAYFAIR CIPHER - ENCRYPTION ===========\n\n";
    cout << "Key: " << key << "\n\n";

    // just printing the matrix so we can see it in the output, showing I/J together like prof wants
    cout << "Key Matrix (I/J share one cell):\n";
    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 5; c++) {
            if (matrix[r][c] == 'I') cout << "I/J ";
            else cout << matrix[r][c] << "   ";
        }
        cout << "\n";
    }

    string stripped = stripMessage(plaintext);
    cout << "\nActual message   : " << plaintext << "\n";
    cout << "Stripped message : " << stripped << "\n"; // this is what actually gets encrypted

    vector<string> pairs = makePairs(stripped);
    cout << "\nPairs            : ";
    for (const string &p : pairs) cout << p << " ";
    cout << "\n";

    // now loop through each pair and encrypt it one by one, building up ciphertext as we go
    vector<string> cipherPairs;
    string ciphertext;
    for (const string &p : pairs) {
        string e = encryptPair(p);
        cipherPairs.push_back(e);
        ciphertext += e;
    }

    cout << "Encrypted pairs  : ";
    for (const string &e : cipherPairs) cout << e << " ";
    cout << "\n";

    cout << "\nCiphertext       : " << ciphertext << "\n"; // final answer

    return 0; // done, that's it
}