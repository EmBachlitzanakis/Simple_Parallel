/* Serial program for matrix-matrix product.
 *
 * VVD
 */

#include <stdio.h>
#include <stdlib.h>
# include <omp.h>


#define N 1024
double omp_get_wtime(void);
int A[N][N], B[N][N], C[N][N];
int readmat(char *fname, int *mat, int n), 
    writemat(char *fname, int *mat, int n);

int main()
{
	int i, j, k, sum;
	FILE *f;
	/* Read matrices from files: "A_file", "B_file" 
	 */
	if (readmat("Amat1024.txt", (int *) A, N) < 0) 
		exit( 1 + printf("paok\n") );
	if (readmat("Bmat1024.txt", (int *) B, N) < 0) 
		exit( 1 + printf("file problem\n") );
	double start; 
	double end; 
	
	
 	for (int fores=0;fores<10 ;fores++){
	start = omp_get_wtime();
	
       // #pragma omp parallel for firstprivate (j,k) reduction (+: sum ) //num_threads (4) //schedule(guided,10)
	for (i = 0; i < N; i++){
		#pragma omp parallel for firstprivate (k) reduction (+: sum )
		for (j = 0; j < N; j++)
		{	
			sum = 0;
			//#pragma omp parallel for reduction (+: sum )
			for (k = 0; k < N; k++)
				sum += A[i][k]*B[k][j];
			C[i][j] = sum;
			
		};
	}
	end = omp_get_wtime();
	double chronos= end - start;
	printf("Work took %f seconds\n", chronos);
	if ((f = fopen("Chronos.txt", "a")) == NULL) /* grafoume se ena txt tous chronous gia na dimiourgisoume tis grafikes parastasis */
		return (-1);
	fprintf(f, "%lf \n ",chronos);
	fclose(f);
	}
	writemat("Cmat1024.txt", (int *) C, N);

	return (0);
}


/* Utilities to read & write matrices from/to files
 * VVD
 */

#define _mat(i,j) (mat[(i)*n + (j)])


int readmat(char *fname, int *mat, int n)
{
	FILE *fp;
	int  i, j;
	
	if ((fp = fopen(fname, "r")) == NULL)
		return (-1);
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (fscanf(fp, "%d", &_mat(i,j)) == EOF)
			{
				fclose(fp);
				return (-1); 
			};
	fclose(fp);
	return (0);
}


int writemat(char *fname, int *mat, int n)
{
	FILE *fp;
	int  i, j;
	
	if ((fp = fopen(fname, "w")) == NULL)
		return (-1);
	for (i = 0; i < n; i++, fprintf(fp, "\n"))
		for (j = 0; j < n; j++)
			fprintf(fp, " %d", _mat(i, j));
	fclose(fp);
	return (0);
}
