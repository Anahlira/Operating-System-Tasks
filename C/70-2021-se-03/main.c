#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>

struct header{
	uint32_t magic;
	uint32_t count;
}__attribute__((packed));

struct headerC{
	uint32_t magic1;
	uint16_t magic2;
	uint16_t reserved;
	uint64_t count;
}__attribute__((packed));

struct data{
	uint16_t type;
	uint16_t r1;
	uint16_t r2;
	uint16_t r3;
	uint32_t offset1;
	uint32_t offset2;
}__attribute__((packed));

int main(const int argc,const char * argv[]){
	if(argc != 3){
		errx(1,"invalid input");
	}
	
	//open files--------------------------------------------
	int fdi,fdo; 
	if((fdi = open(argv[1],O_RDONLY)) == -1){
		err(2,"err opening file");
	}
	if((fdo = open(argv[2],O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP)) == -1){
		err(2,"err opening file");
	}

	//write
	if(write(fdo, "const uint16_t arr[]={", 22) != 22) err(3,"err wrting");

	uint16_t buf;
	int rd;
	uint32_t size=0;
	while((rd=read(fdi,&buf,sizeof(buf))) == sizeof(buf)){
		size++;
		int i = 0;
		char nums[5];
		while(buf/10 > 0){
			num[i] = (char)(buf % 10);
			buf= buf/10;
			i++;
		}
		for(int j=i; j>=0 ; j--){
			if(write(fdo, nums[j], 1) != 1) err(3,"err wrting");
		}
		if(write(fdo, ",", 1) != 1) err(3,"err wrting");
	}
	if(rd != 0) err(4,"err reading");

	if(write(fdo, "};", 2) != 2) err(3,"err wrting");

	if(write(fdo, "uint32_t arrN = ", 17) != 17) err(3,"err wrting");
	//write  arrN--------------->
		int i = 0;
		char nums[8];
		while(size/10 > 0){
			num[i] = (char)(size % 10);
			buf= buf/10;
			i++;
		}
		for(int j=i; j>=0 ; j--){
			if(write(fdo, nums[j], 1) != 1) err(3,"err wrting");
		}
	//-------------------------->
	if(write(fdo, ";", 1) != 1) err(3,"err wrting");

	//close files------------------------------------------
	close(fd1);
	close(fd2);
	exit(0);
}

