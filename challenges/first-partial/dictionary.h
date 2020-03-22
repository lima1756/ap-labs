#define DICTIONARY_SIZE 69697

typedef struct _package package;
package *addPackage(char *name, char *installedDate);
package *updatePackate(char *name, char *updateDate);
package *removePackage(char *name, char *removeDate);
void destroyDictionary();
void printDictionary();
void writeDictionary(int fd);
