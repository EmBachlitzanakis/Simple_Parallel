/* Serial program for matrix-matrix product.
 *
 * VVD
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define N 256
int A[N][N], B[N][N],res[N],cc[N][N];
int C[N][N];
int readmat(char *fname, int *mat, int n), 
    writemat(char *fname, int *mat, int n);

int main(int argc, char *argv[])
{	
	double start;
	int i, j, k, sum,temp;
	int myid,nproc;
	MPI_Status status;
	int count = N*N;
	int work=N/nproc;
  	MPI_Init(&argc, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  	MPI_Comm_size(MPI_COMM_WORLD, &nproc);

	if (readmat("Amat256.txt", (int *) A, N) < 0) 
		exit( 1 + printf("file problem\n") );
	if (readmat("Bmat256.txt", (int *) B, N) < 0) 
		exit( 1 + printf("file problem\n") );
	
	
		start = MPI_Wtime();

		for (i = myid; i <N; i+=nproc){
			for (j = 0; j < N; j++)
			{	
				sum = 0;
				for (k = 0; k < N; k++)
					sum += A[i][k]*B[k][j];
				C[i][j] = sum;		
			
			};
			MPI_Gather(&C[i],N,MPI_INT,&cc[i],N, MPI_INT, 0,MPI_COMM_WORLD);
			
		}
	
	if(myid==0)
	writemat("MPIpinakas", (int *)cc, N);
	if(myid==0){
		double end = MPI_Wtime();
		double chronos =end-start;
	
	printf("Time  %f \n",chronos);
	}
	
	MPI_Finalize();
	//writemat("MPIpinakas", (int *)cc, N);
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
