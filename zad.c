// zad.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#define MAX 20

//void matmul(char *a, char *b, char *r, m, n, p, q) {

//}
void matmul(char *a, char *b, char *r, int m, int n, int p, int q) {
	int i, j, k, sum = 0;
	for (i = 0; i < m; i++) {
		for (j = 0; j < q; j++) {
			//*(r + i*m + j) = 0;
			for (k = 0; k < p; k++) {
				sum += a[i*m + k] * b[k*p + j];
			}
			r[i*m + j] = sum;
			sum = 0;
		}
	}
}
int main()
{
	int m, n, p, q;
	int i, j;
	scanf("%d %d", &m, &n);
	scanf("%d %d", &p, &q);
	
	char *a = (char *)malloc(m * n * sizeof(char));
	char *b = (char *)malloc(p * q * sizeof(char));
	char *r = (char *)malloc(m * q * sizeof(char));

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			scanf("%d", &a[i*m + j]);
		}
	}

	for (i = 0; i < p; i++) {
		for (j = 0; j < q; j++) {
			scanf("%d", &b[i*p + j]);
		}
	}
	printf("Matrica A:\n");
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			printf("%d\t", a[i*m+j]);
		}
		printf("\n");
	}
	printf("Matrica B:\n");
	for (i = 0; i < p; i++) {
		for (j = 0; j < q; j++) {
			printf("%d\t", *(b + i*p+j));
		}
		printf("\n");
	}
	matmul(a, b, r, m, n, p, q);
	printf("umnozak:\n");
	for (i = 0; i < m; i++) {
		for (j = 0; j < q; j++) {
			printf("%d\t", *(r + i*m + j));
		}
		printf("\n");
	}
	free(a);
	free(b);
	free(r);
}