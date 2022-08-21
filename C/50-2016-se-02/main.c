#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

struct pair{
	uint32_t x;
	uint32_t y;
}__attribute__((packed));

int main(void){
	int fd1,fd2,fd3;
//f1--------------------------------------
	fd1=open("f1",O_RDONLY);
	if(fd1 == -1){
		err(1,"err opening f1");
	}
	
	struct stat statfd;
	if(fstat(fd1,&statfd) == -1){
		err(2, "err stat f1");
	}
	if(statfd.st_size % sizeof(struct pair) != 0){
		errx(3,"not right size of f1");
	}
//f2--------------------------------------
	fd2=open("f2", O_RDONLY);
	if(fd2 == -1){
                err(1,"err opening f2");
        }

	if(fstat(fd2,&statfd) == -1){
                err(2, "err stat f2");
        }
        if(statfd.st_size % sizeof(uint32_t) != 0){
                errx(3,"not right size of f2");
        }
//f3-------------------------------------
	fd3=open("f3", O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP);
        if(fd2 == -1){
                err(1,"err opening f3");
        }

	//read f1---------------------------
	
	struct pair p;
	int readinfo;
	uint32_t buff;
	while((readinfo = read(fd1,&p,sizeof(p))) == sizeof(p)){
		if(lseek(fd2,p.x,SEEK_SET) == -1){
			err(5, "changing fd2 offset");
		}
		for(uint32_t i=0; i<p.y;i++){
			if(read(fd2,&buff,sizeof(buff)) != sizeof(buff)){
				err(6,"err in reading f2");
			}
			if(write(fd3,&buff,sizeof(buff)) != sizeof(buff)){
				err(7,"err writing in f3");
			}
		}
	}
	if(readinfo != 0){
		err(4,"err reading f1");
	}

	close(fd3);
	close(fd2);
	close(fd1);

	exit(0);
}
