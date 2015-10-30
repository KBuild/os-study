#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define BYTESOFSECTOR 512
#define O_BINARY 0x00

int adjust_sector_size(int fd, int source_size);
void write_kernel_info(int tgtfd, int kernel_sector_count);
int copy_file(int srcfd, int tgtfd);

int main(int argc, char* argv[])
{
	int srcfd;//source file descriptor
	int tgtfd;//target file descriptor

	int bootloader_size;
	int kernel32_sector_count;
	int source_size;

	if (argc < 3)
	{
		fprintf(stderr,
			"[ERROR] imagemaker bootloader.bin kernel32.bin\n");
		exit(-1);
	}

	if( (tgtfd = open("disk.img", O_RDWR | O_CREAT | O_TRUNC | O_BINARY,
				S_IREAD | S_IWRITE)) == -1 )
	{
		fprintf(stderr,
			"[ERROR] %s open fail\n", argv[1]);
		exit(-1);
	}

	printf("[INFO] Copy boot loader to image file\n");
	if( (srcfd = open(argv[1], O_RDONLY | O_BINARY)) == -1 )
	{
		fprintf(stderr, "[ERROR] %s open fail\n", argv[1]);
		exit(-1);
	}

	source_size = copy_file(srcfd, tgtfd);
	close(srcfd);

	bootloader_size = adjust_sector_size(tgtfd, source_size);
	printf("[INFO] %s size = [%d] and sector count = [%d]\n",
			argv[1], source_size, bootloader_size);

	printf("[INFO] Copy protected mode kernel to image file\n");

	if( (srcfd = open(argv[2], O_RDONLY | O_BINARY)) == -1 )
	{
		fprintf(stderr,
			"[ERROR] %s open fail\n", argv[2]);
		exit(-1);
	}

	source_size = copy_file(srcfd, tgtfd);
	close(srcfd);

	kernel32_sector_count = adjust_sector_size(tgtfd, source_size);
	printf("[INFO] %s size = [%d] and sector count = [%d]\n",
			argv[2], source_size, kernel32_sector_count);

	printf("[INFO] Start to write kernel infomation\n");
	write_kernel_info(tgtfd, kernel32_sector_count);

	printf("[INFO] Image file create complete\n");

	close(tgtfd);
	return 0;
}

int adjust_sector_size(int fd, int source_size)
{
	int i;
	int adjust_size_sector;
	char ch;

	adjust_size_sector = source_size % BYTESOFSECTOR;
	ch = 0x00;

	if(adjust_size_sector)
	{
		adjust_size_sector = 512 - adjust_size_sector;
		printf("[INFO] File size [%lu] and fill [%u] byte\n",
				source_size, adjust_size_sector);
		for(i = 0 ; i < adjust_size_sector ; i++)
		{
			write(fd, &ch, 1);
		}
	}
	else
	{
		printf("[INFO] File size is aligned 512 byte\n");
	}

	return (source_size + adjust_size_sector) / BYTESOFSECTOR;
}

void write_kernel_info(int tgtfd, int kernel_sector_count)
{
	unsigned short data;
	long position;

	position = lseek(tgtfd, 5, SEEK_SET);
	if(position == -1)
	{
		fprintf(stderr,
			"lseek fail. Return value = %d, errno = %d, %d\n",
			position, errno, SEEK_SET);
		exit(-1);
	}

	data = (unsigned short)kernel_sector_count;
	write(tgtfd, &data, 2);

	printf("[INFO] Total sector count except boot loader [%d]\n",
			kernel_sector_count);
}

int copy_file(int srcfd, int tgtfd)
{
	int source_file_size;
	int readlen;
	int writelen;
	char buffer[BYTESOFSECTOR];

	source_file_size = 0;
	while(1)
	{
		readlen = read(srcfd, buffer, sizeof(buffer));
		writelen = write(tgtfd, buffer, readlen);

		if(readlen != writelen)
		{
			fprintf(stderr, "[ERROR] read != write (size)\n");
			exit(-1);
		}

		source_file_size += readlen;

		if(readlen != sizeof(buffer))
		{
			break;
		}
	}

	return source_file_size;
}
