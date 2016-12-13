/************************************************************
 * Program 2 - Parallel Evaluation of Arithmetic Expressions
 * CIS 452
 *
 * By: Hayden Miedema
 ***********************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>


/* Structure variables for nodes and tree */
typedef struct Node{
    char *data ;
    int new_data;
    struct Node * left;
    struct Node * right;
    struct Node * prev;
}Node;

typedef struct{
    Node front;
    int elements;
}Tree;

/* Method Declarations */
void eval(char[], bool);
int isOperator(char symbol);
float evaluate_numbers(float, float, char);
Tree make_tree(char[][20], int);

/* Variables for two pids from left and right after fork */
int pid_left;
int pid_right;



/*
 * Main method used for testing the eval process
 */
int main(){
    printf ("Test 1:\n");
    eval ("2.0 * 3.0\n", true);
    printf ("Test 2:\n");
    eval ("200.0 + 300.0\n", true);
    printf ("Test 3:\n");
    eval ("10.0 / 5.0\n", true);
    printf ("Test 4:\n");
    eval ("16.0 - 10.5", true);
    printf("\n");
    eval("2.0 * 2.0 + 5.0 - 6.0 + 4.0 * 5.0 + 3.0 * 2.0 + 4.0 * 4.0 + 4.0 * 6.0 + 2.0 - 5.0 + 6.0", true);
    printf("\n");
	eval("4.0 - 80.0 / 3.0", true);
    printf("\n");
	eval("2 + 3 * 4 - 5 / 6", true);
    printf("\n");
	eval("2 + 3 * 4 - 5 / 6", false);

	
	return 0;
}

/*
 * Signal method for holding before sending interrupt
 * Used to view process tree during execution
 */

void call_signal(){
    if(pid_right != 0){
        kill(pid_right, SIGINT);
    }
    if(pid_left != 0){
        kill(pid_left, SIGINT);
    }
}

/*
 * Postfix to infix evaluation helper. Parses through the expression string and forks
 * when encountering an operator. At the end of this method, evaluate_numbers get where
 * the evaluation of the processes occur.
 */
char *eval_helper(Tree tree, Node *node, char op, bool b){
    signal(SIGINT, call_signal);
	int fd_left[4], fd_right[4], left_wait = 0, right_wait = 0;
	char first[15], second[15];
	
	// get character from left node of tree
	char ch1 = (*(*node).left).data[0];
	// get character from right node of tree
	char ch2 = (*(*node).right).data[0];
	// if input isn't operator, must be either number or end of line
	if(!isOperator(ch1)){
		int i = 0;
		while((*((*node).left)).data[i] != '\0' ){
			first[i] = (*((*node).left)).data[i];
			i++;
			if ((*((*node).left)).data[i] == '\0' ){
				first[i] = '\0';
			}
		}
	} else{ // input must be operator
		pipe(fd_left);
        pid_left = fork();
        if(pid_left == 0){//child
			close(fd_left[0]);
			// recurse
			char*result1 = eval_helper(tree, (*node).left, ch1, b);
			int i = 0;
			while((*result1) != '\0' && i < 10){
				first[i] = (*result1);
				result1++;
				i++;
			}
			write(fd_left[1], first, sizeof(first));
			exit(0);
		}
		close(fd_left[1]);
		left_wait = 1;
	}

	// same method for 2nd char of input
	// input isn't operator, must be number or end of line
	if( !isOperator(ch2) ){
		int i = 0;
		while((*((*node).right)).data[i] != '\0' ){
			second[i] = (*((*node).right)).data[i];
			i++;
			if ((*((*node).right)).data[i] == '\0' ){
				second[i] = '\0';
			}
		}
	}
	else{ // input must be operator
		pipe(fd_right);
        pid_right = fork();
        if(pid_right == 0){//child
            close(fd_right[0]);
			// recurse
			char*result2 = eval_helper(tree, (*node).right, ch2, b);
			int i = 0;
			while((*result2) != '\0' && i < 10){
				second[i] = (*result2);
				result2++;
				i++;
			}
			write(fd_right[1], second, sizeof(second));
			exit(0);
		}
		close(fd_right[1]);
		right_wait = 1;
	}
	
	int status = 0;
	if(b == false){
		pause();
	}
	if(left_wait){
		//read from fd_left
		waitpid(pid_left, &status, 0);
		read(fd_left[0], first, sizeof(first));
	}
	if(right_wait){
		//read from fd_right
		waitpid(pid_right, &status, 0);
		read(fd_right[0], second, sizeof(second));
	}
	float x = atof(first);
	float y = atof(second);
	
	float res = evaluate_numbers(x, y, op);
	char * result = malloc(sizeof(char) * 10);
	sprintf(result, "%f", res);
	return result;

}

