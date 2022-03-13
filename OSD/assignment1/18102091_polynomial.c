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
		
		//Processing the input value, which means the end.
		if (degree<0 && coefficient<0) {
			printf("Done!\n");
			break;
		}
		//Invalid input value processing.
		else if (degree<0 || coefficient<0) {
			printf("wrong input. input again.\n");
			continue;
		}
		else {
			struct Node *node = (struct Node *)malloc(sizeof(struct Node)); //init struct variable.
			node->degree = degree;
			node->coefficient = coefficient;
			//count==0 mean node is first input. so add it and save it on head.
			if (count == 0) {
				head=node;
				count = count+1;
			}
			else {
				//save nowNode as the initial address head. because we need to adjust the position of Node according to degree.
				nowNode = head;
				for(int i=0; i<count; i++) {
					if (nowNode->degree > node->degree) {
						//if node have a lower degree than head, head changes.
						if(i==0) {
							nowNode->prev = node;
							node->next = nowNode;
							head = node;
						}
						else {
							//otherwise, connect it using a new storage memory because it has to be inserted between Nodes.
							struct Node *prevNode = (struct Node *)malloc(sizeof(struct Node));
							prevNode = nowNode->prev;
							prevNode->next = node;
							nowNode->prev = node;
							node->prev = prevNode;
							node->next = nowNode;
							
						}
						count = count+1;
						break;
					}
					//if degree is the same, add the nodes(dont make new node)
					else if (nowNode->degree == node->degree) {
						nowNode->coefficient = nowNode->coefficient + node->coefficient;
						break;
					}
					else {
						//if degree of new node is greater than degree of all node, add it to the last.
						if (nowNode->next == NULL) {
							nowNode->next = node;
							node->prev = nowNode;
							count++;
							break;
						}
						//if it's not the last node, check the next node.
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
		//If d is zero, it's a constant, so processing the output.
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

//function of multiply two polynimial
struct Node *multiply(struct Node *a, struct Node *b) {
	struct Node *multiplyNode = (struct Node *)malloc(sizeof(struct Node));
	struct Node *nowNode = (struct Node *)malloc(sizeof(struct Node));
	//create two pointer which stored a and b address each other.
	struct Node *aNode = (struct Node *)malloc(sizeof(struct Node));
	struct Node *bNode = (struct Node *)malloc(sizeof(struct Node));
	aNode = a;
	
	int count = 0;
	while (1) {
		//For the law of distribution of multiplication, declare bNode=b in while().
		bNode = b;
		while(1) {
			//it's the same as the principle of inputpoly.
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










