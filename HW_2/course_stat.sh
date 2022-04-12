#!/bin/bash
if [[ ! $# -eq 1 ]] ; then #check if the number of arguments is valid.
	cat out1.txt >&2  
    exit 1
fi

if [[ ! -f $1.txt ]] ; then #check if the course exists.
	cat out2.txt >&2  
    exit 1  
fi

if [[ -d $1_stat ]]; then
	rm -r $1_stat
fi 

mkdir -p $1_stat #create the new folder
cat $1.txt | while read -r id grade ; do #copy the grades column into a new txt file in the new folder.
	grades=$grade
	echo "$line$grades" | grep "[0-9]" >> $1_stat/grade.txt
done < $1.txt


num_of_students=$(cat $1_stat/grade.txt | wc -l)

gcc -g -Wall calc_statistics.c -o prog.exe #compile and llink the c program
./prog.exe $num_of_students $1_stat/grade.txt $1_stat/course_statistics.txt #run the program, send num of students ad the directory name as argument
cat $1_stat/course_statistics.txt #print the result