#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
};

// Initializes a trie node
struct TrieNode *createTrieNode()
{
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    node->isEndOfWord = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

// Trie structure
struct Trie
{
    struct TrieNode *root;
};

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *trie = (struct Trie *)malloc(sizeof(struct Trie));
    if (trie == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    trie->root = createTrieNode();
    return trie;
}

// Inserts a word into the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct TrieNode *crawl = pTrie->root;
    for (int i = 0; i < strlen(word); i++)
    {
        int index = word[i] - 'a';
        if (crawl->children[index] == NULL)
        {
            crawl->children[index] = createTrieNode();
        }
        crawl = crawl->children[index];
    }
    // Mark the end of the word
    crawl->isEndOfWord++;
}


// Helper function to recursively deallocate trie nodes
void deallocateTrieNodes(struct TrieNode *node)
{
    if (node == NULL)
    {
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        deallocateTrieNodes(node->children[i]);
    }
    free(node);
}

// Deallocates the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL)
    {
        return NULL;
    }
    deallocateTrieNodes(pTrie->root);
    free(pTrie);
    return NULL;
}

// Helper function to search for a word in the trie and count its occurrences
int searchWord(struct TrieNode *root, char *word)
{
    struct TrieNode *crawl = root;
    for (int i = 0; i < strlen(word); i++)
    {
        int index = word[i] - 'a';
        if (crawl->children[index] == NULL)
        {
            return 0; // Word not found
        }
        crawl = crawl->children[index];
    }
    // Check if it's the end of a word
    if (crawl != NULL && crawl->isEndOfWord)
    {
        return 1; // Found the word
    }
    return 0; // Word not found
}




// Computes the number of occurrences of a word in the trie
int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    struct TrieNode *crawl = pTrie->root;
    for (int i = 0; i < strlen(word); i++)
    {
        int index = word[i] - 'a';
        if (crawl->children[index] == NULL)
        {
            return 0; // Word not found
        }
        crawl = crawl->children[index];
    }
    // Check if the word exists and return its occurrence count
    return crawl->isEndOfWord; // Return the count of occurrences
}


// Reads the dictionary file and stores the words in the array
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    int numWords;
    fscanf(file, "%d", &numWords);

    for (int i = 0; i < numWords; ++i)
    {
        pInWords[i] = (char *)malloc(20 * sizeof(char)); // Assuming maximum word length of 19 characters
        fscanf(file, "%s", pInWords[i]);
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Test the trie structure with sample words
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate the trie structure
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}
