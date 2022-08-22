#include <stdlib.h>
#include <unistd.h>
#include <err.h>
//open
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#include <string.h>

void printLetter(const char a);

void printLetter(const char a){
	if(write(1,&a,sizeof(a)) != sizeof(a)){
		err(4,"err while writing");
	}
}

int main(const int argc,const char * argv[]){
	if (argc < 2){
		errx(1,"not enought arguments");
	}
	
	int k=0;

	if(strcmp(argv[1],"-d") == 0) k=1; // -d
	else if(strcmp(argv[1],"-s") == 0) k=2; //-s
	else{ 
		if(strlen(argv[1]) != strlen(argv[2])){
			errx(2,"strings must be the same size");
		}
		k=3;
	}
		int rf;
		int flag = 0;
		char buf;
		char new;
		char old='a';
		while((rf = read(0,&buf,sizeof(buf))) == sizeof(buf)){
			if(k==1 || k ==3) flag = 1;
			if(k==2 && flag == 1 && old == buf) continue;
			if(k==2) {
				flag=0;
				printLetter(buf);
			}

			for(size_t i = 0; i < strlen(argv[2]); i++){
				new=argv[2][i];
				if(k==1 && buf == new){
					flag = 0;
					break;
				}

				if(k==2){ 
				   if(buf == new){
				   	old=buf;
					flag=1;
				   }
				}

				if(k==3){
					old=argv[1][i];
					if(old==buf){
						printLetter(new);
						flag=0;
						break;
					}
				}
			}	

			if((k == 1 || k==3) && flag == 1){
				printLetter(buf);
			}
		}
		if(rf != 0){
			err(3,"err while reading");
		}
	exit(0);
}
