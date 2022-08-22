#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

#include <stdio.h>

void wr(const char a);

int main(const int argc, const char * argv[]){
	if( ! (argc == 3 || argc == 5))	{
		errx(1,"wrong number arguments");
	}
	
	size_t i=0;
	int k=2;
	int field=0;
	char r=' ';
	if(strcmp(argv[1],"-d") == 0){
		r=argv[2][0];
		field = 1;
		k=4;
		i=1;
		if(strcmp(argv[3],"-f") != 0 || argc != 5){
			errx(1,"invalid input");
		}
	}
	else if (strcmp(argv[1],"-c") != 0){
		errx(1,"invalid input");
	}

	//get lenght
	size_t N,M;
	if(strlen(argv[k]) == 1){
		M=atoi(&argv[k][0]);
		if(M == 0) errx(1,"invalid input");
		N=0;
	}
	else if (strlen(argv[k]) == 3){
		N=atoi(&argv[k][0]);
		M=atoi(&argv[k][2]);
		if(M == 0 || N == 0 || N > M) errx(1,"invalid input");
	}
	else errx(1,"invalid input");

	int rf;
	char buf;
	while((rf = read(0,&buf,sizeof(buf))) == sizeof(buf)){
		if(field == 1 && buf == r) {
			i++;
			continue;
		}
		if(field == 0) i++;
		if(i > M){
			exit(0);
		}

		if(N==0){
			if(M == i){
				wr(buf);
			}
		}
		else{
			if( i >= N){
				wr(buf);
			}
		}
	}
	if(rf != 0){
		err(3,"err reading");
	}

	exit(0);
}

void wr(const char a){
	if(write(1,&a,sizeof(a)) != sizeof(a)){
		err(2,"err writing");
	}
}

