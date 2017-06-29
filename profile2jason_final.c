//
// profile2jason.c
//NOTE: This program is heatflow equation specific!!!!!  
// Takes profile information from the TAU output and 
// print high level performance information:
// 1) Work flow performance:
//	"execution time" in msec/minutes. If its in 00:00.000 format then the time is in minutes
//	otherwise its in msec	
// 2) Detail performance for each component:
//	name of the component
//	number of processors
// 	aggregate execution time (in msec/minutes)
//	aggregate communication time (in msec/minutes)
//	idle time (in msec/minutes)
// 3)Intra_component_performance:	
//	volume of communications in bytes
//	number of messages		 
// Created by Abid Malik on 6/28/17.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>

#define LENGTH 1024                                             // length of a string
#define COMPONENTS 2                                            // number of components in the workflow
								// for the heat flow equation we have two components


typedef struct jasonfileformat
{
    char name_of_the_component [COMPONENTS][LENGTH];
    int  number_of_processors_for_component[COMPONENTS];
    char  aggregate_execution_time[COMPONENTS][LENGTH];
    char  aggregate_communication_time[COMPONENTS][LENGTH];
    char  idle_time[COMPONENTS][LENGTH];
    char  communication_volume_between_components[COMPONENTS][LENGTH];
    char number_of_messages_between_components[COMPONENTS][LENGTH];
    char  workflow_exection_time [LENGTH];
    
} codar;


void process_workflow_time(char*, codar*);
void process_components (char*, codar*, int);
void print_JSON(codar*, char*);


int main (int argc, char** argv){
    
    char path_to_file [COMPONENTS+2][LENGTH];
    codar *jayfile;
    
    if ( argc < (COMPONENTS+2))
    {
        printf ("******Insufficent input parameters******\n");
        printf ("You need %d input parameters\n", COMPONENTS+1);
        printf ("./prof2jason [path_to_timefile] [path_to_component_1_profile_files] [path_to_component_2_profile_files] .... path_to_component_N_profile_files] [path_for_JSON_file]\n");
        exit(0);
        
    }
    
    jayfile = (codar*) malloc ( sizeof (codar));
    
    for ( int i=1; i < argc; i++){
        
        strcpy( path_to_file[i-1], argv[i]);
        printf ( "The path to file-%d is %s\n", i, path_to_file[i-1]);
    }
    
    
    process_workflow_time (path_to_file[0], jayfile );
    
    for ( int j=0; j < COMPONENTS; j++){
        
        process_components (path_to_file[j+1], jayfile, j);
        
    }
    
    print_JSON(jayfile, path_to_file[COMPONENTS+1]);
    
    free(jayfile);
    
    return 0;
}






void process_workflow_time (char* path, codar* format){
    
    FILE* ifp;
    char line[LENGTH];
    
    ifp = fopen (path, "r");
    if (ifp == NULL){
        printf (" unable to open the file at---- %s\n", path);
        exit(1);
    }
    
    while (fscanf(ifp,"%s", line) != EOF){
    }
    
    strcpy(format->workflow_exection_time,line);    
    fclose(ifp);
}


