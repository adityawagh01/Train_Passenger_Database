#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define BOGIES 8 //assuming 2 each of AC1,AC2,AC3 and sleeper
#define SEATS 20   //20 seats for each bogie

struct passengerDetails{
    char first_name[20];
    char last_name[20];
    int passengerID;   //PNR no of passenger
    char train_name[20];
    char boarding_station[20];
    char travelling_class[10]; 
    char destination_station[20];
    int train_no;
    char date[10];
    int seat_no;
    int bogie_no;
    bool coaches[BOGIES][SEATS]; // assuming the train in the form of a 2D array where rows=bogies , columns=seats
    int no_of_passengers;
};

struct node{
    struct passengerDetails record;  // data part
    struct node *next;
};

// Function to create a node for a new passeenger
// This function will keep sorted the LL based on train no
struct node *createNode(struct node *head,struct node *ptr)
{
    int key=ptr->record.train_no; // for sorting the LL based on train no
    
    if(head==NULL)
    {
        head=ptr;
    }
    else if(key<= head->record.train_no)
    {
        ptr->next=head;
        head=ptr;
    }
    else
    {
        // creating a temp pointer to traverse the LL
        struct node *temp=head;
        while((temp->next != NULL) && (temp->next->record.train_no < key))
        {
            temp=temp->next;
        }
        ptr->next=temp->next;
        temp->next=ptr;
    }
    
    //initializing the seat no of train to false(vacant) when a new passenger is registered
    for(int i=0;i<BOGIES;i++)
    {
        for(int j=0;j<SEATS;j++)
        {
            ptr->record.coaches[i][j]=false;
        }
    }
    ptr->record.no_of_passengers=1;
    
    return head;
}

//updating the seat no for all train with common train no and date 
void updateSeat(struct node *ptr,struct node *temp,int i,int j,int op)
{
    // if passenger data is inserted(op=1) else if passenger data is deleted(op=0)
    if(op==0)
    {
        while(temp!=NULL && temp->record.train_no != ptr->record.train_no)
        {
            temp=temp->next;
        }
    }
    
    // update the seat no and no_of_passengers for each passenger travelling in the same train
    while(temp!=NULL && temp->record.train_no == ptr->record.train_no) 
    {
        if(strcmp(ptr->record.date,temp->record.date)==0)
        {
            if(op==1)
            {
                temp->record.coaches[i][j]=true;  
                temp->record.no_of_passengers +=1;
            }
            else
            {
                temp->record.coaches[i][j]=false; 
                temp->record.no_of_passengers -=1;
            }
        }
        temp=temp->next;
    }
}

// Function to allocate seat to the passenger
int seatNo(struct node *head,struct node *ptr)
{
    int index=-1,flag=1;
    int class;
    int i,j;
    
    if(strcmp(ptr->record.travelling_class,"AC1") == 0)
        class=1;
    else if(strcmp(ptr->record.travelling_class,"AC2") == 0)
        class=2;
    else if(strcmp(ptr->record.travelling_class,"AC3") == 0)
        class=3;
    else
        class=4;
    
    struct node *temp=ptr;
    int bogies=BOGIES/4;
    
    // if there is no other train as of the passenger we allocate first seat of the desired class to passenger 
    if(ptr->next == NULL || ptr->record.train_no != ptr->next->record.train_no)
    {
        ptr->record.coaches[(class-1)*bogies][0]=true;
        ptr->record.bogie_no=(class-1)*bogies + 1; // add 1 because array index starts from 0 but bogie_no cannot be 0
        index=1;
    }
    else
    {
        int found=0,brk=1;
        temp=temp->next;
        
        //search for a same train of that of the passenger for same date
        while(temp!=NULL && ptr->record.train_no==temp->record.train_no && brk==1)
        {
            if(strcmp(ptr->record.date,temp->record.date)==0)
            {
                found=1;
                brk=0;
            }
            if(brk==1)
            {
                temp=temp->next;    
            }
        }
        
        // if no train is found assign seat no as 1 of that class
        if(found==0)
        {
            ptr->record.coaches[(class-1)*bogies][0]=true;
            ptr->record.bogie_no=(class-1)*bogies + 1;
            index=1; 
        }
        else
        {
            //updating the seats for new passenger that are already filled up 
            for(int i=0 ; i<BOGIES ; i++)
            {
                for(int j=0;j<SEATS;j++)
                {
                    ptr->record.coaches[i][j]=temp->record.coaches[i][j];
                }   
            }
            
            //finding a vacant seat for the passenger 
            for(i=(class-1)*bogies ; i<class*bogies && flag==1 ; i++)
            {
                for(j=0;j<SEATS && flag==1;j++)
                {
                    if(ptr->record.coaches[i][j]==false)
                    {
                        index=j+1;
                        ptr->record.coaches[i][j]=true;
                        ptr->record.bogie_no = i+1;
                        flag=0;
                        j--;
                        i--;
                    }
                }    
            }
            // update the seats for all the passenger of the same train for same date
            updateSeat(ptr,temp,i,j,1);
        }
    }
    
    return index; // index is the seat no allocated to the passenger
}

