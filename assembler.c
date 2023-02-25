#include "preprocess.h"
#include "secondpass.h"

int main( int argc , char* argv[] ){

	FILE *fd = NULL , *fd1 = NULL /*, *fd2 = NULL , *fd3 = NULL , *fd4 = NULL*/ ;
	int i , ic = 0, dc = 0 ;
	char *inst  , *data ;
	ptr2 hptr = NULL;

	for( i = 1 ; i < argc ; i++ ){

		preprocess( &fd , &fd1 ,/* "b" */ argv[i] );

		firstpass( fd1  , &ic , &dc , &inst , &data , &hptr );
		
		secondpass( fd1 , &ic , &dc , &inst , &data , &hptr , /*"b"*/  argv[i] );
		/*printlist(hptr);*/
		
		freelabellist( &hptr );
		free( inst );
		free( data );
		fclose( fd );
		fclose( fd1 );
		/*fclose( fd2 );
		fclose( fd3 );
		fclose( fd4 );*/

	}
	
	return 0;
	
}
