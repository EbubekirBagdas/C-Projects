/*Ebubekir Bağdaş 150122053*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/*Define a structure for linked list nodes*/
struct Node{
    int digit;
    struct Node* next;
};
/*Function to add a new node at the beginning of the linked list*/
void addNode(struct Node** head, int digit){
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode -> digit = digit;
    newNode -> next = *head;
    *head = newNode;

}
/* Function to print the linked list as a string */
char * printList(struct Node* head, int size){
    char* ch = (char*)malloc(size * sizeof(char));
    int currentIndex = 0;
    struct Node* temp = head;
    while(temp != NULL){
        /*Check if more space is needed for the string*/
        if(currentIndex >= size){
            size *= 2;
            ch = (char*)realloc(ch, size * sizeof(char));/*Reallocate memory*/
        }
        ch[currentIndex] = temp -> digit + '0';
        currentIndex++;
        temp = temp -> next;
    }
    ch[currentIndex] = '\0';
    return ch;
}
/* Function to print the linked list as a string */
int getLength(struct Node* num){
    int length = 0;
    struct Node* newNum = num;
    while(newNum != NULL){
        newNum = newNum -> next;
        length++;
    }
    return length;
}
/* Function to print the result in a formatted string */
char * printResult(struct Node* head, int size, int length, int decimalPart){
    struct Node* temp = head;
    char *ch = (char*)malloc(size * sizeof(char));
    ch[0] = '\0';
    while(temp != NULL){
        char tempStr[2];
        sprintf(tempStr, "%d", temp -> digit);/*Convert digit to string*/
        strcat(ch, tempStr);/*Append to the result string*/
        decimalPart++;/*Increment decimal part counter*/
        if(length - decimalPart == 0){/*Check if a decimal point is needed*/
            strcat(ch, ".");
        }
        temp = temp -> next;
    }
    return ch;
}
/* Function to reverse the linked list */
struct Node* reverseList(struct Node* head){
    struct Node* next = NULL;
    struct Node* current = head;
    struct Node* prev = NULL;

    while(current != NULL){
        next = current -> next;/*Store next node*/
        current -> next = prev;/*Reverse the current node's next pointer*/
        prev = current;/*Move prev to current*/
        current = next;/*Move to next node*/
    }

    return prev;

}
/* Function to read a number from a file and return it as a linked list */
struct Node* readNumberFromFile(FILE * file){

    struct Node* num = NULL;
    char ch;
    while((ch = fgetc(file)) != '\n' && ch != EOF){
        if(ch >= '0' && ch <= '9')
            addNode(&num, ch - '0');
    }

    return reverseList(num);

}
/* Function to add two linked lists representing numbers */
struct Node* addResults(struct Node* temp1, struct Node* temp2){
    struct Node* result = NULL;
    struct Node* p1 = reverseList(temp1);
    struct Node* p2 = reverseList(temp2);
    int carry = 0;/*Initialize carry for addition*/
    while(p1 != NULL || p2 != NULL){
        int sum = carry;
        if(p1 != NULL){
            sum += p1 -> digit;
            p1 = p1 -> next;
        }
        if(p2 != NULL){
            sum += p2 -> digit;
            p2 = p2 -> next;
        }

        addNode(&result, sum % 10);/*Add the result digit to the list*/
        carry = sum / 10;/*Update carry*/


    }

    if (carry > 0)/*If carry is left*/
    {
        addNode(&result, carry);
    }

    return result;
}
/* Function to multiply two linked lists representing numbers */
struct Node* multiply(struct Node* num1, struct Node* num2){

    struct Node* n1 = reverseList(num1);
    struct Node* n2 = reverseList(num2);
    struct Node* temp = n1;
    struct Node* p1 = n1;
    int shift = 0;
    while(temp != NULL){
        temp = temp -> next;
         shift++;
    }

    struct Node* temps[shift];/*Array to store intermediate results*/
    for(int i = 0; i < shift; i++){
        temps[i] = NULL;/*Initialize all elements to NULL*/
    }

    int i = 0, product;

