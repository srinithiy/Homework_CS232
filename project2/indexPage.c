#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Trie Node Structure
struct TrieNode {
    int count; // count for the word
    struct TrieNode* children[26]; // pointers to children nodes for each letter
};

// Function Prototypes
struct TrieNode* createTrieNode();
struct TrieNode* indexPage(const char* url);
int addWordOccurrence(struct TrieNode* root, const char* word, const int wordLength);
void printTrieContents(struct TrieNode* root, char* buffer, int bufferSize, int bufferLength);
void freeTrieMemory(struct TrieNode* root);
int getText(const char* srcAddr, char* buffer, const int bufSize);

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <URL>\n", argv[0]);
        return 1;
    }

    const char* url = argv[1];

    struct TrieNode* root = indexPage(url);

    if (root != NULL) {
        printTrieContents(root, NULL, 0, 0);
        freeTrieMemory(root);
    }

    return 0;
}

struct TrieNode* indexPage(const char* url) {
    char buffer[300001];
    int bytesRead = getText(url, buffer, sizeof(buffer));

    if (bytesRead > 0) {
        struct TrieNode* root = createTrieNode();
        char wordBuffer[100]; // Adjust the size as needed

        int i = 0;
        int wordLength = 0;

        while (i < bytesRead) {
            if (isalpha(buffer[i])) {
                wordBuffer[wordLength++] = tolower(buffer[i]);
            } else if (wordLength > 0) {
                wordBuffer[wordLength] = '\0';
                addWordOccurrence(root, wordBuffer, wordLength);
                wordLength = 0;
            }
            i++;
        }

        if (wordLength > 0) {
            wordBuffer[wordLength] = '\0';
            addWordOccurrence(root, wordBuffer, wordLength);
        }

        return root;
    } else {
        fprintf(stderr, "Error reading the web page content.\n");
        return NULL;
    }
}

int addWordOccurrence(struct TrieNode* root, const char* word, const int wordLength) {
    struct TrieNode* current = root;

    for (int i = 0; i < wordLength; i++) {
        int index = word[i] - 'a';

        if (current->children[index] == NULL) {
            current->children[index] = createTrieNode();
        }

        current = current->children[index];
    }

    current->count++;
    return 1; // Success
}

void printTrieContents(struct TrieNode* root, char* buffer, int bufferSize, int bufferLength) {
    if (root == NULL) {
        return;
    }

    if (buffer == NULL) {
        buffer = (char*)malloc(bufferSize * sizeof(char));
        bufferLength = 0;
    }

    if (root->count > 0) {
        buffer[bufferLength] = '\0';
        printf("%s: %d\n", buffer, root->count);
    }

    for (int i = 0; i < 26; i++) {
        if (root->children[i] != NULL) {
            buffer[bufferLength] = 'a' + i;
            printTrieContents(root->children[i], buffer, bufferSize, bufferLength + 1);
        }
    }

    if (bufferLength == 0) {
        free(buffer);
    }
}

void freeTrieMemory(struct TrieNode* root) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < 26; i++) {
        freeTrieMemory(root->children[i]);
    }

    free(root);
}

struct TrieNode* createTrieNode() {
    struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (node != NULL) {
        node->count = 0;
        for (int i = 0; i < 26; ++i) {
            node->children[i] = NULL;
        }
    }
    return node;
}

int getText(const char* srcAddr, char* buffer, const int bufSize) {
    FILE* pipe;
    int bytesRead;

    snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

    pipe = popen(buffer, "r");
    if (pipe == NULL) {
        fprintf(stderr, "ERROR: could not open the pipe for command %s\n", buffer);
        return 0;
    }

    bytesRead = fread(buffer, sizeof(char), bufSize - 1, pipe);
    buffer[bytesRead] = '\0';

    pclose(pipe);

    return bytesRead;
}