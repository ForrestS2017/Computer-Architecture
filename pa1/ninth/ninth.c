#include <stdlib.h>
#include <stdio.h>

typedef struct Node{
	int data;
	struct Node * left;
	struct Node * right;
} Node;

void Insert(int len);     // Returns node before one you want
void FreeTable();
void Operate(char op, int val);
void Insert(int val);
Node* Search(int val);
Node* Delete(Node* node, int val);
void ReadFile(int argc, char** argv);  // Reads and inserts or deletes
Node * MakeNode(int val);
Node *root = NULL;



int main(int argc, char** argv)
{
	 if(argc != 2)
	 {
	  printf("error");
	  exit(0);
	 }

	 ReadFile(argc, argv);

	 FreeTable();

}

void ReadFile(int argc, char** argv)
{
	int r, val;
 	 char op;

    FILE* fp = NULL;
    char* filename = NULL;

    filename = argv[1];

    fp = fopen(filename, "r");
    if(fp == NULL)
    {
        printf("error: File does not exist\n");
        exit(0);
    }

    r = fscanf(fp, "%c\t", &op);     				// Get operation
    if(r != 1){printf("0\n"); exit(0);}  			// Error throw

    while(r != EOF)
    {
    	fscanf(fp, "%d\n", &val);
    	Operate(op, val);
    	r = fscanf(fp, "%c\t", &op);

    }
    fclose(fp);

}

void Operate(char op, int val)
{

	switch(op)
	{
	case 'i':
		Insert(val);
		return;
	case 's':
		Search(val);
		return;
	case 'd':
		Delete(root, val);
		return;
	}

}

void Insert(int val)
{
	//Node * insert = NULL;
	int cnt;
	cnt = 1;
	// Case 1: Empty tree
	if(root == NULL)
	{
		Node * insert = (Node*)malloc(sizeof(Node));
		insert = MakeNode(val);
		root = insert;
		printf("inserted 1\n");
		return;
	}
	else
	{
		Node * temp = root;

		while(temp){
			// Val is smaller, go left
			if(temp->data > val)
			{
				if(temp->left != NULL)
				{
					temp = temp->left;
					cnt++;
					continue;
				}
				Node * insert = (Node*) malloc(sizeof(Node));
				insert = MakeNode(val);;
				temp->left = insert;
				cnt++;
				printf("inserted %d\n", cnt);
				return;
			}
			// Val is bigger, go right
			else if(temp->data < val)
			{
				if(temp->right != NULL)
				{
					temp = temp->right;
					cnt++;
					continue;
				}
				Node * insert = (Node*) malloc(sizeof(Node));
				insert = MakeNode(val);
				temp->right = insert;
				cnt++;
				printf("inserted %d\n", cnt);
				return;
			}
			// Val is present, print duplicate
			else if(temp->data == val)
			{
				printf("duplicate\n");
				return;
			}
		}
	}

}

Node* Search(int val)
{
	int cnt;
	cnt = 1;
		// Case 1: Empty tree
	if(root == NULL)
	{
		printf("absent %d\n", val);
		return NULL;
	}
	else
	{
		Node * temp = root;
			while(temp != NULL){
			// Val is smaller, go left
			if(temp->data > val)
			{
				temp = temp->left;
				cnt++;
				continue;
			}
			// Val is bigger, go right
			else if(temp->data < val)
			{
				temp = temp->right;
				cnt++;
				continue;
			}
			// Val is present, print duplicate
			else if(temp->data == val)
			{
				printf("present %d\n", cnt);
				return temp;
			}
		}
	}
	printf("absent\n");
	return NULL;
}

Node* Delete(Node* target, int val)
{
	printf("Delete: %d\n", val);

	// Case 1: empty tree
	if(!target)
	{
		printf("fail\n");
		return NULL;
	}

	//If the targeted value is smaller or larger than the current value
	if( target->data < val)
	{
		target->right = Delete(target->right, val);
	}
	else if( target->data > val)
	{
		target->left = Delete(target->left, val);
	}
	// Otherwise, if we found it
	else
	{
		//	ONE child
		if(target->right == NULL)
		{
			Node * temp = target->right;
			free(target);
			return temp;
		}
		else if(target->left == NULL)
		{
			Node * temp = target->left;
			free(target);
			return temp;
		}

		// TWO child, find inorder successor
		Node * temp = target->right;
		while(temp->left != NULL) temp = temp->left;
		//Swap the values
		target->data = temp->data;
		// Delete old inorder successor
		target->right = Delete(target->right, temp->data);
	}

	return target;
}


Node * MakeNode(int val)
{
	Node * newNode = (Node*) malloc(sizeof(Node));
	newNode->data = val;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

void FreeTable()
{

}


/*
Node* Delete(Node* target, int val)
{
	printf("Delete: %d\n", val);

	// Case 1: empty tree
	if(!target)
	{
		printf("fail\n");
		return NULL;
	}

	target = Search(val);
	// Case 2: there's only one side
	if(target->left == NULL)
	{
		Node * temp = target;
		target = target->right;
		if(temp == target) root = target;
		free(temp);
		return NULL;
	}
	else if(target->right == NULL)
	{
		Node * temp = target;
		target = target->left;
		if(temp == target) root = target;
		free(target);
		return NULL;
	}

	Node * temp = target->right;
	while(temp->left != NULL)
	{
		temp = temp->left;
	}

	target->data = temp->data;

	target->right = Delete(target->right, temp->data);


	return root;
}*/
