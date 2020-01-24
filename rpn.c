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
node *input(long double x, node *);

//resposible for computing
typedef long double (*funoperator) (long double, long double);
long double add(long double x, long double y);
long double sub(long double x, long double y);
long double prd(long double x, long double y);
long double divd(long double x, long double y);
long double (*operator[128]) (long double, long double);
void initializeOperatorArray();

//responsible for removing nodes
void del(node *);

//responsible for displaying current status of nodes
void display(node *, int);

int main()
{
    //initializing variables
    int set, sign, cursor, flag, flag1, total = 0;
    long double num;
    char *buff = calloc(1, 1024);
    if (buff == NULL){ exit(1); }
    char dummy;
    char *test = calloc(1, 5);
    node *curr = NULL;
    initializeOperatorArray();

    //welcome message
    printf ("\n\nWelcome to RPN (Reverse Polish Notation) calculator!!\n");
    printf ("\nYou may enter only either number or operation (currently: + - * /) per input\n");
    printf ("\nto remove the last value enter : del \nto exit enter : quit\n\n");

    //start of calculation
    while (1){
        //resetting
        set = 0;
        sign = 1;
        cursor = 0;
        flag = 1;
        flag1 = 1;
        *test = '\0';

        //taking data
        printf ("\nData: ");
        scanf(" ");
        fgets(buff, 1024, stdin);
        while ((dummy = *(buff + set)) == ' '){ set++; };
        
        //for exiting
        while(isalpha(*(buff + set + cursor))){
            *(test + cursor) = *(buff + set + cursor);
            cursor++;
            *(test + cursor) = '\0';
            if(!strcmp(test, "quit")){ flag1 = 0; flag = 0; }
            if(!strcmp(test, "del")){ 
                flag1 = 0; 
                if(curr != NULL){
                    node *temp = curr->prev;
                    del(curr);
                    curr = temp;
                    total--;
                }
                else{
                    printf ("\nCan't delete anymore\n");
                }
             }
            if(cursor == 4){ break; }
        }
        set += cursor;
        if(!flag){ break; }
        if(*test != '\0' && flag1){ printf ("\nInvalid value please follow the instruction\n"); }

        //for numbers
        if(*(buff + set) == '-'){ sign *= -1; set++; }
        while ((dummy = *(buff + set)) == ' '){ set++; };
        if(isdigit(*(buff + set))){
            num = sign * strtold(buff + set, NULL);
            node *temp = input(num, curr);
            curr = temp;
            total++;
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
                    if (num2 == 0 && opp == divd){ 
                        printf ("\nCan't divide by zero\n"); 
                        goto next; 
                    }
                    prev->value = opp(num1, num2);
                    del(curr);
                    curr = temp;
                    total--; 
                    
                }
            }
        }

        next: if(curr != NULL){ display(curr, total); }
    }

    //goodbye message
    printf ("\n\nGood Bye !! \n\n");

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
void display(node *curr, int total)
{
    int set = 0;
    node **table = calloc(total, sizeof(node *));
    if(curr != NULL){
        while(1){
            table[set] = curr;
            if(curr->prev == NULL){ break; }
            curr = curr->prev;
            set++;
        }
        printf ("Table of current values: \n");
        for( ; set >= 0; set--){
            printf ("\t%Lg\n", table[set]->value);
        }
    }
    return;
}
