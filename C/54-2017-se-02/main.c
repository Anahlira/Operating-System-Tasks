#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct base{
	int sequence;
	int newLine;
	int num;
}__attribute__((packed));

void print(const int fd, struct base *b);

void print(const int fd, struct base *b){
	char c;
	int ri;
	while((ri = read(fd,&c,1))){
		if(b->sequence && b->newLine){
			printf("%d ",b->num);
			setbuf(stdout,NULL);
			b->newLine=0;
		}
		if(write(1,&c,1) != 1){
			err(1,"err in writing");
		}
		if(c == '\n'){
			b->newLine=1;
			b->num++;
		}
	}
	if(ri != 0){
		err(2,"err in reading");
	}
}

int main(const int argc, const char * argv[]){
	struct base b = {0,1,1};
	int k=1;
	if(argc > 1 && strcmp(argv[1],"-n") == 0){
		b.sequence=1;
		k=2;
	}

	if(argc == 1 || (argc == 2 && b.sequence)){
		print(0,&b);
		exit(0);
	}

	for(int i = k; i < argc;i++){
		if(strcmp(argv[i],"-") == 0){
			print(0,&b);
			continue;
		}

		int fd = open(argv[i],O_RDONLY);
		if(fd == -1){
			err(3,"can't open file");
		}
		print(fd,&b);
		close(fd);
	}


	exit(0);
}