/*
 * Method used to construct the expression tree
 */
Tree make_tree(char data[][20], int nodes){
    Tree result;
    int start = nodes - 1, ncounter = 0, count;

    // make nodes for sums left and right side of tree
    Node*left_total = malloc(sizeof(Node));
    Node*right_total = malloc(sizeof(Node));

    // give space
    (*left_total).data = malloc(sizeof(char) * 10);
    (*right_total).data = malloc(sizeof(char) * 10);

    // connections for left and right sides
    Node *left_array[nodes];
    Node *right_array[nodes];

    // give space to each spot in arrays
    for(count = 0; count < nodes; count++){
        left_array[count] = malloc(sizeof(Node));
        right_array[count] = malloc(sizeof(Node));
    }

    (*left_total).new_data = 0;
    (*right_total).new_data = 0;

    // create head of tree
    Node *node_front = &result.front;
    (*node_front).data = malloc(sizeof(char) * 10);
    (*node_front).prev = NULL;
    (*node_front).left = left_total;
    (*node_front).right = right_total;

    (*left_total).prev = node_front;
    (*right_total).prev = node_front;

    (*node_front).data[0] = data[start][0];
    (*node_front).data[1] = '\0';
    (*node_front).new_data = 1;

    for(start = start - 1; start >= 0; start--){
        int go = 1;
        while(go){
            if((*((*node_front).left)).new_data == 0){
                go = 0;
                int i = 0;
                while(data[start][i] != '\0'){
                    (*((*node_front).left)).new_data = 1;
                    (*((*node_front).left)).data[i] = data[start][i];
                    if(isOperator(data[start][i])){
                        (*((*node_front).left)).data[1] = '\0';

                        node_front = ((*node_front).left);

                        (*left_array[ncounter]).prev = node_front;
                        (*right_array[ncounter]).prev = node_front;

                        (*left_array[ncounter]).data = malloc(sizeof(char) * 10);
                        (*right_array[ncounter]).data = malloc(sizeof(char) * 10);

                        (*left_array[ncounter]).new_data = 0;
                        (*right_array[ncounter]).new_data = 0;

                        (*node_front).right = &(*right_array[ncounter]);
                        (*node_front).left = &(*left_array[ncounter]);

                        ncounter++;
                        break;
                    }
                    i++;
                    if(data[start][i] == '\0'){
                        (*((*node_front).left)).data[i] = '\0';
                    }
                }
            } else if((*((*node_front).right)).new_data == 0){
                go = 0;
                int i = 0;
                while(data[start][i] != '\0'){
                    (*((*node_front).right)).new_data = 1;
                    (*((*node_front).right)).data[i] = data[start][i];
                    if(isOperator(data[start][i])){
                        (*((*node_front).right)).data[1] = '\0';

                        node_front = ((*node_front).right);

                        (*left_array[ncounter]).prev = node_front;
                        (*right_array[ncounter]).prev = node_front;

                        (*left_array[ncounter]).data = malloc(sizeof(char) * 10);
                        (*right_array[ncounter]).data = malloc(sizeof(char) * 10);

                        (*left_array[ncounter]).new_data = 0;
                        (*right_array[ncounter]).new_data = 0;

                        (*node_front).right = &(*right_array[ncounter]);
                        (*node_front).left = &(*left_array[ncounter]);

                        ncounter++;
                        break;
                    }
                    i++;
                    if(data[start][i] == '\0'){
                        (*((*node_front).right)).data[i] = '\0';
                    }
                }
            }
            else{
                node_front = ((*node_front).prev);
            }
        }
    }
    
    return result;

}

