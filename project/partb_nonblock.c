#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <sys/time.h>
#include<iostream>
#include<string>
#include<fstream>
#include<chrono>
#include<time.h>
using namespace std;
/*
  In here 'process o' which distribute the workload to other processes is considered 
  as Root (Master) process and other processes which do the computation is considered 
  as Slave task processes. 
*/
// Number of rows and columnns in a matrix

#define N 2

MPI_Status status;
MPI_Request request=MPI_REQUEST_NULL;
// Matrix holders are created
//double matrix_a[N][N],matrix_b[N][N],matrix_c[N][N];
void matrix_multiplication(int rows_matrix_a,int col_matrix_a,int rows_matrix_b,int col_matrix_b)
{
int ok=0;
double for_matrix_a[rows_matrix_a][col_matrix_a];
double for_matrix_b[rows_matrix_b][col_matrix_b];
double for_matrix_c[rows_matrix_a][col_matrix_b];
 int processCount, processId, slaveTaskCount, source, dest, rows, offset;
 struct timeval start, stop;
 //int rows_matrix_a,rows_matrix_b,col_matrix_a,col_matrix_b;
	MPI_Comm_rank(MPI_COMM_WORLD, &processId);
// Number of processes in communicator will be assigned to variable -> processCount
  MPI_Comm_size(MPI_COMM_WORLD, &processCount);
// Number of slave tasks will be assigned to variable -> slaveTaskCount
  slaveTaskCount = processCount - 1;
 if (processId == 0) {
	
	for(int i=0;i<rows_matrix_a;i++)
	{
		for(int j=0;j<col_matrix_a;j++)
		{
			for_matrix_a[i][j]=rand()%10;
			//for_matrix_a[i][j]=for_storing_the_values_in_matrices[for_getting_optimal_solution_for_matrices_1-1][i][j];
		}
	}
	cout<<"Matrix 1 : "<<endl;
	for(int i=0;i<rows_matrix_a;i++)
	{
		for(int j=0;j<col_matrix_a;j++)
		{
			cout<<for_matrix_a[i][j]<<" ";
			//for_matrix_a[i][j]=for_storing_the_values_in_matrices[for_getting_optimal_solution_for_matrices_1-1][i][j];
		}
		cout<<endl;
	}
	cout<<"Matrix 2 : "<<endl;

	for(int i=0;i<rows_matrix_b;i++)
	{
		for(int j=0;j<col_matrix_b;j++)
		{
			for_matrix_b[i][j]=rand()%10;
			//for_matrix_b[i][j]=for_storing_the_values_in_matrices[for_getting_optimal_solution_for_matrices_2-1][i][j];
		}
	}
	for(int i=0;i<rows_matrix_b;i++)
	{
		for(int j=0;j<col_matrix_b;j++)
		{
			cout<<for_matrix_b[i][j]<<" ";
			//for_matrix_b[i][j]=rand()%10;
			//for_matrix_b[i][j]=for_storing_the_values_in_matrices[for_getting_optimal_solution_for_matrices_2-1][i][j];
		}
		cout<<endl;
	}


//}
// Determine number of rows of the Matrix A, that is sent to each slave process
    rows = rows_matrix_a/slaveTaskCount;
// Offset variable determines the starting point of the row which sent to slave process
    offset = 0;
// Calculation details are assigned to slave tasks. Process 1 onwards;
// Each message's tag is 1
    for (dest=1; dest <= slaveTaskCount; dest++)
    {
      // Acknowledging the offset of the Matrix A
      MPI_Isend(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD,&request);
      // Acknowledging the number of rows
      MPI_Isend(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD,&request);
      // Send rows of the Matrix A which will be assigned to slave process to compute
	  //MPI_Send(&col_matrix_a, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
	  //MPI_Send(&rows_matrix_b, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
	  //MPI_Send(&col_matrix_b, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Isend(&for_matrix_a[offset][0], rows*col_matrix_a, MPI_DOUBLE,dest,1, MPI_COMM_WORLD,&request);
      // Matrix B is sent
      MPI_Isend(&for_matrix_b, col_matrix_a*col_matrix_b, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD,&request);
      // Offset is modified according to number of rows sent to each process
      offset = offset + rows;
    }
// Root process waits untill the each slave proces sent their calculated result with message tag 2
    for (int i = 1; i <= slaveTaskCount; i++)
    {
      source = i;
      // Receive the offset of particular slave process
      MPI_Irecv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &request);
      MPI_Wait(&request,&status);
      // Receive the number of rows that each slave process processed
      MPI_Irecv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &request);
      MPI_Wait(&request,&status);
      // Calculated rows of the each process will be stored int Matrix C according to their offset and
      // the processed number of rows
      MPI_Irecv(&for_matrix_c[offset][0], rows*col_matrix_b, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &request);
      MPI_Wait(&request,&status);
    }

