# Lab 03: Playfair Cipher

**CS-353: Information Security** | NUST SEECS, Department of Computing

| | |
|---|---|
| **Student** | Bilal Ahmed |
| **Class** | BS Computer Science — Semester 3 |
| **Topic** | Classical symmetric block cipher (Playfair) |
| **Language** | C++ (g++ 13.2) |
| **Environment** | Windows, VS Code |

**CLO-3:** Assembles information security solutions by applying key concepts using a variety of tools and techniques.

---

## Objective

To understand the Playfair cipher and implement encryption and decryption of text in C++.

## Files

| File | Description |
|---|---|
| `task1_playfair_encrypt.cpp` | Task 1: Playfair encryption |
| `task2_playfair_decrypt.cpp` | Task 2: Playfair decryption |

## How Playfair Works

Playfair encrypts text in **pairs of letters** (digraphs) using a 5×5 key matrix.

**1. Build the key matrix.** Write the key with duplicate letters removed, then fill the remaining cells with the rest of the alphabet. `I` and `J` share one cell, so the grid holds 25 letters.

**2. Prepare the message.** Remove spaces, punctuation and case, and treat `J` as `I`. Split the result into pairs. If a pair would contain two identical letters, insert a filler `Q` between them. If one letter is left over at the end, pad it with `Q`.

**3. Encrypt each pair** using the position of its two letters in the matrix:

| Situation | Encryption | Decryption |
|---|---|---|
| Same row | Replace each letter with the one to its **right** (wraps around) | Replace each with the one to its **left** |
| Same column | Replace each letter with the one **below** (wraps around) | Replace each with the one **above** |
| Rectangle | Swap columns (keep each letter's row) | Same as encryption |

## Task 1: Encryption

**Key:** `EXPLANATION` (letters used: `EXPLANTIO`)

**Key matrix:**

```
E   X   P   L   A
N   T   I/J O   B
C   D   F   G   H
K   M   Q   R   S
U   V   W   Y   Z
```

**Plaintext:** `Information security is a "MUST" to learn.`

**Stripped message (33 letters):** `INFORMATIONSECURITYISAMUSTTOLEARN`

Since 33 is odd, a filler `Q` is added at the end to make full pairs.

**Pairs:**

```
IN FO RM AT IO NS EC UR IT YI SA MU ST TO LE AR NQ
```

**Ciphertext:**

```
OTGISQXBOBBKNKYKOIWOZBKVMBIBAXLSIK
```

The full pair-by-pair breakdown, including which rule applied to each pair, is in the lab report.

## Task 2: Decryption

Decryption uses the same key matrix and reverses the row and column rules. The decrypted pairs are joined, then the filler `Q` characters are removed to recover the stripped plaintext.

| | |
|---|---|
| **Ciphertext** | `OTGISQXBOBBKNKYKOIWOZBKVMBIBAXLSIK` |
| **Decrypted (raw)** | `INFORMATIONSECURITYISAMUSTTOLEARNQ` |
| **Decrypted (filler removed)** | `INFORMATIONSECURITYISAMUSTTOLEARN` |

Spaces, capitalisation and punctuation cannot be recovered, because they are discarded before encryption.

## Build and Run

Requires a C++ compiler such as `g++`.

**Task 1: Encryption**

```bash
g++ -std=c++17 task1_playfair_encrypt.cpp -o task1
./task1          # on Windows: .\task1.exe
```

The key and plaintext are hardcoded in `main()`. Edit the `key` and `plaintext` variables to try your own.

**Task 2: Decryption**

```bash
g++ -std=c++17 task2_playfair_decrypt.cpp -o task2
./task2          # decrypts the default ciphertext from Task 1
./task2 "OTGISQXBOBBKNKYKOIWOZBKVMBIBAXLSIK"    # or pass your own ciphertext
```

The ciphertext must have an even number of letters and no pair of identical letters. The program reports an error otherwise.

## Sample Output

**Task 1**

```
=========== PLAYFAIR CIPHER - ENCRYPTION ===========

Key: EXPLANATION

Key Matrix (I/J share one cell):
E   X   P   L   A
N   T   I/J O   B
C   D   F   G   H
K   M   Q   R   S
U   V   W   Y   Z

Actual message   : Information security is a "MUST" to learn.
Stripped message : INFORMATIONSECURITYISAMUSTTOLEARN

Pairs            : IN FO RM AT IO NS EC UR IT YI SA MU ST TO LE AR NQ
Encrypted pairs  : OT GI SQ XB OB BK NK YK OI WO ZB KV MB IB AX LS IK

Ciphertext       : OTGISQXBOBBKNKYKOIWOZBKVMBIBAXLSIK
```

**Task 2**

```
=========== PLAYFAIR CIPHER - DECRYPTION ===========

Key: EXPLANATION

Key Matrix (I/J share one cell):
E   X   P   L   A
N   T   I/J O   B
C   D   F   G   H
K   M   Q   R   S
U   V   W   Y   Z

Ciphertext        : OTGISQXBOBBKNKYKOIWOZBKVMBIBAXLSIK
Cipher pairs      : OT GI SQ XB OB BK NK YK OI WO ZB KV MB IB AX LS IK
Decrypted pairs   : IN FO RM AT IO NS EC UR IT YI SA MU ST TO LE AR NQ

Decrypted (raw)   : INFORMATIONSECURITYISAMUSTTOLEARNQ
Decrypted (clean) : INFORMATIONSECURITYISAMUSTTOLEARN
```

## Limitations

- Playfair is a classical cipher and is **not secure** by modern standards. It is included here for learning only.
- `I` and `J` are merged, so a decrypted `J` comes back as `I`.
- Decryption removes any trailing `Q`, so a message that genuinely ends in `Q` would lose that letter.
- The filler `Q` is stripped only when it sits between two identical letters or at the very end. A real `Q` elsewhere in the message is kept.

## Conclusion

Playfair encrypts letters in pairs using a 5×5 key matrix. Decryption follows the same rules in reverse, giving back the original message (without its spaces, case and punctuation).