/*
 * Postfix to infix evaluation, that then passes control to the
 * make_tree method
 */
void eval(char str[], bool b){
	
	char stack[40][20];
	int elements, spaces, operators, operands, stringi, spot1, spot2;
    elements = spaces = operators = operands = stringi = spot1 = spot2 = 0;
	
	while(str[stringi] != '\0'){
		
		if(isspace(str[stringi])){
			stack[spot2][spot1] = '\0';
			spot2++;
			spot1 = 0;
			elements++;
            spaces++;
		}
		else{
			stack[spot2][spot1] = str[stringi];
            if(isOperator((str[stringi]))){
                operators++;
            }
			spot1++;
			if(str[stringi + 1] == '\0'){
				stack[spot2][spot1] = '\0';
				elements++;
			}
		}
		stringi++;
	}

    //quick equation for number of operands
    operands = elements - operators;
	printf("The given equation has %d elements in total\n", elements);
    printf("%d Operators\n", operators);
    printf("%d Operands\n", operands);


	printf("%s = \n ", str);
	
	spot2 = 0;
	int temp = 0, t_ops = -1, x;
    char temp_in_stack[elements][20]; // temp stack to hold equation as full expression is parsed

    // initialize and populate temporary array to hold operations
    char temp_ops[4];
	for(x =0; x < 4; x++){
        // initialize to empty space as filler to check in upcoming if
		temp_ops[x] = "";
	}

    // checks to see if each position in the equation is an operator, operand or space
	while(spot2 < elements) {
        if( isOperator(stack[spot2][0]) ) {
			if(temp_ops[0] == ""){
                // take operator from stack, place into temp array
				temp_ops[0] = stack[spot2][0];
				t_ops++;
			}else{
				int j;
				for(j =0; j<4 ; j++){
					if(temp_ops[j] == '\0'){
						t_ops++;
						temp_ops[t_ops] = stack[spot2][0];
						break;
					} else if(isOperator(temp_ops[j]) >= isOperator(stack[spot2][0])){ // element is an operator
						for(t_ops = t_ops; isOperator(temp_ops[t_ops]) >= isOperator(stack[spot2][0]); j--){
							temp_in_stack[temp][0] = temp_ops[t_ops];
							temp_in_stack[temp][1] = '\0';
							temp++;
							temp_ops[t_ops]= '\0';
							t_ops--;
							j--;
						}
					} else{
						t_ops++;
						temp_ops[t_ops] = stack[spot2][0];
						break;
					}
				}
			}
		} else {
			int i;
            // element wasn't an operator, parse into new spot in 2d array
			for(i = 0; stack[spot2][i] != '\0'; i++){
				temp_in_stack[temp][i] = stack[spot2][i];
				if(stack[spot2][i + 1] == '\0'){
					temp_in_stack[temp][i + 1] = '\0';
				}
			}
			temp++;
		}
		spot2++;
	}

    if(temp_ops[0] != '\0'){
		for(t_ops = t_ops; t_ops > -1; t_ops--){
			temp_in_stack[temp][0] = temp_ops[t_ops];
			temp_in_stack[temp][1] = '\0';
			temp++;
		}
	}

    int a, c, row = 0, col = 0;
    for (a = 0; a < row; a++) {
        for (c = 0; c < col; c++) {

            printf("%c ", temp_in_stack[a][b]);
        }
        printf("\n");
    }

	Tree *final_tree = malloc(sizeof(Tree));
	(*final_tree) = make_tree(temp_in_stack, elements);
    char * final = eval_helper(*final_tree, &(*final_tree).front, (*final_tree).front.data[0], b);
	printf("%s\n", final);
	
}



/*
 * Utility method for checking in a char is an operator
 */
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
            //numOps++;
            return 1;
            break;
        default:
            return 0;
            // returns 0 if the symbol is other than given above
    }
}

/*
 * Utility method for evaluating the various possible operations
 */
float evaluate_numbers(float x, float y, char op){
    switch(op){
        case '+':
            return x + y;
            break;
        case '-':
            return y - x;
            break;
        case '*':
            return x * y;
            break;
        case '/':
            return y / x;
            break;
        default:
            return 0;
        // returns 0 upon error or invalid equation
    }
}




