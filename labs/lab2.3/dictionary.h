#define DICTIONARY_SIZE 69697

struct entry *getElement(char *key);
struct entry *addElement(char *key, int len, int data);
void printDictionary();

