#define _GNU_SOURCE
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <assert.h>

// Input struct
typedef enum InputState_t { NUMBER, OPERAND, DISPLAY, DELETE, EXIT, INVALID } InputState_t;

typedef struct Input_t {
  InputState_t state;
  union {
    double value;
    char operand;
  };
} Input_t;

// Double linked list
typedef struct Node_t {
  double value;
  struct Node_t *prev, *next;
} Node_t;

typedef struct List_t {
  Node_t *head, *tail;
  uintmax_t count;
} List_t;

// Helper functions
Node_t *createNode(double value) {
  Node_t *ret = (Node_t *) malloc(sizeof(Node_t));
  assert(ret);
  *ret = (Node_t) {
    .value = value
  };
  return ret;
};

void deleteNode(Node_t *node) {
  free(node);
}

void pushList(List_t *list, Node_t *node) {
  if(list->count == 0) {
    *list = (List_t) {
      .head = node,
      .tail = node,
      .count = 1
    };
    return ;
  }

  list->tail->next = node;
  node->prev = list->tail;
  list->tail = node;
  list->count++;
}

Node_t *popList(List_t *list) {
  if (list->count == 0) {
    return NULL;
  }
  else if (list->count == 1) {
    Node_t *curr = list->tail;
    *list = (List_t){};
    return curr;
  }

  Node_t *curr = list->tail;
  list->tail = curr->prev;
  list->tail->next = NULL;
  list->count--;
  
  return curr;
}

void displayNode(const Node_t *node) {
  printf("\t%g\n", node->value);
}

void displayList(const List_t *list) {
  printf ("Table of current values: \n");
  for(Node_t *curr = list->head; curr != NULL; curr = curr->next) {
    displayNode(curr);
  }
}

// computational functions
double add(double x, double y) {
  return x + y;
}
double sub(double x, double y) {
  return x - y;
}
double mult(double x, double y) {
  return x * y;
}
double divd(double x, double y) {
  return x / y;
}

void operation(List_t *list, char operand) {
  if(list->count < 2) {
    printf("Not enough numbers to perform operation on.\n");
    return ;
  }

  Node_t *curr = popList(list);
  Node_t *prev = popList(list);
  double total;
  switch(operand) {
    case '+':
      total = add(prev->value, curr->value);
      pushList(list, createNode(total));
      break;
    case '-':
      total = sub(prev->value, curr->value);
      pushList(list, createNode(total));
      break;
    case '*':
      total = mult(prev->value, curr->value);
      pushList(list, createNode(total)); 
      break;
    case '/':
      if(curr->value == 0) {
        printf("Math Error: division by zero");
        pushList(list, prev);
        pushList(list, curr);
        return ;
      }
      total = divd(prev->value, curr->value);
      pushList(list, createNode(total));
      break;
    default:
      pushList(list, prev);
      pushList(list, curr);
      printf("Invalid operand\n");
      return ;
  }

  free(prev);
  free(curr);
}

void operate(Input_t input) {
  static List_t list;

  switch(input.state) {
    case NUMBER:
      Node_t *newValue = createNode(input.value);
      pushList(&list, newValue);
      break;
    case DELETE:
      if(list.count == 0) {
        printf("No more elements!!\n");
        break;
      }
      popList(&list);
      break;
    case INVALID:
      printf("Invalid operation \n");
      break;
    case DISPLAY:
      displayList(&list);
      break;
    case OPERAND:
      operation(&list, input.operand);
      break;
    default:
      printf("Invalid State\n");
      break;
  }
}

void printRegError(int errcode, const regex_t *regex) {
  size_t total = regerror(errcode, regex, NULL, 0) + 1;
  char buffer[total];
  regerror(errcode, regex, buffer, total);
  printf("%s\n", buffer);
}

