#include "firstpass.h"
#include "funcs.h"


void firstpass( FILE * fd1 , int* ic , int* dc , char** inst , char** data , ptr2* hptr ){

	int numline = 0 , curr , flag , names , counter = 0 , type , i , num2 ,  oper1 , oper2 , instnum , add , param1 , param2;
	char line[MAX_LINE];
	char label[MAX_LABEL];
	char word[MAX_LINE];
	char num[MAX_NUM];
	fpos_t pos;
	/*ptr2 hptr = NULL;*/
	char* q = NULL;
	
	buildarray( inst , data );
	
	rewind( fd1 );
	fgetpos( fd1 , &pos );

	while( !feof( fd1 ) ){
	
		curr = 0;
		add = 0;
		param1 = 0;
		param2 = 0;
		oper1 = 0 ;
		oper2 = 0;	
		
		fsetpos( fd1 , &pos );
		fgets( line , MAX_LINE , fd1 );
		numline++;
		getword( line , label , &curr );
		
		if( !strlen( label ) || label[0] == ';'){
		
			fgetpos( fd1 , &pos );
			fgetc( fd1 );
			continue;
			
		}
			
		flag = islabel( label );
		
		if( flag == 2 ){
		
			fprintf( stderr , "illegal label at line %d \n\n" , numline );
			fgetpos( fd1 , &pos );
			fgetc( fd1 );
			continue;
			
		}
		
		if( flag == 1 ){
		
			names = findlabel( *hptr , label );
			
			if( !names ){
			
				fprintf( stderr , "2 labels with the same name at line %d \n\n" , numline );
				fgetpos( fd1 , &pos );
				fgetc( fd1 );
				continue;
			
			}
			
			addtolabellist( /*&*/hptr , label );
			
			getword( line , word , &curr );
			
			type = instordata( word );
			
			if( type ){
			
				(*hptr) -> line = *ic + 1 ;
				(*hptr) -> data = 0 ;
			}
			
			else{
			
				(*hptr) -> line = *dc + 1 ;
				(*hptr) -> data = 1 ;
			}
			
		
		}
		
		if ( !flag ){
		
			strcpy( word , label );
			
		}	
		
		type = instordata( word );
		
		if( type ){
		
			fprintf( stderr , "\n\n"  );
			instnum = checkinst( word /*, &inst */);
			(*ic)++;
			oper1 = methodparam( line , &curr , &param1 );
			fprintf( stdout , "\nmethod param1	%d\n\n" , oper1 ); 
			
			/*if( oper1 != -1 )
				add++;*/
				
			q = ( char * ) realloc ( *inst , ( *ic  ) * 14   );
			if( !q ){
				printf( "allocated failed\n\n" );
				exit(0);
			}
			*inst = q ;
			
			if( oper1 != 2 && oper1 != -1 ){
			
				oper2 = methodparam( line , &curr , &param1 );
				
				fprintf( stdout , "method param2	%d\n\n" , oper2 ); 
				
				if( oper2 != -1 && ( oper1 != 3 || oper2 != 3)){
					add += 2;

				}
				
				else if( (oper1 == 3 && oper2 == 3) ||  oper2 == -1 )
					add++;
					
					
			}
			
			else if( oper1 == 2){
			
				oper2 = oper1;
				oper1 = 0;
				
				
				if( line[curr - 3] == 'r' && line[curr - 2] >= 48 && line[curr - 2] <= 55 && line[curr]  == 'r' && line[curr +1] >= 48 && line[curr +1] <= 55 && line[curr+2] == ')' ){
				
				param2 = 3;
				add += 2;
				
				}
				
				else{
					if( line[curr]  == '#')
						param2 = 0;
					else if( line[curr]  == 'r' && line[curr +1] >= 48 && line[curr +1] <= 55 && line[curr+2] == ')'){
				
						param2 = 3;
					}
					else
						param2 = 1;	
				
			
					add += 3 ;
				}
			
				
			}
			
			
			if( oper2 == -1){
			
				oper2 = oper1;
				oper1 = 0;
				
			}
				
			fprintf(stdout , "%d\t%d\t%d\t%d\t%d\t\n\n" , param1 , param2 , instnum , oper1 , oper2 );
			putinstruction( inst , /*inst*/instnum , oper1 , oper2 , ic , param1 , param2 );
			*ic += add ;
			
			q = ( char * ) realloc ( *inst , ( *ic  ) * 14   );
			if( !q ){
				printf( "allocated failed\n\n" );
				exit(0);
			}
			*inst = q ;
			
			for( i = 0 ; i < add*14 ; i++){
			
			*( *inst + ( *ic - add ) *14 + i )= 0;
			
			}
			
		}
		
		
		else{
		
			if( !strcmp( word , ".data") ){
			
				while( line[curr] != '\n'){
					getnum( line , num , &curr );
			
					num2 = atoi (num);
									
					(*dc)++;
					insertnum( num2 , data , &counter , dc );
					getcomma( line , &curr );
				}	
				
			}
			
			else if(!strcmp( word , ".string")){
			
				getstr( line , &curr , &counter , data ,dc);
		
			} 
			
			else if( !strcmp( word , ".extern") ){
			
				getword( line , word , &curr );
				names = findlabel(  *hptr , word );
				if( !names ){
			
					fprintf( stderr , "extern label defined in the file at line %d \n\n" , numline );
					fgetpos( fd1 , &pos );
					fgetc( fd1 );
					continue;
			
				}
				
				addtolabellist( /*&*/hptr , word );
				(*hptr) -> ext = 1;
				(*hptr) -> counter = 0;
					
			
			
			}
		
		}
		
		
		fgetpos( fd1 , &pos );
		fgetc( fd1 );
		
	}
	
	for(i = 0 ; i < (( *ic ) * 14 ) ; i++){
		fprintf( stderr , " %d " ,  *( *inst + i )  );
	}		
	for(i = 0 ; i < (( *dc ) * 14 ) ; i++){
		fprintf( stderr , " %d " ,  *( *data + i )  );
	}		
	fprintf( stderr , "\n\n ic == %d \n\n dc == %d\n\n" , *ic , *dc );
	/*freelabellist( &hptr );*/

}

