/**************************************
 * Hayden Miedema
 * 
 * Program 2 - CIS 452
 * ***********************************/

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>	
#include <stdbool.h>
#define MAX 20

//
//   Your other functions
//   and variables
//
//   should go here
//

void infixtoprefix(char infix[20], char prefix[20]);
void reverse(char array[30]);
char pop();
void push(char symbol);
int isOperator(char symbol);
int prcd(char symbol);
int top = -1;
char stack[MAX];
char expr[100];
void convert();

/* 
 * Start of Functions  
*/
float evaluate (const char* expr, bool immediate_result) {
  return 0.0;  /* replace it with your own */
}

void convert(){
  char infix[20], prefix[20], temp;
  strcpy(infix, expr);
  puts(infix);
  infixtoprefix(infix, prefix);
  reverse(prefix);
  puts((prefix));
}

int main() {
  //
  // you may add more code here
  //
  printf("Enter infix operation: ");
  fgets(*expr, 20, stdin);
  convert();
  printf("The expression in prefix is: %s", expr);  
  
  /* 8< 8< 8< begin-cut-here >8 >8 >8 */
  printf ("Test 1 %f\n", evaluate ("2.0 * 3.0", true));
  printf ("Test 2 %f\n", evaluate ("200.0 + 300.0", true));
  printf ("Test 3 %f\n", evaluate ("10.0 / 5.0", true));
  printf ("Test 4 %f\n", evaluate ("16.0 - 10.5", true));
  /* add more test of your own */
  /* 8< 8< 8< end-cut-here >8 >8 >8 */
  return 0;
}

/* Infix to Prefix Converter Utility
 * 
 */
//--------------------------------------------------------
void infixtoprefix(char infix[20], char prefix[20]) {
	int i, j = 0;
	char symbol;
	stack[++top] = '#';
	reverse(infix);
	for (i = 0; i < strlen(infix); i++) {
		symbol = infix[i];
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
	  return 1;
	break;
	default:
		return 0;
 // returns 0 if the symbol is other than given above
  }
 }


