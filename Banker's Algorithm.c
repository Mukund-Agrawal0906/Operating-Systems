#include <stdio.h>
#include <unistd.h>

#define noOfResources 3
#define numberOfProcess 5
int counter;
int safeInd;
int safeSequence[numberOfProcess];

void printVector(int n,int array[]);
void printMatrix(int matrix[numberOfProcess][noOfResources]);
int isDeadlock(int visited[], int needMatrix[numberOfProcess][noOfResources], int availableResources[noOfResources], int allocationMatrix[numberOfProcess][noOfResources]);

int main()
{

    int visited[numberOfProcess];     
    int totalResources[noOfResources]; 

    printf("+----------------------------------------------------------+\n");
    printf("Enter totalInstances vector : ");
    for (int row = 0; row < noOfResources; row++)
    {
        scanf("%d", &totalResources[row]);
    }

    int allocationMatrix[numberOfProcess][noOfResources];
    int requestMatrix[numberOfProcess][noOfResources];
    int needMatrix[numberOfProcess][noOfResources];

    printf("+----------------------------------------------------------+\n");
    printf("Enter Values for allocation Matrix\n");
    for (int row = 0; row < numberOfProcess; row++)
    {

        for (int col = 0; col < noOfResources; col++)
        {
            scanf("%d", &allocationMatrix[row][col]);
        }
    }
    printf("+----------------------------------------------------------+\n");

    printf("Enter values for request Matrix\n");
    for (int row = 0; row < numberOfProcess; row++)
    {
        for (int col = 0; col < noOfResources; col++)
        {
            scanf("%d", &requestMatrix[row][col]);
        }
    }

    for (int row = 0; row < numberOfProcess; row++)
    {
        for (int col = 0; col < noOfResources; col++)
        {
            needMatrix[row][col] = requestMatrix[row][col] - allocationMatrix[row][col];
        }
    }

    int availableResources[noOfResources];
    for (int col = 0; col < noOfResources; col++)
    {
        int sum = 0;
        for (int row = 0; row < numberOfProcess; row++)
        {
            sum += allocationMatrix[row][col];
        }
        availableResources[col] = totalResources[col] - sum; 
    }
    printf("+----------------------------------------------------------+\n");
    printf("needMatrix : \n");
    printMatrix(needMatrix);
    printf("+----------------------------------------------------------+\n");
    printf("\tavailable Resources : ");

    for (int col = 0; col < noOfResources; col++)
    {
        printf("%d ", availableResources[col]);
    }
    printf("\n+----------------------------------------------------------+\n");

    for (int i = 0; i < numberOfProcess; i++)
    {
        visited[i] = 0;
    }

    int terminatedProcess[numberOfProcess];                                        
    int terminatedInd = 0;                                                         
    counter = 0;                                                                   
    safeInd = 0;                                                                   
    if (isDeadlock(visited, needMatrix, availableResources, allocationMatrix) == 1) 
    {
        printf("++++++++++++++++++++++++++++++++++++++\n");
        printf("\tSystem is in Deadlock\n");
        printf("++++++++++++++++++++++++++++++++++++++\n");

        while (isDeadlock(visited, needMatrix, availableResources, allocationMatrix) == 1)
        {
            int maxAllocated = 0, ind = 0;
            for (int row = 0; row < numberOfProcess; row++)
            {
                if (visited[row] == 0)
                {
                    int tempmax = 0;
                    for (int col = 0; col < noOfResources; col++)
                    {
                        tempmax += allocationMatrix[row][col];
                    }
                    if (tempmax > maxAllocated)
                    {
                        ind = row;
                        maxAllocated = tempmax;
                    }
                }
            }
            visited[ind] = 1;
            safeSequence[safeInd++] = -1;
            for (int col = 0; col < noOfResources; col++)
            {
                availableResources[col] += allocationMatrix[ind][col];
            }
            terminatedProcess[terminatedInd++] = ind;
            sleep(2);
            printf("Terminated %d\n", ind);
            counter++;
        }
        printf("++++++++++++++++++++++++++++++++++++++\n");
        printf("\tTerminated Processes are : ");
        printVector(terminatedInd,terminatedProcess);
    }
    else
    {
        printf("No Deadlock\n:");
    }

    printf("++++++++++++++++++++++++++++++++++++++\n");
    printf("\tsafeSequence : ");
    for (int i = 0; i < numberOfProcess; i++)
    {
        if (safeSequence[i] > -1)
        {
            printf("p%d ", safeSequence[i]);
        }
    }
    printf("\n++++++++++++++++++++++++++++++++++++++\n");
    return (0);
}


int isDeadlock(int visited[], int needMatrix[numberOfProcess][noOfResources], int availableResources[noOfResources], int allocationMatrix[numberOfProcess][noOfResources])
{
    while (counter < numberOfProcess)
    {
        int deadlock = 1;
        for (int row = 0; row < numberOfProcess; row++)
        {
            if (visited[row] == 0)
            {
                int flag = 0;
                for (int col = 0; col < noOfResources; col++)
                {
                    if (needMatrix[row][col] > availableResources[col]) 
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0)
                {
                    sleep(1);
                    printf("\tallocated : %d\n", row); 
                    safeSequence[safeInd++] = row;    
                    for (int col = 0; col < noOfResources; col++)
                        availableResources[col] += allocationMatrix[row][col];
                    printf("available Resources : ");
                    printVector(noOfResources,availableResources);
                    visited[row] = 1; 
                    counter++;      
                    deadlock = 0;     
                }
            }
        }
        if (deadlock == 1)
        {
            return 1;
        }
    }
    return 0;
}
void printVector(int n, int array[])
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void printMatrix(int matrix[numberOfProcess][noOfResources])
{
    for (int row = 0; row < numberOfProcess; row++)
    {
        for (int col = 0; col < noOfResources; col++)
        {
            printf("%d ", matrix[row][col]);
        }
        printf("\n");
    }
    
}


/**
 *  totalResource Vector : 7 4 5
Allocation Matrix
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2

Request Matrix
7 4 3
2 2 2
6 0 2
3 1 1
4 3 2


*/
