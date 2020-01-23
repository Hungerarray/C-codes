#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//value storing node
typedef struct _node{
    struct _node *prev;
    long double value;
}node;

//responsible for creating the node
node *create();

//responsible for initializing node
node *input(long double, node *);

//resposible for computing
typedef long double (*funoperator) (long double, long double);
long double add(long double, long double);
long double sub(long double, long double);
long double prd(long double, long double);
long double divd(long double, long double);
funoperator (*operator[128]) (long double, long double) = { NULL };
void initializeOperatorArray();

//responsible for removing nodes
void del(node *);

//responsible for displaying current status of nodes
void display(node *);

int main()
{
    //initializing variables
    int set, sign;
    long double num;
    char *buff = calloc(1, 1024);
    if (buff == NULL){ exit(1); }
    char dummy;
    node *curr = NULL;
    initializeOperatorArray();

    //welcome message
    printf ("\n\nWelcome to RPN (Reverse Polish Notation) calculator!!\n");
    printf ("\nYou may enter only either number or operation (currently: + - * /) per input\n");
    printf ("to exit enter : quit\n\n");

    //start of calculation
    while (1){
        //resetting
        set = 0;
        sign = 1;

        //taking data
        printf ("Data: ");
        scanf(" ");
        fgets(buff, 1024, stdin);
        while ((dummy = *(buff + set)) == ' '){ set++; };
        
        //for exiting
        if(isalpha(*(buff + set))){
            if (strcmp(buff + set, "quit")){ break; }
            printf ("\nInvalid value please follow the instruction\n");
        }

        //for numbers
        if(*(buff + set) == '-'){ sign *= -1; }
        while ((dummy = *(buff + set)) == ' '){ set++; };
        if(isdigit(*(buff + set))){
            num = strtold(buff, NULL) * sign;
            node *temp = input(num, curr);
            curr = temp;
        }

        //for operations
        else if (ispunct((int)*(buff + set))){
            //get operation function address
            funoperator opp = operator[(int)*(buff + set)];
            //check for valid function operator
            if(opp == NULL){ printf ("\nInvalid operator\n"); }
            else{
                //find the two values from node
                long double num1, num2; 
                node *prev = curr->prev;
                //check for first node
                if(prev == NULL){ printf ("\nCannot operate on single function\n"); }
                else{
                    node *temp = curr->prev;
                    num1 = prev->value;
                    num2 = curr->value;
                    prev->value = opp(num1, num2);
                    del(curr);
                    curr = temp;
                }
            }
        }

        if(curr != NULL){ display(curr); }
    }

    //goodbye message
    printf ("\n\n Good Bye !! \n\n");

    return 0;
}

//responsible for creating the node
node *create(){
    node *temp = (node *)calloc(1, sizeof(node));
    if(temp == NULL){ exit(1); }
    return temp;
}

//responsible for initializing node
node *input(long double num, node *prev)
{
    node *curr = create();
    curr->prev = prev;
    curr->value = num;
    return curr;
}

//resposible for computing
long double add(long double a, long double b){ return a + b; }
long double sub(long double a, long double b){ return a - b; }
long double prd(long double a, long double b){ return a * b; }
long double divd(long double a, long double b){ return a / b; }
void initializeOperatorArray(){
    operator['+'] = add;
    operator['-'] = sub;
    operator['*'] = prd;
    operator['/'] = divd;
}

//respomsible for removing nodes
void del(node *remove)
{
    free(remove);
    return;
}

//responsible for displaying current status of nodes
void display(node *curr)
{
    node *temp = calloc(1, sizeof(node)), *temp2 = calloc(1, sizeof(node));
    temp = curr;
    while (1){
        if(temp->prev == NULL){ break; }
        temp2 = temp->prev;
        temp = temp2;
    }
    while(1){
        printf ("%Lg\n", temp->value);
        if(temp++ == curr){ break; } 
    }
    return;
}
