#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdint.h>
#include <stdio.h>

struct element{
	uint8_t name[8];
	uint32_t offset;
	uint32_t length;
}__attribute__((packed));

int main(const int argc,const char * argv[]){
	if(argc != 2)
	   	errx(1,"wrong input");

	//open file --------------------------
	
	int fd;
	if((fd=open(argv[1],O_RDONLY)) == -1) err(1,"can't open file");
	
	struct stat st;
	if(fstat(fd,&st) == -1) err(2,"Can't stat");
	if(st.st_size % sizeof(struct element) != 0 || (st.st_size / sizeof(struct element)) - 8 > 0){
		errx(3,"wrong format");
	}

	//create pipe for talking------------
	int a[2];
	if(pipe(a) == -1) err(7,"ërr creating pipe");

	//read file elements -----------------
	
	int rd;
	struct element e;
	pid_t p;
	int k = 0;
	while((rd = read(fd,&e,sizeof(e))) == sizeof(e)){
		k++;
		if(e.name[7] != 0x00) errx(3,"wrong format");

		if((p=fork()) == -1) err(5,"err forking");
		if(p==0){
			rd = 0;
			break;
		}
	}
	if(rd != 0) err(4,"err while reading %s", argv[1]);
	
	if(p == 0){
		close(a[0]);
		int f;
		if((f=open((char *)e.name,O_RDONLY)) == -1) err(1,"can't open file");

		if(lseek(f,e.offset,SEEK_SET) == -1) err(6,"err with lseek");

		uint16_t r = 0;
		uint16_t buf;
		for(uint32_t i = 0; i < e.length; i++){
			if(read(f,&buf,sizeof(buf)) != sizeof(buf)) err(4,"err while reading in child");
			r = r ^ buf;
		}
		if(write(a[1], &r,sizeof(r)) != sizeof(r)) err(8,"err while writing - child to pipe");
		
		close(a[1]);
		close(f);	
		exit(0);
	}
	close(a[1]);
	
	//combine results------------------------
	
	uint16_t result = 0;
	uint16_t buf;
	for(int i = 0; i < k; i++){
		if(read(a[0],&buf,sizeof(buf)) != sizeof(buf)) err(4,"err while reading from pipe");
		result= result ^ buf;
	}

	printf("result: %X\n",result);

	close(a[0]);
	close(fd);
	exit(0);
}

//file
//uint16_t uint16_t uint16_t uint16_t
//uint16_t uint16_t uint16_t uint16_t
//
