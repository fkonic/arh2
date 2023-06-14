// VectorApp.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <xmmintrin.h>

using namespace std;

void _declspec(naked) sum_asm(float const* A, float const* B, int count, float *R) {
	_asm {
		/* cdecl prolog: */
		push ebp
		mov ebp, esp 

		push ebx /* volatilni registri */
		push edi

		mov eax, [ebp + 8] /* A */
		mov ebx, [ebp + 12] /* B */
		xor ecx, ecx /* i = 0 */
		mov edx, [ebp + 16] /* n */
		mov edi, [ebp + 20] /* R */

PETLJA:	fld DWORD PTR[eax + ecx * 4] /* st = a[i] */
		fld DWORD PTR[ebx + ecx * 4] /* st = b[i], st(1) = a[i] */
		faddp st(1), st(0) /* st = a[i]+b[i], pop st(1) */
		fstp DWORD PTR[edi + ecx * 4] /* R[i] = st, pop st */

		inc ecx /* i = i + 1 */
		cmp ecx, edx /* i < n ?*/
		jne PETLJA 

		pop edi /* obnova registara */
		pop ebx
		
		/* cdecl epilog: */
		pop ebp /* umjesto ’add esp,4, pop ebp’ moze biti ’leave’*/
		ret /* povratak iz potprograma */
	}
}

void sum_c(float const* A, float const* B, int count, float *R) {
	for (int i = 0; i < count; i++)
		*(R + i) = *(A + i) + *(B + i);
	return;
}

void _declspec(naked) sum_SSE(float const* A, float const* B, int count, float *R) {
	_asm {
		/* cdecl prolog: */
		push ebp
		mov ebp, esp

		push ebx /* volatilni registri */
		push edi
		push esi

		mov eax, [ebp + 8] /* A */
		test eax, 0fh /* poravnat? */
		jnz KRAJ
		mov ebx, [ebp + 12] /* B */
		test ebx, 0fh /* poravnat? */
		jnz KRAJ
		xor ecx, ecx /* i = 0 */
		mov edx, [ebp + 16] /* n */
		mov edi, [ebp + 20] /* R */
		test edi, 0fh /* poravnat? */
		jnz KRAJ

VEC:	/*cmp edx, 4
		jl ITER /* n < 4, ne moze SSE */

		movaps xmm0, [eax + ecx * 4] /* xmm0 = a[i:i+4] */
		movaps xmm1, [ebx + ecx * 4] /* xmm1 = b[i:i+4] */
		addps xmm0, xmm1 /* xmm0 = xmm0 + xmm1 */
		movaps[edi + ecx * 4], xmm0 /* R[i:i+4] = xmm0 */

		add ecx, 4 /* i += 4 */
		sub edx, 4 
		cmp edx, 0 /* n >= 4 ? */
		jge VEC

		test edx, 03 /* edx % 4 == 0? */
		jz KRAJ

//ITER:	fld DWORD PTR[eax + ecx * 4] /* st = a[i] */
//		fld DWORD PTR[ebx + ecx * 4] /* st = b[i], st(1) = a[i] */
//		faddp st(1), st(0) /* st = a[i]*b[i], pop st(1) */
//		fstp DWORD PTR[edi + ecx * 4] /* R[i] = st, pop st */
//
//		inc ecx /* i++ */
//		dec edx 
//		test edx, edx /* n = 0 ? */
//		jne ITER
			
KRAJ:	pop esi
		pop edi /* obnova registara */
		pop ebx

		/* cdecl epilog: */
		pop ebp 
		ret
	}
}

void sum_SSE_c(float const* A, float const* B, int count, float *R) {
	for (int i = 0; i < count; i += sizeof(float)) {
		if (i + 4 <= count) {
			__m128 x = _mm_load_ps((A + i));
			__m128 y = _mm_load_ps((B + i));
			__m128 z = _mm_add_ps(x, y);
			_mm_store_ps(R + i, z);
		}
		else {
			for (; i % 4 < count % sizeof(float); i++) {
				*(R + i) = *(A + i) + *(B + i);
			}
		}
	}
}

int main()
{
	int n = 30;
	__declspec(align(16)) float a[30] = { 7.57, 4.36, 9.0, 7.11, 6.69, 3.52, 2.09, 6.96, 4.18, 1.47, 0.21, 1.61, 7.8, 0.22, 1.54, 2.29, 0.54, 8.8, 0.66, 3.29, 3.8, 3.46, 1.69, 5.32, 6.26, 0.23, 6.8, 1.55, 4.82, 9.86 };
	__declspec(align(16)) float b[30] = { 7.96, -4.15, -3.09, -6.47, 2.05, -8.97, -6.51, -7.95, 7.9, -3.96, -0.24, 7.05, -8.11, -5.43, -9.21, 3.9, 4.47, -6.92, 8.57, -2.46, 7.59, -8.7, 7.33, -5.07, -7.52, 3.23, -2.76, 9.95, -2.78, 9.24 };
	float r[30];
	float c[30];
	__declspec(align(16)) float s[30];

	sum_asm(a, b, n, r);
	std::cout << "ASM: ";
	for (int i = 0; i < n; i++)
		cout << r[i] << " ";
	cout << endl;

	sum_c(a, b, n, c);
	cout << "C: ";
	for (int i = 0; i < n; i++)
		cout << r[i] << " ";
	cout << endl;

	sum_SSE(a, b, n, s);
	cout << "SSE: ";
	for (int i = 0; i < n; i++)
		cout << s[i] << " ";
	cout << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
