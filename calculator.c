#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef double (*funoperator) (double , double);

void data(char *, double *);
double add(double, double);
double sub(double, double);
double prd(double, double);
double divd(double, double);
funoperator code(char);
double oppe(funoperator opp, double, double);

int main(){
    double num1, num2;
    char opcode, c;
    data("first", &num1);
    data("second", &num2);
    while (1){
        fprintf (stdout, "\nSelect your opcode ( + - * / ): ");
        fscanf (stdin, " %c", &opcode);
        while((c = getchar ()) != '\n');
        if(opcode == '+' || opcode == '-' || opcode == '*' || opcode == '/') { break; }
        fprintf (stdout, "\nError in taking operation");
    }
    fprintf (stdout, "\n%.6lg %c %.6lg = %lg", num1, opcode, num2, oppe(code(opcode), num1, num2));
    return 0;
}

void data(char *term, double *num){
    char *buff = (char *) malloc(1024);
    char *endptr;
    while(1){
        char dummy;
        int set = 0;
        int sign = 1;
        fprintf (stdout, "\nEnter %s number: ", term);
        fgets(buff, 1024, stdin);
        while((dummy = *(buff + set)) == ' '){ set++; }
        if (*(buff + set) == '-'){ sign *= -1; set++; }
        while((dummy = *(buff + set)) == ' '){ set++; }
        if (isdigit(*(buff + set))){
            *num = sign * strtod(buff + set, &endptr);
            break;
        }
        fprintf (stdout, "\nError in taking value\n");
    }
    free(buff);
    return ;
}
double add(double a, double b){ return a + b; }
double sub(double a, double b){ return a - b; }
double prd(double a, double b){ return a * b; }
double divd(double a, double b){ return a / b; }
double oppe(funoperator opp, double a, double b){ return opp(a, b); }
funoperator code(char opcode){
    switch (opcode){
        case '+': return add;
        case '-': return sub;
        case '*': return prd;
        case '/': return divd;
    }
    exit (1);
}