bool isNumber(const char *input) {
  static regex_t numReg;
  static bool compiled = false;
  static const char *regex = "^\\s*[+-]?([0-9]+\\.?[0-9]*|\\.[0-9]+)\\s*$";

  if(!compiled) {
    int err = regcomp(&numReg, regex, REG_EXTENDED);
    if(err) {
      printRegError(err, &numReg);
    }
    assert(err == 0);
  } 
  return regexec(&numReg, input, 0, NULL, 0) == 0;
}

bool isOperand(const char *input) {
  static regex_t operandReg;
  static bool compiled = false;
  static const char *regex = "^\\s*[+*/-]\\s*$";

  if(!compiled) {
    int err = regcomp(&operandReg, regex, REG_EXTENDED);
    if(err) {
      printRegError(err, &operandReg);
    }
    assert(err == 0);
  } 
  return regexec(&operandReg, input, 0, NULL, 0) == 0;
}

bool isDisplay(const char *input) {
  static regex_t displayReg;
  static bool compiled = false;
  static const char *regex = "^\\s*display\\s*$";

  if(!compiled) {
    int err = regcomp(&displayReg, regex, REG_EXTENDED | REG_ICASE);
    if(err) {
      printRegError(err, &displayReg);
    }
    assert(err == 0);
  } 
  return regexec(&displayReg, input, 0, NULL, 0) == 0;
}

bool isDelete(const char *input) {
  static regex_t deleteReg;
  static bool compiled = false;
  static const char *regex = "^\\s*delete\\s*$";

  if(!compiled) {
    int err = regcomp(&deleteReg, regex, REG_EXTENDED | REG_ICASE);
    if(err) {
      printRegError(err, &deleteReg);
    }
    assert(err == 0);
  } 
  return regexec(&deleteReg, input, 0, NULL, 0) == 0;
}

bool isExit(const char *input) {
  static regex_t exitReg;
  static bool compiled = false;
  static const char *regex = "^\\s*exit\\s*$";

  if(!compiled) {
    int err = regcomp(&exitReg, regex, REG_EXTENDED | REG_ICASE);
    if(err) {
      printRegError(err, &exitReg);
    }
    assert(err == 0);
  } 
  return regexec(&exitReg, input, 0, NULL, 0) == 0;
}

char getOperand(const char *input) {
  char curr;
  const char *temp = input;
  do {
    curr = *(temp++);
  }while(isspace(curr));
  return curr;
}

Input_t getInput() {
  char *userInput = NULL;
  size_t len;
  getline(&userInput, &len, stdin);

  Input_t ret;
  if(isNumber(userInput)) {
    ret = (Input_t) {
      .state = NUMBER,
      .value = strtod(userInput, NULL)
    };
  } else if (isOperand(userInput)) {
    ret = (Input_t) {
      .state = OPERAND,
      .operand = getOperand(userInput)
     };
  } else if (isDisplay(userInput)) {
    ret = (Input_t) {
      .state = DISPLAY
    };
  } else if (isDelete(userInput)) {
    ret = (Input_t) {
      .state = DELETE
    };
  } else if (isExit(userInput)) {
    ret = (Input_t) {
      .state = EXIT
    };
  } else {
    ret = (Input_t) {
      .state = INVALID
    };
  }
  
  free(userInput);
  return ret;
}

void printMenu() {
  printf("Your input\n");
  printf("\t1. Number\n");
  printf("\t2. Operand (+,-,*,/)\n");
  printf("\t3. <display>, to display the current stack layout\n");
  printf("\t4. <delete>, to delete previous number\n");
  printf("\t5. <exit>, to quit the program\n");
}

void mainLoop() {
  while(true) {
    printMenu();
    Input_t inp = getInput();
    if(inp.state == EXIT) {
      break;
    }
    operate(inp);
  }
}

int main() {
  printf ("\n\nWelcome to RPN (Reverse Polish Notation) calculator!!\n");
  mainLoop(); 
  printf ("\n\nGood Bye !! \n\n");
  return EXIT_SUCCESS;
}