#ifndef FILE_H_
#define FILE_H_
#include "queue.h"

void file_write(struct RecordQueue q, const char* filename);
void file_read(struct RecordQueue* q, const char* filename);

#endif // FILE_H_
