#include <stdio.h>

int main(int argc, char *argv[])
{
	//32 bits unsigned data type
	unsigned long page,address,offset;
	//String to unsigned long
	address= atoll(argv[1]);
	//address/4KB
	page = address >> 12; 
	//offset is the remainder of address/4KB
	offset = address & 0xfff;

	printf("The address %lu contains: \n", address);
	printf("page number = %lu\n",page);
	printf("offset = %lu\n", offset);

	return 0;
}
