#include <stdio.h>
#include <stdlib.h>

//declare structure Node
struct Node {
        int degree;
        int coefficient;

        struct Node *next;
        struct Node *prev;
};


//function of inputpoly
struct Node *inputpoly(void) {
        //To return of first node's address, i make head pointer variable.
        //(struct Node *) : default return value shape of malloc is void *, so I change this to fit struct Node *.
        struct Node *head = (struct Node *)malloc(sizeof(struct Node));
        //To make doubly linked list, i make nowNode pointer variable. I use this to connect next node's address.
        struct Node *nowNode = (struct Node*)malloc(sizeof(struct Node));
        int count =0; // to distinguish first node
        while (1) {
                int degree;
                int coefficient;
                printf("Input (degree) (coefficient):");
                scanf("%d %d", &degree, &coefficient);

                if (degree<0 && coefficient<0) {
                        printf("Done!\n");
                        break;
                }
                else if (degree<0 || coefficient<0) {
                        printf("wrong input. input again.\n");
                        continue;
                }
                else {
                        struct Node *node = (struct Node *)malloc(sizeof(struct Node)); //init struct variable.
                        node->degree = degree;
                        node->coefficient = coefficient;
                        if (count == 0) {
                                head=node;
                                count = count+1;
                                //nowNode = node;
                        }
                        else {
                                //nowNode->next = node;
                                //node->prev = nowNode;
                                //nowNode = node;
                                nowNode = head;
                                for(int i=0; i<count; i++) {
                                        if (nowNode->degree > node->degree) {
												if(i==0) {
                                                        nowNode->prev = node;
                                                        node->next = nowNode;
                                                        head = node;
                                                }
                                                else {
                                                        struct Node *prevNode = (struct Node *)malloc(sizeof(struct Node));
                                                        prevNode = nowNode->prev;
                                                        prevNode->next = node;
                                                        nowNode->prev = node;
                                                        node->prev = prevNode;
                                                        node->next = nowNode;
                                                        //head = node;
                                                        //free....?
                                                }
                                                count = count+1;
                                                break;
                                        }
                                        else if (nowNode->degree == node->degree) {
                                                nowNode->coefficient = nowNode->coefficient + node->coefficient;
                                                break;
                                        }
                                        else {
                                                if (nowNode->next == NULL) {
                                                        nowNode->next = node;
                                                        node->prev = nowNode;
                                                        count++;
                                                        break;
                                                }
                                                else {
                                                        nowNode = nowNode->next;
                                                }
                                        }
                                }
                        }
                }
        }
        return head;

        //To prevent memory leak, release memory
        free(nowNode);
        free(head);
}


//function of printNode
void printNode(struct Node *inp) {
        //I make nowNode, the pointer variable. I use this to move Node through doubly linked list
        struct Node *nowNode = (struct Node *)malloc(sizeof(struct Node));
        nowNode = inp;
        while (1) {
                //Since the number of N is more than one, we first write it down.
                //To check user's input, no exception that coefficient is zero.
                if (nowNode->degree == 1) { //1st term case
                        printf("%dx",nowNode->coefficient);
                }
                else if (nowNode->degree == 0) {
                        printf("%d",nowNode->coefficient);
                }
                else {
                        printf("%dx^%d",nowNode->coefficient,nowNode->degree);
                }
                if (nowNode->next != NULL) {
                        //if there is address of nowNode's next, write +, and transfer the address of nowNode to the address of nowNode's next.
                        printf("+");
                        nowNode = nowNode->next;
                }
                else {
                        //if there are no address of nowNode's next, get out while function
                        break;
                }
        }
        printf("\n");


        free(nowNode);
}


struct Node *multiply(struct Node *a, struct Node *b) {
        struct Node *multiplyNode = (struct Node *)malloc(sizeof(struct Node));
        struct Node *nowNode = (struct Node *)malloc(sizeof(struct Node));
        struct Node *aNode = (struct Node *)malloc(sizeof(struct Node));
        struct Node *bNode = (struct Node *)malloc(sizeof(struct Node));
        aNode = a;
        //bNode = b;
        int count = 0;
        while (1) {
                bNode = b;
                while(1) {
                        struct Node *node = (struct Node *)malloc(sizeof(struct Node));
                        node->degree = aNode->degree + bNode->degree;
                        node->coefficient = aNode->coefficient * bNode->coefficient;
                        if (count==0) {
                                multiplyNode = node;
                                count++;
                        }
                        else {
                                nowNode = multiplyNode;
                                for(int i=0; i<count; i++) {
                                        if(nowNode->degree > node->degree) {
                                                if (i==0) {
                                                        nowNode->prev = node;
                                                        node->next = nowNode;
                                                        multiplyNode = node;
                                                }
                                                else {
                                                        nowNode->prev->next = node;
                                                        nowNode->prev = node;
                                                        node->prev = nowNode->prev;
                                                        node->next = nowNode;
                                                }
                                                count++;
                                                break;
                                        }
										else if (nowNode->degree == node->degree) {
                                                nowNode->coefficient = nowNode->coefficient + node->coefficient;
                                                break;
                                        }
                                        else {
                                                if(nowNode->next == NULL) {
                                                        nowNode->next = node;
                                                        node->prev = nowNode;
                                                        count++;
                                                        break;
                                                }
                                                else {
                                                        nowNode = nowNode->next;
                                                }
                                        }
                                }
                        }
                        if (bNode->next == NULL) {
                                break;
                        }
                        else {
                                bNode = bNode->next;
                        }
                }
                if (aNode->next == NULL) {
                        break;
                }
                else {
                        aNode = aNode->next;
                }
        }
        return multiplyNode;

