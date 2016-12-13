#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
# define MAX 20

void infixtoprefix(char infix[20], char prefix[20]);
void reverse(char array[30]);
char pop();
void push(char symbol);
int isOperator(char symbol);
int prcd(char symbol);
int top = -1;
char stack[MAX];
char expr[20];
int numOps = 0;
void convert();
void evalExpr(char prefix[20]);
pid_t pid;

int main() {
  printf("Enter infix operation: ");
  fgets(expr, 20, stdin);
  convert();

  return 0;
}

void evalExpr(char prefix[20]){
	int i, j;
	char input[20][20];
	float result = 0.0;
	for(i=0; i< sizeof(prefix); i++){
	  for(j=0; j<sizeof(prefix); j++){
	    if(isspace(prefix[i])){
	      continue;
	    }else if(prefix[i] == "\0"){
	      continue;
	    }else{
	      input[i][j] = prefix[i];
	    }
	  
	}
	}
	if((pid = fork()) < 0){
		perror("fork failed\n");
		exit(1);
	}
	else if(!pid){
		switch(expr[0]){
			case '+':
			result = atof(a) + atof(b);
		}
	}
	//if(pid)
		printf("Result: %f\n", result);
}

void convert(){
      char infix[20], prefix[20], temp;
        strcpy(infix, expr);
	puts(infix);
	infixtoprefix(infix, prefix);
	reverse(prefix);
	strcpy(expr, prefix);
	puts((prefix));
	evalExpr(prefix);
}

//--------------------------------------------------------
void infixtoprefix(char infix[20], char prefix[20]) {
	int i, j = 0;
	char symbol;
	stack[++top] = '#';
	reverse(infix);
	for (i = 0; i < strlen(infix); i++) {
		symbol = infix[i];
		if (symbol == ' ')
		  continue;
	if (isOperator(symbol) == 0) {
		prefix[j] = symbol;
		j++;
	} else {
	if (symbol == ')') {
		push(symbol);
	} else if (symbol == '(') {
		while (stack[top] != ')') {
			prefix[j] = pop();
			j++;
		}
		pop();
  }else{
    if (prcd(stack[top]) <= prcd(symbol)) {
		push(symbol);
    }else{
      while (prcd(stack[top]) >= prcd(symbol)) {
        prefix[j] = pop();
        j++;
      }
      push(symbol);
	}
   }
  }
 }
  while (stack[top] != '#') {
    prefix[j] = pop();
    j++;
   }
  prefix[j] = '\0';
 }
 
 ////--------------------------------------------------------
 void reverse(char array[30]) {
 // for reverse of the given expression
	int i, j;
	char temp[100];
	for (i = strlen(array) - 1, j = 0; i + 1 != 0; --i, ++j) {
		temp[j] = array[i];
	}
	temp[j] = '\0';
	strcpy(array, temp);//copying temp array to array
  // return array;
 }
 //--------------------------------
char pop() {
	char a;
	a = stack[top];
	top--;
  return a;
 }
 //----------------------------------
void push(char symbol) {
 top++;
 stack[top] = symbol; 
 }
 //------------------------------------------
 
int prcd(char symbol) {
 // returns the value that helps in the precedence
      switch (symbol) {
	case '+':
	case '-':
	return 2;
      break;
	case '*':
	case '/':
	return 4;
      break;
	case '$':
	case '^':
	return 6;
      break;
	case '#':
	case '(':
	case ')':
	return 1;
      break;
  } 
}
//-------------------------------------------------
int isOperator(char symbol) {
	switch (symbol) {
	  case '+':
	  case '-':
	  case '*':
	  case '/':
	  case '^':
	  case '$':
	  case '&':
	  case '(':
	  case ')':
	  numOps++;
	  return 1;
	break;
	default:
		return 0;
 // returns 0 if the symbol is other than given above
  }
 }


