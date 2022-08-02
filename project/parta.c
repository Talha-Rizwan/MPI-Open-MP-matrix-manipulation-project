#include<iostream>
#include<fstream>
#include<string>
#include<time.h>
using namespace std;
int main()
{
	fstream myfile;
	srand(time(0));
	int count=0;
	int count1=0;
	int count2=0;
	int * for_storing_dimensions;
	int * for_storing_dimensions_without_duplication;
	int ***for_storing_the_values_in_matrices;
	//While loop in which we are checking number of lines present in the file
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
	//Array for storing the dimensions of every matrix
	for_storing_dimensions=new int [count*2];
	//Array for storing the dimensions without duplication
	for_storing_dimensions_without_duplication=new int [count];
	//3d dynamic array for storing all the values in matrix
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
	//Initializing all matrices by 0
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
	//File reading (line by line)
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
			//Splitting the line from first index to where x is present
			before_x=line_by_line.substr(0,position);
			//From x to end
			after_x=line_by_line.substr(position+1,line_by_line.length());
			int temp1=stoi(before_x);
			for_storing_dimensions[count1]=temp1;
			int temp2=stoi(after_x);
			for_storing_dimensions[count1+1]=temp2;
			//Random Initialization of matrices
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
	//Storing dimensions of every matrix
	for(int i=0;i<(count*2)-1;i+=2)
	{
			for_storing_dimensions_without_duplication[counter_for_dimensions_without_duplication]=for_storing_dimensions[i];
			counter_for_dimensions_without_duplication++;
	}
	for_storing_dimensions_without_duplication[counter_for_dimensions_without_duplication]=for_storing_dimensions[(count*2)-1];
	counter_for_dimensions_without_duplication++;
	//Storing dimensions without duplication
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
	//For storing the number of calculation that matrix will take with one another
	int **for_storing_the_minimum_values_of_matrix_multiplication=new int*[counter_for_dimensions_without_duplication];
	//For storing the ordering 
	int **for_ordering_of_multiplying_matrix=new int*[counter_for_dimensions_without_duplication];
	//In which order they should be multiplied
	int *for_storing_the_order_in_which_they_will_be_multiplied_reversed_order=new int[counter_for_dimensions_without_duplication];
	//As you need values that are pushed at last first, so reversing the array
	int *for_storing_the_order_in_which_they_will_be_multiplied=new int[counter_for_dimensions_without_duplication];
	for(int i=0;i<counter_for_dimensions_without_duplication;i++)
	{
		for_storing_the_minimum_values_of_matrix_multiplication[i]=new int[counter_for_dimensions_without_duplication];
	}	
	for(int i=0;i<counter_for_dimensions_without_duplication;i++)
	{
		for_ordering_of_multiplying_matrix[i]=new int[counter_for_dimensions_without_duplication];
	}
	//Initializing 2d arrays by zero
	for(int i=0;i<counter_for_dimensions_without_duplication;i++)
	{
		for(int j=0;j<counter_for_dimensions_without_duplication;j++)
		{
			for_storing_the_minimum_values_of_matrix_multiplication[i][j]=0;
			for_ordering_of_multiplying_matrix[i][j]=0;
		}
	}
	//Initializing 1d arrays by zero
	for(int i=0;i<counter_for_dimensions_without_duplication;i++)
	{
		for_storing_the_order_in_which_they_will_be_multiplied_reversed_order[i]=0;
		for_storing_the_order_in_which_they_will_be_multiplied[i]=0;
	}	
	int iterator,minimum,storing_every_minimum;
	//For calculating the minimum number of calculations
	for(int i=1;i<counter_for_dimensions_without_duplication-1;i++)
	{
		for(int j=1;j<counter_for_dimensions_without_duplication-i;j++)
		{
			iterator=j+i;
			minimum=214748364;
			for(int k=j;k<=iterator-1;k++)
			{
				//Checking the number of calculations
				storing_every_minimum=for_storing_the_minimum_values_of_matrix_multiplication[j][k]+for_storing_the_minimum_values_of_matrix_multiplication[k+1][iterator]+for_storing_dimensions_without_duplication[j-1]*for_storing_dimensions_without_duplication[k]*for_storing_dimensions_without_duplication[iterator];
				//Updating minimum value if it is less than the previous
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
	//For getting the optimal order from table
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
			for_storing_the_order_in_which_they_will_be_multiplied_reversed_order[counter]=second_split;
			counter++;
			ending_point--;
			for_storing_the_order_in_which_they_will_be_multiplied_reversed_order[counter]=second_split+1;
			counter++;
			ending_point--;
		}
	}
	int cou=0;
	//Reversing the array
	for(int i=counter_for_dimensions_without_duplication-2;i>=0;i--)
	{
		for_storing_the_order_in_which_they_will_be_multiplied[cou]=for_storing_the_order_in_which_they_will_be_multiplied_reversed_order[i];
		cou++;
	}
	//Displaying the array
	cout<<"The optimal order for multiplying the matrices will be : "<<endl;
	for(int i=0;i<=counter_for_dimensions_without_duplication-2;i++)
	{
		cout<<"Matrix number : "<<for_storing_the_order_in_which_they_will_be_multiplied[i]<<endl;
	
	}
	return 0;	
}
