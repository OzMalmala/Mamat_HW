//file4.c
#include <stdio.h>
#include <stdlib.h>
#include "file4.h"
#include "file1.h"
#include "file2.h"
int file4_function(){
	printf("This is file4_function()\n");
	file1_function();
	file2_function();
	return 0;
}
