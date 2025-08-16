#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#define size 3
#define MAX_SIZE 20
#define PHONE_SIZE 15
#define EMAIL_SIZE 50
#define STATION_SIZE 30
#define DATE_SIZE 12

typedef struct NODE
{
	int reg_no;
	char name[MAX_SIZE];
	char phone[PHONE_SIZE];
	char email[EMAIL_SIZE];
	char from_station[STATION_SIZE];
	char to_station[STATION_SIZE];
	char travel_date[DATE_SIZE];
	float advance_payment;
	struct NODE *next;
} node;

// booking history 
char history[50][100];  
int history_count = 0;

// BST for search
typedef struct BST_NODE
{
	int reg_no;
	struct BST_NODE* left;
	struct BST_NODE* right;
} bst_node;

node* deq();
void create();
int reserve();
int cancel(int);
void enq(node*);
void display();

// BST functions
bst_node* bst_insert(bst_node* root, int reg_no);
int bst_search(bst_node* root, int reg_no);
void search_passenger_by_reg();

// history functions
void add_simple_history(char* message);
void show_history();


node *start;
node *front;
node *rear;
bst_node *bst_root;  

int count=0;
int num=0;

void create()
{
	start=(node *)malloc(sizeof(node));
	start->reg_no = 1;

	printf("Name: ");
	scanf("%s",start->name);
	printf("Phone Number: ");
	scanf("%s", start->phone);
	printf("Email: ");
	scanf("%s", start->email);
	printf("From Station: ");
	scanf("%s", start->from_station);
	printf("To Station: ");
	scanf("%s", start->to_station);
	printf("Travel Date (DD/MM/YYYY): ");
	scanf("%s", start->travel_date);
	printf("Advance Payment Amount : ");
	scanf("%f", &start->advance_payment);
	
	start->next=NULL;
	num++;
	
	// add to history
	add_simple_history("BOOKED: First passenger registered");
	
}

int reserve()
{
	
	if(start == NULL)
	{
		create();
		return 1;
	}

	else 
	{
		node *temp, *new_node, *prev;

		new_node=(node *)malloc(sizeof(node));
		
		printf("Name: ");
		scanf("%s",new_node->name);
		printf("Phone Number: ");
		scanf("%s", new_node->phone);
		printf("Email: ");
		scanf("%s", new_node->email);
		printf("From Station: ");
		scanf("%s", new_node->from_station);
		printf("To Station: ");
		scanf("%s", new_node->to_station);
		printf("Travel Date (DD/MM/YYYY): ");
		scanf("%s", new_node->travel_date);
		printf("Advance Payment Amount: ");
		scanf("%f", &new_node->advance_payment);

		new_node->next=NULL;
		
		temp=start;
		int i=1;
		if(temp->reg_no == 0)
		{
			temp->reg_no = 1;
			strcpy(temp->name,new_node->name);
			strcpy(temp->phone, new_node->phone);
			strcpy(temp->email, new_node->email);
			strcpy(temp->from_station, new_node->from_station);
			strcpy(temp->to_station, new_node->to_station);
			strcpy(temp->travel_date, new_node->travel_date);
			temp->advance_payment = new_node->advance_payment;
			num++;
			// add to history
			add_simple_history("BOOKED: Passenger registered");
			return 1;
		}
		while(temp->next != NULL)
		{
			if(temp->reg_no != i++)
				break;

			prev = temp;
			temp=temp->next;
		}	

		if(num<size)
		{
			num++;
			i++;
			if(temp->reg_no == (prev->reg_no+1) || i == 2) // If all gaps were filled
			{
				new_node->reg_no = i;
				temp->next = new_node;
				// add to history
				add_simple_history("BOOKED: New passenger added");
				return i;
			}
			else	// Filling the gaps
			{
				new_node->next = temp;
				prev->next = new_node;
				new_node->reg_no = (temp->reg_no) - 1;
				printf("reg = %d\n",new_node->reg_no);
				// add to history
				add_simple_history("BOOKED: Gap filled");
				return new_node->reg_no;
			}
			
		}

		else
		{
			enq(new_node);
			return 0;
		}
	}
}


int cancel(int reg)
{
	node *ptr, *preptr, *new;
	ptr=start;
	preptr=NULL;
	if(start==NULL)
	return -1;
	
	// add cancellation to history
	add_simple_history("CANCELLED: Passenger cancelled");
	
	if(ptr->next==NULL && ptr->reg_no==reg)	// If only 1 person in reservation
	{
		start=NULL;
		num--;
		free(ptr);
		return 1;
	}
		
	else
	{	if(reg == 1)
		{
			ptr->reg_no = 0;
			new=deq(reg);
			if(new != NULL)
			{
				ptr->reg_no = 1;
				strcpy(ptr->name,new->name);
				strcpy(ptr->phone, new->phone);
				strcpy(ptr->email, new->email);
				strcpy(ptr->from_station, new->from_station);
				strcpy(ptr->to_station, new->to_station);
				strcpy(ptr->travel_date, new->travel_date);
				ptr->advance_payment = new->advance_payment;
				num++;
				// add to history 
				add_simple_history("PROMOTED: From waiting list");
				return 1;
			}
		}	

		else
		{
			while(ptr->reg_no != reg && ptr->next!=NULL)
			{
				preptr=ptr;
				ptr=ptr->next;
			}

			if(ptr->next == NULL && ptr->reg_no != reg)
				return -1;
			else
				preptr->next = ptr->next;

			free(ptr);
			new=deq(reg);
			num--;
			if(new != NULL)
			{
				node *waiting = start;
				while(waiting->reg_no != (new->reg_no - 1))
					waiting = waiting->next;

				new->next = waiting->next;
				waiting->next = new;
				num++;
				// add to history
				add_simple_history("PROMOTED: From waiting list");
			}
			return 1;
		}
	}
	return -1; // registration not found
}

