#include <stdio.h>

void q1 (void);
void q2 (void);
void q3 (void);
void q4 (void);

int main (void){
	int question = 0;
	printf("Input nmbr of question (q1-4): ");
	scanf("%*c%d", &question);
	if (question == 1) q1();
	else if (question == 2) q2();
	else if (question == 3) q3();
	else if (question == 4) q4();
	else printf("wrong question");
	return 0;
}

void q1 (void){
	int lines = 0;
	printf("Enter how many lines: ");
	scanf ("%d", &lines);
	for (int i = lines; i > 0; i--){
		for (int j = 1; j <= i; j++){
			printf("*");
		}
		printf("\n");
	}
}

void q2 (void){
	int lines = 0;
	printf("Enter how many lines: ");
	scanf ("%d", &lines);
	for (int i = 0; i <= lines; i++){
		for (int j = 1; j <= i; j++){
			printf("*");
		}
		printf("\n");
	}
	for (int i = lines; i > 0; i--){
		for (int j = 1; j <= i; j++){
			printf("*");
		}
		printf("\n");
	}
}

void q3 (void){
	int lines = 0;
	printf("Enter how many lines: ");
	scanf ("%d", &lines);
	for (int i = 0; i <= lines; i++){
		for (int j = 1; j <= i; j++){
			printf("%d ", i);
		}
		printf("\n");
	}
}

void q4 (void){
	int lines = 0;
	printf("Enter how many lines: ");
	scanf ("%d", &lines);
	for (int i = 0; i <= lines; i++){
		for (int j = 1; j <= i; j++){
			printf("%d ", j);
		}
		printf("\n");
	}
}