        free(multiplyNode);
        free(nowNode);
        free(aNode);
        free(bNode);
}


int main () {
        while(1) {
                int number;
                printf("--------------------------------------------------------\n");
                printf("what do you want?\n");
                printf("1. make one polynomial and print\n");
                printf("2. make two polynomial and multiply and print\n");
                printf("any number : exit program\n");
                printf("choose number : ");
                scanf("%d",&number);
                if (number == 1) {
                        printNode(inputpoly());
                }
                else if (number ==2) {
                        struct Node *node1 = (struct Node *)malloc(sizeof(struct Node));
                        struct Node *node2 = (struct Node *)malloc(sizeof(struct Node));
                        printf("make polynomial1\n");
                        node1 = inputpoly();
                        printf("make polynomial2\n");
                        node2 = inputpoly();
                        printNode(multiply(node1, node2));

                        free(node1);
                        free(node2);
                }
                else {
                        break;
                }
        }
        return 0;
};











// #include <stdio.h>
// #include <stdlib.h>

// //Declare NODE struct
// struct NODE
// {
// 	struct NODE *prev;
// 	int coef;
// 	int deg;
// 	struct NODE *next;
// };

// //head, tail set global variable
// struct NODE *head = NULL;
// struct NODE *tail = NULL;

// //Function for making new node
// struct NODE *makenode(int coefficient, int degree)
// {
// 	// Allocate memory as much as size of NODE to node
// 	struct NODE *node = (struct NODE *)malloc(sizeof(struct NODE));

// 	// Follow the instructions, create an empty node.
// 	node->prev = NULL;
// 	node->coef = coefficient;
// 	node->deg = degree;
// 	node->next = NULL;
// 	return node;

// 	//Free allocated memory for node
// 	free(node);
// }

// //Function for initialize Doubly linked list
// void init()
// {
// 	// Allocate memory as much as size of NODE to head, tail
// 	head = (struct NODE *)malloc(sizeof(struct NODE));
// 	tail = (struct NODE *)malloc(sizeof(struct NODE));

// 	//Allocate 0 to value of head, tail that is useless
// 	head->coef = 0;
// 	head->deg = 0;
// 	tail->coef = 0;
// 	tail->deg = 0;

// 	//Follow the instructions, allocate prev/next of head and tail
// 	head->next = tail;
// 	head->prev = NULL;
// 	tail->next = NULL;
// 	tail->prev = head;
// }

// //Function for input new node to existing linked list
// void input(int coefficient, int degree)
// {
// 	//Allocate values(coefficient,degree) to newnode with return values by invoking Makenode function
// 	struct NODE *newnode = makenode(coefficient, degree);

// 	//Make a pointer to find a suitable place for the new node
// 	struct NODE *p;

// 	//pointer will move tail to head
// 	p = tail;

// 	while (1)
// 	{
// 		//When the pointer points to the tail, one space forward
// 		if (p->next == NULL)
// 		{
// 			p = p->prev;
// 		}
// 		else
// 		{
// 			//Move one space forward until (degree of pointer <= degree of newnode || pointer points to the head)
// 			if (p->deg > newnode->deg && p->prev->prev != NULL)
// 			{
// 				p = p->prev;
// 			}
// 			else
// 			{
// 				//Insert newnode between what the pointer points and what is behind it
// 				newnode->prev = p;
// 				newnode->next = p->next;
// 				p->next->prev = newnode;
// 				p->next = newnode;
// 				break;
// 			}
// 		}
// 	}
// }


// //Function for userinput
// struct NODE *inputpoly()
// {
// 	//Init Doubly linked list for userinput
// 	init();

// 	while (1)
// 	{
// 		//Input each degree and coefficient by user
// 		int degree_input;
// 		int coefficient_input;
// 		printf("Input (degree) (coefficient):");
// 		scanf("%d %d", &degree_input, &coefficient_input);

