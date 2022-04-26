//file1.c
#include <stdio.h>
#include <stdlib.h>
#include "file1.h"
#include "file2.h"
int file1_function(){
	printf("This is file1_function()\n");
	file2_function();
	return 0;
}
