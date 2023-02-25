#include "funcs.h"

void getword( char line[] , char word[] , int* curr ){

	int i = 0;
	
	while( line[*curr] == ' ' || line[*curr] == '\t' || line[*curr] == ','){
	
		(*curr)++;
	
	}
	
	while( *curr < MAX_LINE && line[*curr] != ' ' && line[*curr] != '\t' && line[*curr] != '\n' && line[*curr] != ',' ){
	
		word[i] = line[*curr];
		i++;
		(*curr)++;
	
	}
	(*curr)++;
	word[i] = '\0';


}

void getnum( char line[] , char num[] , int* curr ){

	
	int i = 0;
	
	while( line[*curr] == ' ' || line[*curr] == '\t'){
	
		(*curr)++;
	
	}
	
	if( isdigit( line[*curr])  || line[*curr] == '+' || line[*curr] == '-' ){
	
		while( *curr < MAX_LINE  && line[*curr] != ' ' && line[*curr] != '\t' && line[*curr] != '\n' && line[*curr] != ','){
	
			if( line[*curr] != '+' ){
				num[i] = line[*curr];
				i++;
			}
			(*curr)++;
	
		}
	}
	num[i] = '\0';



}


void getcomma( char line[] , int* curr ){

	while( line[*curr] == ' ' || line[*curr] == '\t'){
	
		(*curr)++;
	
	}
	
	if( line[*curr] == ','){
		(*curr)++;
	}	
	
	while( line[*curr] == ' ' || line[*curr] == '\t'){
	
		(*curr)++;
	
	}

}


void getstr( char line[] , int* curr , int* counter , char** p , int *dc){

	char* q;
	char ch;

	
	while( line[*curr] == ' ' || line[*curr] == '\t'){
	
		(*curr)++;
	
	}
	if( line[*curr] == '"'){
	
		(*curr)++;
		while ( line[*curr] != '"'){
			ch = line[*curr];
		
			(*dc)++;
			q = ( char * ) realloc ( *p , ( *dc  ) * 14   );
			*p = q;
			insertdata( ch ,  &p , &counter);
	
	
			(*curr)++;
		}
		
		(*dc)++;
	q = ( char * ) realloc ( *p , ( *dc  ) * 14   );
	*p = q;
	ch = 0 ;
	insertdata( ch ,  &p , &counter);

	
	}
	
}

void insertdata( char ch , char ***p , int **counter){

	char mask = 1;
	int i;
	char bin[14];
	for( i = 0 ; i < 6 ; i++){
		bin[i] = 0 ;
	}
	for(i = 7 ; i>=0 ;i-- ){
		mask = mask << i;
		mask = mask & ch ;
		mask = mask >> i;
		bin [ 13 - i] = mask ;
		mask =1;
	}

	for( i = 0; i<14 ;  i++ ){
	
		*( **p + **counter) = bin[i];
		(**counter)++;
	}
	
	
}

void insertnum(int num , char **p , int* counter , int* dc){

	unsigned int mask = 1;
	int i;
	
	char* q = NULL;
	q = ( char * ) realloc ( *p , ( *dc  ) * 14   );
	*p = q;
	
	
	for(i = 13 ; i >= 0 ;i-- ){
		mask = mask << i;
		mask = mask & num ;
		mask = mask >> i;
		*( *p + *counter) = mask;
		(*counter)++;
		mask = 1;
		
	}
	

}
