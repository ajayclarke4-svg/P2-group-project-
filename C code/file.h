#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include "employee.h"

FILE *open_file();
void close_file(FILE *file);
void Clean_file(struct employee *emp);

#endif