void enq(node *new_node)
{
	if(rear==NULL)
	{
		rear=new_node;
		rear->next=NULL;
		front=rear;
	}
	else
	{
		node *temp;
		temp=new_node;
		rear->next=temp;
		temp->next=NULL;
		rear=temp;
	}
	count++;
}

node* deq(int reg)
{
	node *front1;
	front1 = front;
	if(front==NULL)
		return NULL;
	else
	{
	    count-- ;
		if(front->next != NULL)
		{
			front=front->next;
			front1->next=NULL;
			front1->reg_no = reg;
			return front1;
		}
		else
		{
			front=NULL;
			rear=NULL;
			front1->reg_no = reg;
			return front1;
		}
	}
}


void display()
{
	node *temp;
	temp = start;
	while(temp!=NULL)
	{
		if(temp->reg_no != 0)
		{
			printf("\nRegistration Number: %d\n", temp->reg_no);
			printf("Name : %s\n", temp->name);
			printf("Phone : %s\n", temp->phone);
			printf("Email : %s\n", temp->email);
			printf("From Station : %s\n", temp->from_station);
			printf("To Station : %s\n", temp->to_station);
			printf("Travel Date : %s\n", temp->travel_date);
			printf("Advance Payment : Rs. %.2f\n\n", temp->advance_payment);
		}
		temp=temp->next;
    }
}

// BST Functions 

// BST insert function
bst_node* bst_insert(bst_node* root, int reg_no) {
    if (root == NULL) {
        bst_node* new_node = (bst_node*)malloc(sizeof(bst_node));
        new_node->reg_no = reg_no;
        new_node->left = new_node->right = NULL;
        return new_node;
    }
    if (reg_no < root->reg_no) 
        root->left = bst_insert(root->left, reg_no);
    else if (reg_no > root->reg_no) 
        root->right = bst_insert(root->right, reg_no);
    return root;
}

// BST search function 
int bst_search(bst_node* root, int reg_no) {
    if (root == NULL) return 0;
    if (root->reg_no == reg_no) return 1;
    if (reg_no < root->reg_no) 
        return bst_search(root->left, reg_no);
    else 
        return bst_search(root->right, reg_no);
}

// search interface using BST
void search_passenger_by_reg() {
    int reg_no;
    printf("\nEnter Registration Number to search: ");
    scanf("%d", &reg_no);
    
    // add reg_no to BST 
    bst_root = bst_insert(bst_root, reg_no);
    
    // search in linked list for details
    node* temp = start;
    while(temp != NULL) {
        if(temp->reg_no == reg_no) {
            printf("\n=== PASSENGER FOUND ===\n");
            printf("Registration Number: %d\n", temp->reg_no);
            printf("Name: %s\n", temp->name);
            printf("Phone: %s\n", temp->phone);
            printf("Email: %s\n", temp->email);
            printf("From Station: %s\n", temp->from_station);
            printf("To Station: %s\n", temp->to_station);
            printf("Travel Date: %s\n", temp->travel_date);
            printf("Advance Payment: Rs. %.2f\n", temp->advance_payment);
            printf("======================\n");
            return;
        }
        temp = temp->next;
    }
    printf("\nPassenger with Registration Number %d not found!\n", reg_no);
}

// History Functions 

void add_simple_history(char* message) {
    if(history_count < 50) {
        strcpy(history[history_count], message);
        history_count++;
    }
}

void show_history() {
    printf("\n=== BOOKING HISTORY ===\n");
    for(int i = 0; i < history_count; i++)
        printf("%d. %s\n", i+1, history[i]);
    printf("========================\n");
}

int main()
{
	int choice, status=0,canc=0, reg=0;
	start=NULL;

	rear=NULL;
	front=NULL;
	
	printf("\t\t\t***RAILWAY RESERVATION***\t\t\t\t\n");
	printf("\t\t\t    ***ADMIN PANEL***\t\t\t\t\n");
	
	while(choice!=6)
	{
	printf("\n\nDo you want to - \n 1. Reserve a ticket? \n 2. Cancel Booking \n 3. Display passenger details \n 4. Fast Search Passenger (by Reg No.) \n 5. View History \n 6. exit\n\n");
	scanf("%d", &choice); 
	switch(choice)
	{	
		case 1 :  status = reserve();
	              if(status==0)
	                printf("\nBooking Full!! \nYou are added to waiting list. Waiting list number %d", count);
	              else
	                printf("\nBooking Successful!!! Enjoy your journey! Your Reg No. is %d\n", status);
	              break;
	        
	    case 2:   printf("\nGive the Registration number to be cancelled : ");
	              scanf("%d", &reg);
	              if(reg>num)
	              	printf("Registration number invalid !!");
	              else
	              	{
	              		canc=cancel(reg);
	              		if(canc==-1)
	              			printf("Registration number invalid !!");
	              		else
	              			printf("Registration cancelled successfully");
	              	}
	              break;
	              
	    case 3: display();
	    break;
	    case 4: search_passenger_by_reg();
	    break; 
	    case 5: show_history();   
	    break;
	    case 6: exit(0);   
	    break;
	    default: printf("\nWrong choice!\n");       		          
	
	}
	
}

}

