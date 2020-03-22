#define DICTIONARY_SIZE 69697

typedef struct _package package;
package *addPackage(char *name, char *installedDate, int *counter);
package *updatePackate(char *name, char *updateDate, int *counter);
package *removePackage(char *name, char *removeDate, int *counter);
void destroyDictionary();
void printDictionary();
void writeDictionary(int fd);
