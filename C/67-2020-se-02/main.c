#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>

int main(const int argc, const char * argv[]){
	if(argc != 3){ //expected arguments SCL SDL name_of_new_file
		errx(1,"invalid input");
	}
	
	//open files-------------------------------------------------	
	int fd[3];
	for(int i = 0; i < 2; i++){
		if((fd[i] = open(argv[i+1],O_RDONLY)) == -1){
			err(2,"err opening file %s",argv[i+1]);
		}
	}
	if((fd[2] = open(argv[3],O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP)) == -1){
		err(2,"err opening file %s",argv[3]);
	}

	//size of files--------------------------------------------
	struct stat st[2]; // scl - sdl
	for(int i=0;i<2;i++){
		if(fstat(fd[i],&st[i]) == -1){
			err(3, "err geting stat of %s",argv[i+1]);
		}
	}

	if(st[0].st_size*8 != (st[1].st_size/22)){
		errx(5, "wrong format of files");
	}

	//write in file--------------------------------------------
	uint8_t bitMask[8]={
	0x80,//1000 0000
	0x40,
	0x20,
	0x10,
	0x08,
	0x04,//0000 0100 
	0x02,//0000 0010 
	0x01//0000 0001 
	};
	uint8_t bit;
	uint16_t c;
	int rd;
	while((rd=read(fd[0],&bit,sizeof(bit))) == sizeof(bit)){
		for(int i = 0; i < 8; i++){
			if(read(fd[1],&c,sizeof(c)) != sizeof(c)) err(4,"err reading %s", argv[1]); 
			
			if((bit & bitMask[i]) != 0){
				if(write(fd[2],&c,sizeof(c)) != sizeof(c)) err(6,"err writing %s", argv[3]);
			}
		}
	}
	if(rd != 0) err(4,"err reading %s", argv[2]);

	//close files -------------------------------------------
	for(int i =0; i< 3; i++){
		close(fd[i]);
	}
	exit(0);
}