void process_components(char* path, codar* format, int i){
    
    FILE* ifp;
    char line[LENGTH];
    char table [COMPONENTS][LENGTH][LENGTH];
    char adios_table[COMPONENTS][LENGTH][LENGTH];
    
    int counter_s;
    int counter_a;
    int tidx_s, tidx_a;
    int j;
    
    ifp =fopen (path, "r");
    if (ifp == NULL){
        printf (" Unable to open file at --%s\n", path);
        exit(1);
    }
    
    counter_s = 0;
    counter_a = 0;
    tidx_s = 0;
    tidx_a = 0;
    
    
    
    while (fscanf(ifp,"%s", line) != EOF){
      if (strcmp(line,"SUMMARY")==0){
            counter_s++;
        }

        if (counter_s ==1){
            strcpy(table[i][tidx_s], line);
            tidx_s++;
        }

        if ((strcmp(line,"EVENTS")==0) && (i==0)){
              counter_a++;
        }

        if ((counter_a == 1) && strcmp(line, "NODE")==0)
            counter_a++;
        if (counter_a ==1){
            strcpy(adios_table[i][tidx_a], line);
            tidx_a++;
        }     
    }

    int temp;
    for (j=0; j < tidx_s; j++){
        if ( strcmp(table[i][j],".TAU")==0){
            temp = atoi(table[i][j+5]);
            format->number_of_processors_for_component[i]=temp;	//number of processors
            strcpy(format->name_of_the_component[i], table[i][j+9+(i*4)]);//name of the component
            strcpy(format->aggregate_execution_time[i], table[i][j+4]);// aggregate execution time
            strcpy(format-> aggregate_communication_time[i], table[i][j+22-(i*5)]);// aggregate communication time
                    }
        if (strcmp(table[i][j],"MPI_Barrier()")==0){
            // calculating idle time
            strcpy(format->idle_time[i], table[i][j-4]);
        }
            
    }
    
    counter_a = 0;
    
    for (j=0; j < tidx_a; j++){
        if ( strcmp(adios_table[i][j],"ADIOS")==0 && counter_a==0){
            counter_a++;
            strcpy(format->number_of_messages_between_components[i], adios_table[i][j-5]);// number of messages
            strcpy(format->communication_volume_between_components[i], adios_table[i][j-2]);// volume of the communications        
        }
    }
    
    fclose (ifp);
}


void print_JSON (codar* format, char* path){
    
    
    
    FILE* ofp = fopen(path, "w") ;
    printf("creating the jason file at %s\n", path);
  //**************************
    fprintf(ofp, "{\n");
    fprintf(ofp, "\t \"Work_flow_performance\":{\n");
    fprintf(ofp, "\t \t \t \"execution_time\": %s\n", format->workflow_exection_time);
    fprintf(ofp, "\t \t \t \t},\n");
    fprintf (ofp, "\t \"Detail_of_each_Component\":[\n");
    fprintf (ofp, "\t \t{\n");
    fprintf (ofp, "\t \t \t \"name\":\"%s]\",\n", format->name_of_the_component[0]);
    fprintf (ofp, "\t \t \t \"processors\": %d,\n", format->number_of_processors_for_component[0]);
    fprintf (ofp, "\t \t \t \"aggr_execution_time\":\"%s\",\n", format->aggregate_execution_time[0]);
    fprintf (ofp, "\t \t \t \"aggr_communication_time\":\"%s\",\n", format->aggregate_communication_time[0]);
    fprintf (ofp, "\t \t \t \"idle_time\":\" %s\"\n", format->idle_time[0]);
    fprintf (ofp, "\t \t \t},\n");
    fprintf (ofp, "\t \t{\n");
    fprintf (ofp, "\t \t \t \"name\":\" %s]\",\n", format->name_of_the_component[1]);
    fprintf (ofp, "\t \t \t \"processors\": %d,\n", format->number_of_processors_for_component[1]);
    fprintf (ofp, "\t \t \t \"aggr_execution_time\": \"%s\",\n", format->aggregate_execution_time[1]);
    fprintf (ofp, "\t \t \t \"aggr_communication_time\":\"%s\",\n", format->aggregate_communication_time[1]);
    fprintf (ofp, "\t \t \t \"idle_time\":%s\n", format->idle_time[1]);
    fprintf (ofp, "\t \t \t}\n");
    fprintf (ofp, "\t ],\n");
    fprintf(ofp, "\t \"Intra_component_detail\":{\n");
    fprintf(ofp, "\t \t \t \"volume_of_communication\": \" %s x %s\",\n", format->communication_volume_between_components[0],format->number_of_messages_between_components[0]);
    fprintf(ofp, "\t \t \t \"number_of_messages\": %s\n", format->number_of_messages_between_components[0]);
    fprintf(ofp, "\t \t \t \t}\n");
    fprintf(ofp, "}\n");
  //******************************
    printf("\nDone!!!!\n");
    fclose(ofp);
    
    
}