void buildarray( char** inst , char** data){
 
	*data = ( char * ) malloc ( sizeof ( char ) );
	if( !(*data) ){
		printf( "allocated failed\n\n" );
		exit(0);
	}
	*inst = ( char * ) malloc ( sizeof ( char ) );
	if( !(*inst) ){
		printf( "allocated failed\n\n" );
		exit(0);
	} 
 
 
}


int islabel( char label[] ){

	int i ;
	int length = strlen( label );
	
	if( label[ length - 1 ] == ':' ) {
	
		if( isalpha( label[0] ) ){
	
			for( i = 1 ; i < length-1 ; i++){
		
				if( !isalnum( label[i] ) )
		  			return 2;
		
			}
			label[ length -1 ] = '\0';
			return 1;	
		
		}
		
		return 2;
		
	}
	
	return 0;
	
}

void addtolabellist( ptr2* hptr, char nameoflabel[] ){


	ptr2 t , p ;
	t = ( ptr2 ) malloc ( sizeof ( label ) );
	
	if( !t ){
		printf( "allocated failed\n\n" );
		exit(0);
	}
	
	strcpy( t -> name , nameoflabel );
	t -> ext = 0;
	t -> ent = 0;
	p = *hptr;
	*hptr = t ;
	t->next = p ; 

}

void freelabellist( ptr2* hptr ){

	ptr2 p;
	
	while( *hptr ){
	
		p = *hptr;
		*hptr =  (*hptr) -> next ;
		free( p );
		
	}

}



int findlabel( ptr2 hptr , char name1[] ){

	while( hptr ){ 
		if( !strcmp (( (hptr) -> name) , name1 ) )
			return 0; 
		hptr = ( (hptr) -> next);
	}
	return 1;

}

int instordata( char word[] ){

	if( word[0] == '.' )
		return 0;
	return 1;

}