// Function to take input for passengers 
struct node *insert(struct node *head)
{
    int n;
    int seat;
    
    printf("Enter the no of passengers: ");
    scanf("%d",&n);
    
    for(int i=0;i<n;i++)
    {
        // creating a node to take input of passenger from user
        struct node *ptr=malloc(sizeof(struct node));
        ptr->next=NULL;
        
        printf("\nEnter passenger %d first name: ",i+1);
        scanf("%s",ptr->record.first_name);
        printf("Enter passenger %d last name: ",i+1);
        scanf("%s",ptr->record.last_name);
        printf("Enter passenger id as int (e.g., 123456)\n");
        printf("Enter passenger %d ID: ",i+1);
        scanf("%d",&ptr->record.passengerID);
        printf("Enter passenger %d boarding train: ",i+1);
        scanf("%s",ptr->record.train_name);
        printf("Enter passenger %d boarding station: ",i+1);
        scanf("%s",ptr->record.boarding_station);
        printf("Enter passenger %d destination station: ",i+1);
        scanf("%s",ptr->record.destination_station);
        printf("Enter the travelling_class as 'AC1' ,'AC2' ,'AC3' or 'Sleeper'\n");
        printf("Enter passenger %d travelling class: ",i+1);
        scanf("%s",ptr->record.travelling_class);
        printf("Enter train id as int (e.g., 12345)\n");
        printf("Enter passenger %d train ID: ",i+1);
        scanf("%d",&ptr->record.train_no);
        printf("Enter the date of travel in 'mm/dd' format\n");
        printf("Enter the date of travel for passenger %d :",i+1);
        scanf("%s",ptr->record.date);
        
        if(ptr == NULL) // if memory won't get allocated then reservation fails
        {
            printf("Reservation for passenger %d failed!!!",i+1);
        }
        else
        {
            head=createNode(head,ptr); // passing ptr node created above to createNode Function to create LL
        
            seat=seatNo(head,ptr);  // seatNo Function will return the seat allocated to the Passenger
            if(seat==-1) //if there is no vacant seat 
            {
                printf("Passenger on waiting list !!");
            }
            else
            {
                ptr->record.seat_no=seat;   
                printf("\nReservation done successfully !! ");
                printf("Your seat no is %d \n",ptr->record.seat_no);
            }
        }
    }
    
    return head;
}

// Function to delete the passenger details
struct node *delete(struct node *head)
{
    int ID;
    printf("Enter the passenger ID to delete : ");
    scanf("%d",&ID);
    
    struct node *ptr=head;

    if(head->record.passengerID == ID)
    {
        int bogie=(head->record.bogie_no)-1;
        int seat=(head->record.seat_no)-1;
        head->record.coaches[bogie][seat]=false; // declaring the seat of passenger as vacant
        
        if(head->next == NULL)
        {
            free(head);
            head=NULL;
        }
        else
        {
            ptr=head->next;
            if(ptr->record.train_no == head->record.train_no)
            {
                updateSeat(head,ptr,bogie,seat,0);
            }
            free(head);
            head=ptr;
        }
        printf("Reservation of Passenger with ID %d cancelled successfully!!\n",ID);
    }
    else
    {
        while(ptr->next!=NULL && ptr->next->record.passengerID != ID)
        {
            ptr=ptr->next;
        }
        if(ptr->next==NULL && ptr->record.passengerID != ID)
        {
            printf("No such passenger with ID %d exist.\n",ID);
            printf("Reservation cancellation failed!!\n");
        }
        else
        {
            struct node *temp=ptr->next;
            
            int bogie=(temp->record.bogie_no)-1;
            int seat=(temp->record.seat_no)-1;
            temp->record.coaches[bogie][seat]=false;
            
            updateSeat(temp,head,bogie,seat,0);
            
            ptr->next=temp->next;
            free(temp);
            temp=NULL;
            printf("Reservation of Passenger with ID %d cancelled successfully!!\n",ID);
        }
    }
    