    while(p1 != NULL){

        struct Node* currentProduct = NULL;
        int carry = 0;
        struct Node* p2 = n2;
        while(p2 != NULL){
            int product = p1 -> digit * p2 -> digit + carry;/*Multiply digits and add carry*/
            addNode(&currentProduct, product % 10);
            carry = product / 10;
            p2 = p2 -> next;

        }

        if (carry > 0)/*If carry is left*/
        {
            addNode(&currentProduct, carry);

        }
        /*Shift the current product based on the position*/
        for(int zero = i; zero > 0; zero--){
            struct Node* zerosNode = (struct Node*)malloc(sizeof(struct Node*));
            zerosNode -> digit = 0;
            currentProduct = reverseList(currentProduct);
            zerosNode -> next = currentProduct;
            currentProduct = reverseList(zerosNode);
        }

        temps[i] = currentProduct;
        i++;
        p1 = p1 -> next;
    }

    struct Node* result = temps[0];/*Initialize result with the first product*/
    i = shift - 1;/*Start from the last product*/
    /*Add all intermediate products to get the final result*/
    for(;i > 0; i--){
        result = addResults(result, temps[i]);

    }

    return result;


}
/* Function to convert a linked list to a string */
char* nodeToStr(struct Node* head, int length){
    char* result = (char*)malloc((length + 1) * sizeof(char));
    result[0] = '\0';

    struct Node* current = head;/*Pointer for traversal*/

    while(current != NULL){
        char digitStr[2];/*Temporary string for storing single digit*/
        sprintf(digitStr, "%d", current -> digit);/*Convert digit to string*/
        strcat(result, digitStr);/*Append to the result string*/
        current = current -> next;
    }

    return result;
}
/*Function to count the number of decimal places in a string*/
void controlDecimalPart(char* num, int* decimalPart){
    int index = strlen(num) - 1; /*Start from the end of the string*/
    char ch = num[index];
    while(index >= 0 && ch != '.'){/*Count digits until a decimal point is found*/
        (*decimalPart)++;
        index--;
        ch = num[index];
    }

}
/*Function to create a string from a file*/
char* createStr(FILE * file){
    char ch;
    int currentIndex = 0;
    int size = 10;
    char* strNum = (char*)malloc(size * sizeof(char));/*Read until end of line or file*/
    while((ch = fgetc(file)) !=  '\n' && ch != EOF){
        if(currentIndex >= size){
            size *= 2;
            strNum = (char*)realloc(strNum, size * sizeof(char));
        }
        strNum[currentIndex] = ch; /*Add character to the string*/
        currentIndex++;
    }
    strNum[currentIndex] = '\0'; /*Null-terminate the string*/
    return strNum;
}

int main()
{

    clock_t start = clock();

    FILE * file = fopen("input.txt", "r");
    if (!file) {
        printf("File couldn't open.\n");
        return 1;
    }

    struct Node* num1 = readNumberFromFile(file);
    struct Node* num2 = readNumberFromFile(file);


    int decimalPart1 = 0;
    int decimalPart2 = 0;

    fclose(file);


    int len1 = getLength(num1);
    int len2 = getLength(num2);

    char * strNum1 = (char*)malloc(len1 * sizeof(char));
    char * strNum2 = (char*)malloc(len2 * sizeof(char));
    file = fopen("input.txt", "r");

    strNum1 = createStr(file);
    strNum2 = createStr(file);

    fclose(file);

    controlDecimalPart(strNum1, &decimalPart1);
    controlDecimalPart(strNum2, &decimalPart2);

    struct Node* result = multiply(num1, num2);
    int lenResult = getLength(result);
    char * strResult = (char*)malloc(lenResult * sizeof(char));

    int decimalPartResult = decimalPart1 + decimalPart2;
    strResult = nodeToStr(result, lenResult);

    if(decimalPartResult != 0){
    strResult = printResult(result, lenResult + 1, lenResult, decimalPartResult);
    }
    else {
    strResult = printList(result, lenResult);
    }



    char* output = (char*)malloc((strlen(strNum1) + strlen(strNum2) + strlen(strResult) + 100) * sizeof(char));
    sprintf(output, "\t%s\n\t%s\n\t\nx_________________________________________________________\n\t%s", strNum1, strNum2, strResult);

    file = fopen("output.txt", "w+");

    clock_t end = clock();
    char* strExecution = (char*)malloc(100 * sizeof(char));
    double execution_time = (double)(end - start) / CLOCKS_PER_SEC;
    sprintf(strExecution, "\n\tExecution Time: %f seconds\n", execution_time);

    strcat(output, strExecution);
    fputs(output, file);

    fclose(file);

    free(strNum1);
    free(strNum2);
    free(strResult);
    free(output);

    return 0;

}