int checkinst( char word[] /*, int * inst */){

	int  i  ;
	char* instructions[] = { "mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};
	
	for( i = 0 ; i < 16 ; i++ ){
		if( !strcmp ( word , instructions[i]) )
			/**inst = i ;*/	
			return i;
	} 
	/*fprintf( stdout , "instruction number	%d\n" , *inst);*/
	return -1;
	

}


int methodparam( char line[] , int* curr , int * param1 ){

	int i;
	char word[MAX_LINE];
	getword( line , word , curr );
	
	if( word[0] == '#')
		return 0;
		
	if( word [0] == 'r'  && word[1] >= 48 && word[1] <= 55  && word[2] == '\0')
		return 3;
			
	if( isalpha ( word[0] ) ){
	
		for( i = 0 ; word[i] != '\0' ; i++ ){
		
			if( word [i] == '('){
				if( word [i +1 ] == '#' ){
					*param1 = 0 ;
				}	
				else if( word [i +1 ] == 'r'  && word [i +2 ] >= 48 && word[i+2] <= 55 &&  word[i+3] == '\0'){
				
					*param1 = 3;
				}
				else
					*param1 = 1;		
			
			return 2;
			}
			
		} 
		return 1;
	
	
	}
	
	return -1;


}


void putinstruction( char ** in , int inst , int oper1 , int oper2 ,int* ic , int param1 , int param2   ){


	if( !param1 || param1 == 1 || param1 == -1)
		*( *in + ( *ic - 1 )*14  ) = 0 ;
	else
		*( *in + ( *ic - 1 )*14  ) = 1;
	if( !param1 || param1 == 2 || param1 == -1)	
		*( *in + ( *ic - 1 )*14 +1  ) = 0; 
	else
		*( *in + ( *ic - 1 )*14 +1 ) = 1;
	if( !param2 || param2 == 1 || param2 == -1)
		*( *in + ( *ic - 1 )*14 +2 ) = 0 ;
	else
		*( *in + ( *ic - 1 )*14 +2 ) = 1;
	if( !param2 || param2 == 2 || param2 == -1)	
		*( *in + ( *ic - 1 )*14 +3  ) = 0; 
	else
		*( *in + ( *ic - 1 )*14 +3 ) = 1;	
	if(inst >=8)
		*( *in + ( *ic - 1 )*14 +4 ) = 1;		
	else		
		*( *in + ( *ic - 1 )*14 +4 ) = 0 ;
	if( (inst >= 4 && inst < 8) || inst >= 12)
		*( *in + ( *ic - 1 )*14 +5 ) = 1 ;	
	else
		*( *in + ( *ic - 1 )*14 +5 ) = 0 ;
	if(inst ==2 || inst ==3 || inst ==6 || inst ==7 || inst ==10 || inst ==11 || inst ==14 || inst ==15 )	
		*( *in + ( *ic - 1 )*14 +6 ) = 1 ;
	else
		*( *in + ( *ic - 1 )*14 +6 ) = 0 ;
	if( inst % 2 )
		*( *in + ( *ic - 1 )*14 +7 ) = 1 ;
	else
		*( *in + ( *ic - 1 )*14 +7 ) = 0 ;
	if( !oper1 || oper1 == 1 || oper1 == -1)
		*( *in + ( *ic - 1 )*14 +8 ) = 0 ;
	else
		*( *in + ( *ic - 1 )*14 +8 ) = 1;
	if( !oper1 || oper1 == 2 || oper1 == -1)	
		*( *in + ( *ic - 1 )*14 +9  ) = 0; 
	else
		*( *in + ( *ic - 1 )*14 +9 ) = 1;	
	if( !oper2 || oper2 == 1 || oper2 == -1)
		*( *in + ( *ic - 1 )*14 +10 ) = 0 ;
	else
		*( *in + ( *ic - 1 )*14 +10 ) = 1;
	if( !oper2 || oper2 == 2 || oper2 == -1)	
		*( *in + ( *ic - 1 )*14 +11  ) = 0; 
	else
		*( *in + ( *ic - 1 )*14 +11 ) = 1;
	*( *in + ( *ic - 1 )*14 +12 ) = 0 ;
	*( *in + ( *ic - 1 )*14 +13 ) = 0 ;
	
	
	
								
}

void printlist( ptr2 hptr ){

	while( hptr ){
	
		printf("%s\n" , hptr -> name);
		printf("%d\n" , hptr -> ext);
		printf("%d\n" , hptr -> ent);
		printf("%d\n" , hptr -> line);
		printf("%d\n" , hptr -> data);
		printf("%d\n" , hptr -> counter);
		puts("\n\n");
		hptr = hptr -> next;
	
	}
}