    return head;
}

void getListDestination(struct node *head)
{
    int train_no;
    char destination_station[20];
    struct node *ptr=head;
    printf("Enter the train ID and destination station you want to get the list for:\n");
    printf("Enter train ID : ");
    scanf("%d",&train_no);
    printf("Enter destination station : ");
    scanf("%s",destination_station);
    
    while(ptr!=NULL && ptr->record.train_no != train_no)
    {
        ptr=ptr->next;
    }
    if(ptr==NULL)
    {
        printf("No such list found!!\n");
    }
    else
    {
        printf("\nThe list is : \n");
        printf("\npassenger_name\tPassenger_ID\n");
        printf("-----------------------------------\n");
        while(ptr->record.train_no == train_no)
        {
            if(strcmp(ptr->record.destination_station,destination_station)==0)
            {
                printf("%s %s\t%d\n",ptr->record.first_name,ptr->record.last_name,ptr->record.passengerID);
            }
            ptr=ptr->next;
        }   
        printf("\n-----------------------------------\n");
    }
}

// Creating a seperate LL to store the sorting based on train date
struct sortByTD{
    char destination_station[20];
    char date[20];
    struct sortByTD *next;  
};

// NodeTD Function will keep sorted the LL based on train date
struct sortByTD *NodeTD(struct node *temp,struct sortByTD *headTD)
{
    struct sortByTD *ptr=malloc(sizeof(struct sortByTD));
    strcpy(ptr->destination_station,temp->record.destination_station);
    strcpy(ptr->date,temp->record.date);
    ptr->next=NULL;
    
    if(headTD==NULL)
    {
        headTD=ptr;
    }
    else if(strcmp(ptr->date,headTD->date)<0)
    {
        ptr->next=headTD;
        headTD=ptr;
    }
    else
    {
        struct sortByTD *temp2=headTD;
        while((temp2->next != NULL)&&(strcmp(temp2->date,ptr->date)<0))
        {
            temp2=temp2->next;
        }
        ptr->next=temp2->next;
        temp2->next=ptr;
    }
    
    return headTD;
}

void SortByTravelDate(struct node *head)
{
    struct sortByTD *headTD=NULL;
    
    struct node*temp=head;
    int passengerID;
    printf("Enter passenger ID: ");
    scanf("%d",&passengerID);
    while(temp!=NULL)
    {
        if(temp->record.passengerID == passengerID)
        {
            headTD=NodeTD(temp,headTD);
        }
        temp=temp->next;
    }
    
    printf("The list of destination station is: \n");
    struct sortByTD *temp2=headTD;
    while(temp2!=NULL)
    {
        printf("%s\n",temp2->destination_station);
        temp2=temp2->next;
    }
    
    // deleting the sortByTD LL since it will no longer be needed after this Function
    // Also it will save space in heap for other operations
    struct sortByTD *current=headTD;
    struct sortByTD *next=NULL;
    
    while(current!=NULL)
    {
        next=current->next;
        free(current);
        current=next;
    }
    headTD=NULL;
}

// Again Creating a seperate LL for sorting based on no_of_passengers on train
struct SortT{
    int no_of_passengers;
    int train_no;
    char date[20];
    struct SortT *next;
};

// NodeTrain Function will keep sorted the LL based on no_of_passengers
struct SortT *NodeTrain(struct SortT *headTrain,struct node *ptr)
{
    struct SortT *temp= malloc(sizeof(struct SortT));
    temp->no_of_passengers=ptr->record.no_of_passengers;
    temp->train_no=ptr->record.train_no;
    strcpy(temp->date,ptr->record.date);
    temp->next=NULL;
    
    if(headTrain == NULL)
    {
        headTrain=temp;
    }
    else if(temp->no_of_passengers < headTrain->no_of_passengers)
    {
        temp->next=headTrain;
        headTrain=temp;
    }
    else
    {
        struct SortT *temp2=headTrain;
        while((temp2->next != NULL)&&(temp2->no_of_passengers < temp->no_of_passengers))
        {
            temp2=temp2->next;
        }
        temp->next=temp2->next;
        temp2->next=temp;
    }
    