// 		//Folow the instructions, break the loop when input values are all minus
// 		if (coefficient_input < 0 && degree_input < 0)
// 		{
// 			printf("Done!\n");
// 			break;
// 		}

// 		//Follow the instructions, ignore now loop and proceed to the next loop when only one of values is negative.
// 		else if (coefficient_input < 0 || degree_input < 0)
// 		{
// 			continue;
// 		}

// 		//When the input condition is met, values are entered in the Doubly linked list.
// 		else
// 		{
// 			input(coefficient_input, degree_input);
// 		}
// 	}

// 	return head;

// 	//Free allocated memory for head/tail
// 	free(head);
// 	free(tail);
// }


// //Function multiply two polynomial
// struct NODE *multiply(struct NODE *a, struct NODE *b)
// {
// 	//Create two pointer and store the
// 	//address of a and b polynomial
// 	struct NODE *p1;
// 	struct NODE *p2;
// 	// Allocate memory as much as size of NODE to p1, p2
// 	p1 = (struct NODE *)malloc(sizeof(struct NODE));
// 	p2 = (struct NODE *)malloc(sizeof(struct NODE));

// 	p1 = a->next;
// 	p2 = b->next;
// 	int deg, coef;

// 	// Init new Doubly linked list for result of multiply
// 	init();
// 	while (p1->next != NULL)
// 	{
// 		//Move the 2nd pointer to the
// 		//starting point of 2nd polynomial
// 		p2 = b->next;
// 		while (p2->next != NULL)
// 		{
// 			//Multiply the coefficient of both polynomials
// 			// and store it in coef
// 			coef = p1->coef * p2->coef;

// 			//Add the degree of both polynomials
// 			// and store it in deg
// 			deg = p1->deg + p2->deg;

// 			//Invoke input function to create
// 			//a new node by passing two parameters
// 			input(coef, deg);

// 			//Move the 2nd pointer
// 			// to get its next node
// 			p2 = p2->next;
// 		}

// 		//Move the 1st pointer
// 		// to get its next node
// 		p1 = p1->next;
// 	}

// 	return head;

// 	//Free allocated memory for head/tail
// 	free(head);
// 	free(tail);

// 	//Free allocated memory for p1,p2
// 	free(p1);
// 	free(p2);
	
// }


// //Function for print polnomial from doubly linked list
// void printNode(struct NODE *head)
// {
// 	// Sum nodes that have same degree
// 	struct NODE *ptr;

// 	//Allocate memory as much as size of NODE to ptr
// 	ptr = (struct NODE *)malloc(sizeof(struct NODE));

// 	//ptr points the first node which represents the result of multiply
// 	ptr = head->next;

// 	//ptr will move until next is tail
// 	while (ptr->next != NULL)
// 	{
// 		//Repeat until the current degree and the next degree are different
// 		while (ptr->deg == ptr->next->deg)
// 		{
// 			//Sum the nodes that have same degree
// 			ptr->coef = ptr->coef + ptr->next->coef;
// 			//Change each pointer of nodes
// 			ptr->next = ptr->next->next;
// 			ptr->next->prev = ptr;
// 		}
// 		//Move the pointer to get next node
// 		ptr = ptr->next;
// 	}

// 	//Pointer will move head to tail
// 	struct NODE *p;
// 	p = head->next;

// 	//Loop until the pointer moves to before step 2 of the tail
// 	while (p->next != tail)
// 	{
// 		printf("%dx^%d + ", p->coef, p->deg);
// 		p = p->next;
// 	}

// 	//Print node that is before of the tail
// 	printf("%dx^%d\n", p->coef, p->deg);

// 	//Free allocated memory for ptr
// 	free(ptr);
// }

// //Main function
// int main()
// {
// 	while (1)
// 	{
// 		int mode_input;
// 		printf("mode를 입력하세요-- ex) 1\n");
// 		printf("--(1)-입력--(2)-출력--(3)-곱셈--(4)-종료--");
// 		scanf("%d", &mode_input);
// 		if (mode_input == 1)
// 		{
// 			//input mode
// 			inputpoly();
// 		}
// 		else if (mode_input == 2)
// 		{
// 			//print mode
// 			printNode(inputpoly());
// 		}
// 		else if (mode_input == 3)
// 		{
// 			//multiply mode
// 			struct NODE *a;
// 			struct NODE *b;
// 			a = (struct NODE *)malloc(sizeof(struct NODE));
// 			b = (struct NODE *)malloc(sizeof(struct NODE));

// 			a = inputpoly();
// 			b = inputpoly();
// 			printNode(multiply(a, b));

// 			free(a);
// 			free(b);
// 		}
// 		else if (mode_input == 4)
// 		{
// 			//End mode
// 			break;
// 		}
// 	}
// 	return 0;
// }
