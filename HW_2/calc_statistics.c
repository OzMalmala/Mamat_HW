#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum { HISTOGRAM_SIZE = 101};

// functions decleration
void calculations(int *grades, 
                  int num_of_students,
                  int *histogram, 
                  int *min, 
                  int *max,
                  double *avg,
                  double *pass_rate,
                  int *median);


int main(int argc, char *argv[]) {
    if (argc < 1) {
        fprintf(stderr, "Error\n");
        return 1;
    }

    //opening the files
    FILE *grades_file, *course_statistics;
    grades_file = fopen(argv[2], "r"); //the argumengt argv[2] is the path to the grade file sent from script
    if (grades_file ==NULL){
        printf("Could not open file\n");
        return 1;
    }


    course_statistics = fopen(argv[3], "w");//the argumengt argv[3] is the path to the course statistics file sent from script
    if (course_statistics ==NULL){
        printf("Could not open file\n");
        return 1;
    }

    // set the grades array (the first argument sent to the function is num of students)
    int num_of_students = atoi(argv[1]);
    fprintf(course_statistics,"num of students = %d\n", num_of_students);

    
    int *grades_arr;
    grades_arr = malloc(sizeof(int)*num_of_students);
    if (grades_arr == NULL){
        fprintf(stderr, "Malloc failed");
        return 1;
    }
    for (int i=0 ; i<num_of_students; i++){
        fscanf(grades_file, "%d", &grades_arr[i]);
    }

    //min, max,avg, pass rate, histogram calculations
    int histogram_arr[HISTOGRAM_SIZE] = {0};
    int  min, max, median;
    double  avg, pass_rate;

    min = grades_arr[0];
    max = grades_arr[0];


    calculations(grades_arr,
                 num_of_students,
                 histogram_arr,
                 &min,
                 &max,
                 &avg,
                 &pass_rate,
                 &median);

    //printing to the txt file
    char c ='%';
    fprintf(course_statistics,"avg = %0.3f\n", avg);
    fprintf(course_statistics,"the median is - %d\n", median);
    fprintf(course_statistics,"max grade = %d, min grade = %d\n", max,min);
    fprintf(course_statistics,"pass rate = %0.2f%c\n", pass_rate,c);

    int line_counter =0;
    for (int i=1; i<HISTOGRAM_SIZE; i++){
        line_counter++;
        if(line_counter%10 == 0){
            fprintf(course_statistics,"%d", histogram_arr[i]);
            fprintf(course_statistics,"\n");
        }
        else{
            fprintf(course_statistics,"%d ", histogram_arr[i]);
        }

    }


    // close files and free memory  
    fclose(grades_file);
    fclose(course_statistics);
    free(grades_arr);
    return 0;
}

void calculations(int *grades, 
                  int num_of_students,
                  int *histogram, 
                  int *min, 
                  int *max,
                  double *avg,
                  double *pass_rate,
                  int *median){

    double sum = 0, pass = 0;
    for(int i=0; i<num_of_students; i++ ){

        if(grades[i] > *max){
            *max = grades[i];
        }

        if(grades[i] < *min){
            *min = grades[i];
        }

        if(grades[i] >= 55){
            pass++;
        }

        sum = sum + grades[i];

        histogram[grades[i]]++;
    }

    *avg = sum/num_of_students;
    *pass_rate = (pass/num_of_students)*100;


    int median_location = (num_of_students+1)/2, med_count=0;
    for(int i=1; i<HISTOGRAM_SIZE; i++){
        med_count = med_count +histogram[i];
        if(med_count >= median_location){
            *median = i;
            break;
        }

    }


}
