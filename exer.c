#include <stdio.h>
#include <string.h>

int palin(char [], int, int);
void copy(char [], const char dummy[], int, int);

int main ()
{
    char text[100];

    printf ("\nEnter any text: ");
    scanf (" %s", text);

    int len = strlen(text);
    char curr_max[len];
    unsigned count, count1, curr_max_len = 0, max_len = 0;

    for (count = 0; count < len; count++){
        for (count1 = len - 1; count1 >= count; count1--){
            printf ("\ncount: %d | count1: %d", count, count1);
            break;
                if(palin(text, count, count1)){
                    max_len = count1 - count;
                    if(curr_max_len == 0 || curr_max_len < max_len){ 
                        curr_max_len = max_len;
                        copy(curr_max, text, count1, count); 
                        printf ("%s", curr_max);    
                    }
                }
        }
    }
    printf (" %s", curr_max);
    
    return 0;
}

int palin(char test[], int strt, int end){
    unsigned count, count1, loop = (strt + end)/2;

    for(count = strt, count1 = end; count <= loop; count++, count1--){
        if(test[count] != test[count1]){
            return 0;
        }
    }

    return 1;
}

void copy(char dest[], const char src[], int strt, int end){
    unsigned count;
    for ( count = 0; count + strt <= end; count++){
        dest[count] = src[count + strt];
    }
    dest[count] = '\0';
    return ;
}