    return headTrain;
}

void SortTrains(struct node *head)
{
    struct node *ptr=head;
    struct SortT *headTrain=NULL;
    
    while(ptr != NULL)
    {
        headTrain=NodeTrain(headTrain,ptr);
        
        ptr=ptr->next;
    }
    
    struct SortT *temp=headTrain;
    printf("The list is:\n");
    printf("\nTrainNo   Date \n");
    printf("---------------------\n");
    while(temp != NULL)
    {
        // printf("The train no is : %d\n",temp->train_no);
        // printf("The travel date is : %s\n",temp->date);

        printf("%d \t %s\n",temp->train_no,temp->date);
        
        temp=temp->next;
    }
    printf("\n---------------------\n");
    
    // deleting the SortTrains LL since it will no longer be needed after this Function
    // Also it will save space in heap for other operations
    struct SortT *current=headTrain;
    struct SortT *next=NULL;
    
    while(current!=NULL)
    {
        next=current->next;
        free(current);
        current=next;
    }
    headTrain=NULL;
}

// PromotePassengers Function will check for seat avaibality in next upper class 
void PromotePassengers(struct node *head)
{
    int class;
    int train_no;
    int ID;
    char date[10];
    
    printf("Enter train ID :");
    scanf("%d",&train_no);
    printf("Enter date of travel :");
    scanf("%s",date);
    printf("Enter passenger ID : ");
    scanf("%d",&ID);
    
    struct node *temp=head;
    
    while(temp!=NULL && temp->record.train_no != train_no)
    {
        temp=temp->next;
    }
    struct node *temp2=temp;
    while(temp->record.train_no == train_no && strcmp(temp->record.date,date)!=0 && temp->record.passengerID!=ID)
    {
        temp=temp->next;
    }
    
    if(strcmp(temp->record.travelling_class,"AC1") == 0)
    {
        printf("Passenger is in AC1 class, hence can't be promoted!!\n");   
    }
    else
    {
        if(strcmp(temp->record.travelling_class,"AC2") == 0)
            class=2;
        else if(strcmp(temp->record.travelling_class,"AC3") == 0)
            class=3;
        else
            class=4;
        
        int bogie=((class-1)*BOGIES)/4;  
        int found=0;
        
        for(int i=bogie-2;i<bogie && found==0;i++)
        {
            for(int j=0;j<SEATS && found==0;j++)
            {
                if(temp->record.coaches[i][j]==false)
                {
                    //declaring old seat vacant
                    int old_bogie=temp->record.bogie_no -1;
                    int old_seat=temp->record.seat_no -1;
                    temp->record.coaches[old_bogie][old_seat]=false; 
                    updateSeat(temp,head,old_bogie,old_seat,0);
                    
                    temp->record.seat_no=j+1;  // updating seat and bogie no and travelling_class of the passenger 
                    temp->record.bogie_no=i+1;
                    if(i<2)
                        strcpy(temp->record.travelling_class,"AC1");
                    else if(i<4)
                        strcpy(temp->record.travelling_class,"AC2");
                    else if(i<6)
                    {
                        strcpy(temp->record.travelling_class,"AC3");
                    }
                    
                    printf("Congratulations!!!\n");
                    printf("Passenger with ID %d promoted to class %s\n",ID,temp->record.travelling_class);
                    printf("New seat no is : %d\n",j+1);
                    
                    updateSeat(temp,temp2,i,j,1); //vacant update 
                    found=1;
                }
            }
        }
    }
}

// display Function to display passenger details sorted by train no 
void display(struct node *head)
{
    struct node *temp=head;
    
    printf("passenger details are : \n");
    printf("\nPASSENGER_NAME\t ID\tTrainID\t SeatNO BOGIE \tCLASS \n");
    printf("----------------------------------------------------------\n");
    while(temp != NULL)
    {   
        printf("%s %s \t",temp->record.first_name,temp->record.last_name);
        printf("%d \t %d \t ",temp->record.passengerID,temp->record.train_no);
        printf("%d \t %d \t %s\n",temp->record.seat_no,temp->record.bogie_no,temp->record.travelling_class);
        
        temp=temp->next;
    }
    printf("\n----------------------------------------------------------\n");
}