// Print the result matrix
    printf("\nResult Matrix C = Matrix A * Matrix B:\n\n");
    for (int i = 0; i<rows_matrix_a; i++) {
      for (int j = 0; j<col_matrix_b; j++)
        printf("%.0f\t", for_matrix_c[i][j]);
      printf ("\n");
    }
    printf ("\n");
  }
// Slave Processes 
  if (processId > 0) {

    // Source process ID is defined
    source = 0;

    // Slave process waits for the message buffers with tag 1, that Root process sent
    // Each process will receive and execute this separately on their processes

    // The slave process receives the offset value sent by root process
    MPI_Irecv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &request);
    MPI_Wait(&request,&status);
    // The slave process receives number of rows sent by root process  
    MPI_Irecv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &request);
    MPI_Wait(&request,&status);
	//MPI_Recv(&col_matrix_a, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
	//MPI_Recv(&rows_matrix_b, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
	//MPI_Recv(&col_matrix_b, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    // The slave process receives the sub portion of the Matrix A which assigned by Root 
    MPI_Irecv(&for_matrix_a, rows*col_matrix_a, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &request);
    MPI_Wait(&request,&status);
   // The slave process receives the Matrix B
	//cout<<"Hell"<<rows_matrix_b<<"  "<<col_matrix_b<<endl;
    MPI_Irecv(&for_matrix_b, col_matrix_a*col_matrix_b, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &request);
	MPI_Wait(&request,&status);
	// Matrix multiplication
    for (int k = 0; k<col_matrix_b; k++) {
      for (int i = 0; i<rows; i++) {
        // Set initial value of the row summataion
        for_matrix_c[i][k] = 0.0;
        // Matrix A's element(i, j) will be multiplied with Matrix B's element(j, k)
        for (int j = 0; j<rows_matrix_a; j++){
			//cout<<k<<" "<<i<<" "<<j<<" "<<endl;
          for_matrix_c[i][k] = for_matrix_c[i][k] + for_matrix_a[i][j] * for_matrix_b[j][k];
		  //cout<<for_matrix_c[i][k]<<", ";
		}
      }
    }
    // Offset will be sent to Root, which determines the starting point of the calculated
    // value in matrix C 
    MPI_Isend(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD,&request);
    // Number of rows the process calculated will be sent to root process
    MPI_Isend(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD,&request);
    // Resulting matrix with calculated rows will be sent to root process
    MPI_Isend(&for_matrix_c, rows*col_matrix_b, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD,&request);
  }
}
int main(int argc, char **argv)
{
	clock_t start=clock();
	fstream myfile;
	srand(time(0));
	int count=0;
	int count1=0;
	int count2=0;
	int * for_storing_dimensions;
	int * for_storing_dimensions_without_duplication;
	int ***for_storing_the_values_in_matrices;
	int for_getting_optimal_solution_for_matrices_1=0;
	int for_getting_optimal_solution_for_matrices_2=0;
	int counter_for_multiplication=0;
	int rows_matrix_a,col_matrix_a,rows_matrix_b,col_matrix_b;

	myfile.open("file.txt",ios::in);
	if(myfile.is_open())
	{
		string for_passing;
		while(getline(myfile,for_passing))
		{
			count++;
		}
	}
	myfile.close();
	for_storing_dimensions=new int [count*2];
	for_storing_dimensions_without_duplication=new int [count];
	for_storing_the_values_in_matrices=new int **[count];
	for(int i=0;i<count;i++)
	{
		for_storing_the_values_in_matrices[i]=new int *[100];
		for(int j=0;j<100;j++)
		{
			for_storing_the_values_in_matrices[i][j]=new int[100];
		}
	}
	for(int i=0;i<count*2;i++)
	{
		for_storing_dimensions[i]=0;
	}
	for(int i=0;i<count;i++)
	{
		for(int j=0;j<100;j++)
		{
			for(int k=0;k<100;k++)
			{
				for_storing_the_values_in_matrices[i][j][k]=0;
			}
		}
	}
	myfile.open("file.txt",ios::in);
	if(myfile.is_open())
	{
		string line_by_line="";
		while(getline(myfile,line_by_line))
		{
			int seprating_the_rows_and_columns=0;
			string before_x="";
			string after_x="";
			cout<<line_by_line<<endl;
			int position=line_by_line.find("x");
			before_x=line_by_line.substr(0,position);
			after_x=line_by_line.substr(position+1,line_by_line.length());
			int temp1=stoi(before_x);
			for_storing_dimensions[count1]=temp1;
			int temp2=stoi(after_x);
			for_storing_dimensions[count1+1]=temp2;
			for(int i=0;i<temp1;i++)
			{
				for(int j=0;j<temp2;j++)
				{
					for_storing_the_values_in_matrices[count2][i][j]=rand()%30;
				}
			}
			count1+=2;
			count2++;
		}
	}
	myfile.close();
	cout<<"We have "<<count2<<" matrices in total "<<endl;
	int counter_for_dimensions_without_duplication=0;
	for(int i=0;i<(count*2)-1;i+=2)
	{
			for_storing_dimensions_without_duplication[counter_for_dimensions_without_duplication]=for_storing_dimensions[i];
			counter_for_dimensions_without_duplication++;
	}
	for_storing_dimensions_without_duplication[counter_for_dimensions_without_duplication]=for_storing_dimensions[(count*2)-1];
	counter_for_dimensions_without_duplication++;
	for(int i=0;i<count2;i++)
	{
		cout<<"Matrix number "<<i+1<<" is : "<<endl;
		for(int j=0;j<for_storing_dimensions_without_duplication[i];j++)
		{
			for(int k=0;k<for_storing_dimensions_without_duplication[i+1];k++)
			{
				cout<<for_storing_the_values_in_matrices[i][j][k]<<"  ";
			}
			cout<<endl;
		}
		cout<<endl;
	}
	//Now finding the optimal order for matrix multiplication
	int **for_storing_the_minimum_values_of_matrix_multiplication=new int*[counter_for_dimensions_without_duplication];
	int **for_ordering_of_multiplying_matrix=new int*[counter_for_dimensions_without_duplication];
	int *for_storing_the_order_in_which_they_will_be_multiplied_reversed_order=new int[counter_for_dimensions_without_duplication];
	int *for_storing_the_order_in_which_they_will_be_multiplied=new int[counter_for_dimensions_without_duplication];
	for(int i=0;i<counter_for_dimensions_without_duplication;i++)
	{
		for_storing_the_minimum_values_of_matrix_multiplication[i]=new int[counter_for_dimensions_without_duplication];
	}	
	for(int i=0;i<counter_for_dimensions_without_duplication;i++)
	{
		for_ordering_of_multiplying_matrix[i]=new int[counter_for_dimensions_without_duplication];
	}
	for(int i=0;i<counter_for_dimensions_without_duplication;i++)
	{
		for(int j=0;j<counter_for_dimensions_without_duplication;j++)
		{
			for_storing_the_minimum_values_of_matrix_multiplication[i][j]=0;
			for_ordering_of_multiplying_matrix[i][j]=0;
		}
	}
	for(int i=0;i<counter_for_dimensions_without_duplication;i++)
	{
		for_storing_the_order_in_which_they_will_be_multiplied_reversed_order[i]=0;
		for_storing_the_order_in_which_they_will_be_multiplied[i]=0;
	}	
	int iterator,minimum,storing_every_minimum;
	for(int i=1;i<counter_for_dimensions_without_duplication-1;i++)
	{
		for(int j=1;j<counter_for_dimensions_without_duplication-i;j++)
		{
			iterator=j+i;
			minimum=214748364;
			for(int k=j;k<=iterator-1;k++)
			{
				storing_every_minimum=for_storing_the_minimum_values_of_matrix_multiplication[j][k]+for_storing_the_minimum_values_of_matrix_multiplication[k+1][iterator]+for_storing_dimensions_without_duplication[j-1]*for_storing_dimensions_without_duplication[k]*for_storing_dimensions_without_duplication[iterator];
				if(storing_every_minimum<minimum)
				{
					minimum=storing_every_minimum;
					for_ordering_of_multiplying_matrix[j][iterator]=k;
				}
			}
			for_storing_the_minimum_values_of_matrix_multiplication[j][iterator]=minimum;
		}
	}
	//cout<<counter_for_dimensions_without_duplication<<endl;
	cout<<"Minimum number of calculations are : "<<for_storing_the_minimum_values_of_matrix_multiplication[1][counter_for_dimensions_without_duplication-1]<<endl;
	int starting_point=1,first_split=0,second_split=0,ending_point=counter_for_dimensions_without_duplication-1;
	int counter=0;
	while(true)
	{
		if(counter==counter_for_dimensions_without_duplication-1)
			break;
		first_split=for_ordering_of_multiplying_matrix[starting_point][ending_point];
		second_split=for_ordering_of_multiplying_matrix[starting_point][ending_point]+1;
		if(starting_point==first_split)
		{
			for_storing_the_order_in_which_they_will_be_multiplied_reversed_order[counter]=starting_point;
			counter++;
			starting_point++;
		}
		else if(starting_point==first_split+1)
		{
			for_storing_the_order_in_which_they_will_be_multiplied_reversed_order[counter]=starting_point;
			counter++;
			starting_point++;
			for_storing_the_order_in_which_they_will_be_multiplied_reversed_order[counter]=starting_point+1;
			counter++;
			starting_point++;
		}
		if(second_split==ending_point)
		{
			for_storing_the_order_in_which_they_will_be_multiplied_reversed_order[counter]=second_split;
			counter++;
			ending_point--;
		}
		else if(second_split+1==ending_point)
		{
			for_storing_the_order_in_which_they_will_be_multiplied_reversed_order[counter]=second_split+1;
			counter++;
			ending_point--;
			for_storing_the_order_in_which_they_will_be_multiplied_reversed_order[counter]=second_split;
			counter++;
			ending_point--;
		}
	}
	int cou=0;
	for(int i=counter_for_dimensions_without_duplication-2;i>=0;i--)
	{
		for_storing_the_order_in_which_they_will_be_multiplied[cou]=for_storing_the_order_in_which_they_will_be_multiplied_reversed_order[i];
		cou++;
	}
	cout<<"The optimal order for multiplying the matrices will be : "<<endl;
	for(int i=0;i<=counter_for_dimensions_without_duplication-2;i++)
	{
		cout<<"Matrix number : "<<for_storing_the_order_in_which_they_will_be_multiplied[i]<<endl;
	
	}
 
	for_getting_optimal_solution_for_matrices_1=for_storing_the_order_in_which_they_will_be_multiplied[counter_for_multiplication];
	counter_for_multiplication++;
	for_getting_optimal_solution_for_matrices_2=for_storing_the_order_in_which_they_will_be_multiplied[counter_for_multiplication];
	counter_for_multiplication++;
	int temp=0;
		if(for_getting_optimal_solution_for_matrices_1>for_getting_optimal_solution_for_matrices_2)
		{
			temp=for_getting_optimal_solution_for_matrices_1;
			for_getting_optimal_solution_for_matrices_1=for_getting_optimal_solution_for_matrices_2;
			for_getting_optimal_solution_for_matrices_2=temp;
		}
	rows_matrix_a=for_storing_dimensions_without_duplication[for_getting_optimal_solution_for_matrices_1-1];
	col_matrix_a=for_storing_dimensions_without_duplication[for_getting_optimal_solution_for_matrices_1];
	rows_matrix_b=for_storing_dimensions_without_duplication[for_getting_optimal_solution_for_matrices_2-1];
	col_matrix_b=for_storing_dimensions_without_duplication[for_getting_optimal_solution_for_matrices_2];
// MPI environment is initialized
  MPI_Init(&argc, &argv);
  matrix_multiplication(rows_matrix_a,col_matrix_a,rows_matrix_b,col_matrix_b);
  MPI_Finalize();
  clock_t end=clock();
  double difference= double(end - start)/CLOCKS_PER_SEC;
  cout<<"The execution time is : "<<difference<<endl;
}
