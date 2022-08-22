#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

void cat(const int fd);

int main(const int argc, const char * argv[]){
	if(argc == 1){
		cat(0);
	}
	for(int i=1; i < argc;i++){
		if(strcmp(argv[i], "-") == 0){
			cat(0);
			continue;
		}
		int fd;
		if((fd = open(argv[i],O_RDONLY)) == -1){
			err(3,"opening file %s",argv[i]);
		}
		cat(fd);
		close(fd);
	}

	exit(0);
}

void cat(const int fd){
	char c;
	int rf;
	while((rf = read(fd,&c,sizeof(c))) == sizeof(c)){
		if(write(1,&c,sizeof(c)) != sizeof(c)){
			err(2, "err writing in stdout");
		}
	}
	if (rf != 0){
		err(1,"err reading");
	}
}