// dummyNodes Function will create dummy Nodes of Passengers
// We can add as many nodes as we want in this Function
struct node *dummyNodes(struct node *head)
{
    struct node *ptr=malloc(sizeof(struct node));
    // dummy passenger node 1
    strcpy(ptr->record.first_name,"Abhilash");
    strcpy(ptr->record.last_name,"Joshi");
    ptr->record.passengerID=100000;
    strcpy(ptr->record.train_name,"Durunto");
    strcpy(ptr->record.boarding_station,"Nagpur");
    strcpy(ptr->record.destination_station,"Mumbai");
    strcpy(ptr->record.travelling_class,"AC1");
    ptr->record.train_no=12290;
    strcpy(ptr->record.date,"01/01");
    ptr->next=NULL;
    
    head=createNode(head,ptr);
    ptr->record.seat_no=seatNo(head,ptr);
    ptr=NULL;
    
    // dummy passenger node 2
    ptr=malloc(sizeof(struct node));
    strcpy(ptr->record.first_name,"Rajesh");
    strcpy(ptr->record.last_name,"Singh");
    ptr->record.passengerID=100001;
    strcpy(ptr->record.train_name,"Howrah");
    strcpy(ptr->record.boarding_station,"Ahmedabad ");
    strcpy(ptr->record.destination_station,"Howrah");
    strcpy(ptr->record.travelling_class,"AC2");
    ptr->record.train_no=12833;
    strcpy(ptr->record.date,"01/01");
    ptr->next=NULL;
    
    head=createNode(head,ptr);
    ptr->record.seat_no=seatNo(head,ptr);
    ptr=NULL;
    
    // dummy passenger node 3
    ptr=malloc(sizeof(struct node));
    strcpy(ptr->record.first_name,"Seema");
    strcpy(ptr->record.last_name,"Mishra");
    ptr->record.passengerID=100003;
    strcpy(ptr->record.train_name,"Gitanjali");
    strcpy(ptr->record.boarding_station,"Mumbai");
    strcpy(ptr->record.destination_station,"Howrah");
    strcpy(ptr->record.travelling_class,"Sleeper");
    ptr->record.train_no=12853;
    strcpy(ptr->record.date,"01/02");
    ptr->next=NULL;
    
    head=createNode(head,ptr);
    ptr->record.seat_no=seatNo(head,ptr);
    ptr=NULL;
    
    // dummy passenger node 4
    ptr=malloc(sizeof(struct node));
    strcpy(ptr->record.first_name,"Kartik");
    strcpy(ptr->record.last_name,"Patel");
    ptr->record.passengerID=100010;
    strcpy(ptr->record.train_name,"Rajdhani");
    strcpy(ptr->record.boarding_station,"Bilaspur");
    strcpy(ptr->record.destination_station,"Delhi");
    strcpy(ptr->record.travelling_class,"AC3");
    ptr->record.train_no=12441;
    strcpy(ptr->record.date,"01/02");
    ptr->next=NULL;
    
    head=createNode(head,ptr);
    ptr->record.seat_no=seatNo(head,ptr);
    ptr=NULL;
    
    return head;
}

int main()
{
    int choice;
    struct node *head=NULL;
    
    printf("\nThe dummy Passenger nodes are : \n");
    head=dummyNodes(head);
    display(head);
    
    do
    {
        printf("\n");
        printf("0 - Exit.\n");
        printf("1 - Insert.\n");
        printf("2 - delete.\n");
        printf("3 - get List destination.\n");
        printf("4 - Sort by travel date.\n");
        printf("5 - Sort trains based on no_of_passengers.\n");
        printf("6 - Promote Passengers.\n");
        printf("7 - Display Passenger details.\n");

        printf("Enter your choice : ");
        scanf("%d",&choice);
        printf("\n");

        switch(choice)
        {
            case 0: break;
             
            case 1:
                head=insert(head);
            break;
                 
            case 2:
                if(head==NULL)
                {
                    printf("Passengers list is empty!!\n");
                }
                else
                {
                    head=delete(head);
                }
            break;
            
            case 3:
                getListDestination(head);
            break;
            
            case 4:
                SortByTravelDate(head);
            break;
            
            case 5:
                SortTrains(head);
            break;
            
            case 6:
                PromotePassengers(head);
            break;
            
            case 7:
                display(head);
            break;
             
            default:
                printf("An invalid choice.\n");
        }
    } while (choice != 0);
    
    
    return 0;
}
