typedef struct Memory{
	int start;
	int length;
	struct Memory *next;
} Memory;


void initMemory(Memory *memory, int length);
void printMemory(Memory *memory);
void allocateMemory(Memory *memory, int start, int length);
void freeMemory(Memory *memory, int start, int length);