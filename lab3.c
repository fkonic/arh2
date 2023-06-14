#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys\timeb.h>
#include <malloc.h>
#include <math.h>

/*	OS Microsoft Windows 10 Professional 64-bit
	Processor - Intel Core i5-5005U CPU @ 2.00 GHz
	L1	32 KB (I) + 32 KB (D) x2 	64 B	8-way/8-way associate
	L2	256 KB	x2					64 B	8-way associate 
	L3	3 MB						64 B	12-way associate	*/

#define S1 64 * (1 << 10)
#define B1 64
#define S2 256 * (1 << 10)
#define B2 64
#define S3 3 * (1 << 20)
#define B3 64

void Init(char *polje, int size)
{
	int i;
	for (i = 0; i < size; i++)
		*(polje + i) = 0;
}

void Sum(char *polje, int size) {
	unsigned long rez = 0;
	for (int i = 0; i < size; ++i) {
		rez += *(polje + i);
	}
	printf("%ld\n", rez);
}

double A()
{
	int i, j;
	double start, stop, duration, troughput;
	char *L1 = (char *)malloc(2 * S1 * sizeof(char));

	Init(L1, 2 * S1);

	start = clock();
	for (i = 0; i < 30000; i++)
		for (j = 0; j < 2 * S1; j++)
			L1[j]++;	
	stop = clock();
	
	Sum(L1, 2 * S1);
	free(L1);
	
	duration = (stop - start) / CLOCKS_PER_SEC;
	printf("A: %.2lf s\n", duration);
	printf("%f ns\n", duration/30000/(2*S1)*pow (10.0, 9.0));
	troughput = 30000 / duration * 2 * S1 / (1<<20); // * sizeof(char) = 1 B
	
	return troughput;
}

double B()
{
	int i, j;
	double start, stop, duration, throughput;
	char *L2 = (char *)malloc(2 * S1 * sizeof(char));

	Init(L2, 2 * S1);
	
	start = clock();
	for (i = 0; i < 200000; i++)
		for (j = 0; j < 2 * S1; j += B1)
			L2[j]++;
	stop = clock();
	
	Sum(L2, 2 * S1);
	free(L2);
	
	duration = (stop - start) / CLOCKS_PER_SEC;
	printf("B: %.2lf s\n", duration);
	printf("%f ns\n", duration/200000/(2*S1/B1) *pow (10.0, 9.0));
	throughput = 200000 / B1 / duration * 2 * S1 / (1<<20);
	
	return throughput;
}

double C()
{
	int i, j;
	double start, stop, duration, throughput;
	char *L3 = (char *)malloc(2 * S2 * sizeof(char));

	Init(L3, 2 * S2);

	start = clock();
	for (i = 0; i < 30000; i++)
		for (j = 0; j < 2 * S2; j += B2)
			L3[j]++;
	stop = clock();
	
	Sum(L3, 2 * S2);
	free(L3);
	
	duration = (stop - start) / CLOCKS_PER_SEC;
	printf("C: %.2lf s\n", duration);
	printf("%f ns\n", duration/30000/(2*S2/B2)*pow (10.0, 9.0));
	throughput = 30000 / B2 / duration * 2 * S2 / (1<<20);
	
	return throughput;
}

double D()
{
	int i, j;
	double start, stop, duration, throughput;
	char *RAM = (char *)malloc(2 * S3 * sizeof(char));

	Init(RAM, 2 * S3);

	start = clock();
	for (i = 0; i < 1000; i++)
		for (j = 0; j < 2 * S3; j += B3)
			RAM[j]++;
	stop = clock();

	Sum(RAM, 2 * S3);
	free(RAM);
	
	duration = (stop - start) / CLOCKS_PER_SEC;
	printf("D: %.2lf s\n", duration);
	printf("%f ns\n", duration/1000/(2*S3/B3)*pow (10.0, 9.0));
	throughput = 1000 / B3 / duration / (1 << 20) * S3 * 2;
	return throughput;
}


int main()
{
	double tL1, tL2, tL3, tRAM;
	tL1 = A();
	printf("L1 throughput: %f MB/s\n\n", tL1);

	tL2 = B();
	printf("L2 throughput: %f MB/s\n\n", tL2);
	
	tL3 = C();
	printf("L3 throughput: %f MB/s\n\n", tL3);

	tRAM = D();
	printf("RAM throughput: %f MB/s\n\n", tRAM);

	printf("L2/L1 = %f\n", tL1 / tL2);
	printf("L3/L2 = %f\n", tL2 / tL3);
	printf("RAM/L3 = %f\n", tL3/ tRAM);

	return 0;
}