#include <stdlib.h>
#include <unistd.h>
#include <err.h>
//open
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#include <stdint.h>

struct data{
	uint16_t offset;
	uint8_t original;
	uint8_t new;
}__attribute__((packed));

int main(const int argc, const char * argv[]){
	if(argc != 4){
		errx(2, "wrong input");
	}
	int fd1,fd2,fdp;
	if((fd1=open(argv[2],O_RDONLY)) == -1){
		err(1,"err reading %s",argv[2]);
	}
	if((fdp=open(argv[1],O_RDONLY)) == -1){
		err(1,"err reading %s",argv[1]);
	}
	if((fd2=open(argv[3],O_CREAT| O_TRUNC | O_WRONLY, S_IRUSR |S_IWUSR | S_IRGRP)) == -1){
		err(1,"err reading %s",argv[3]);
	}
	
	int rf;
	uint8_t c;
	//coping file1 to file2
	while((rf=read(fd1,&c,sizeof(c))) == sizeof(c)){
		if(write(fd2,&c,sizeof(c)) != sizeof(c)){
			err(4,"writing in %s",argv[3]);
		}
	}
	if(rf != 0){
		err(3,"err reading %s", argv[2]);
	}
	
	//start changing file2
	struct data d;
	while((rf=read(fdp,&d,sizeof(d))) == sizeof(d)){
		if(lseek(fd1,d.offset,SEEK_SET) == -1){
			err(4,"err changing location in %s", argv[3]);
		}	
		if(read(fd1,&c,sizeof(c)) != sizeof(c)){
			err(3,"err reading %s", argv[3]);
		}
		if(c != d.original){
			errx(5, "err in files contex");
		}
		if(lseek(fd1,d.offset,SEEK_SET) == -1){
			err(4,"err changing location in %s", argv[3]);
		}	
		if(write(fd1,&d.new,sizeof(d.new)) != sizeof(d.new)){
			err(4,"writing in %s",argv[3]);
		}
	}
	if(rf != 0){
		err(3,"err reading %s", argv[1]);
	}

	close(fd1);
	close(fd2);
	close(fdp);
	exit(0);
}
