#ifndef HEADER_FILE_NAME
#define HEADER_FILE_NAME
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#define MAX_PAGE 1000 //Arithmos twn anafwrwn poy tha diavastoyn
 
//Typos enos frame me ta dedomena toy
typedef struct {
    unsigned long long time; //Xronos apo thn arxh ths ekteleshs toy programmatos
    int in_disc; //Boolean timh gia thn eyresh toy write data
    int write; //Boolean timh pou einai 1 otan ginetai write enw 0 gia read
    int full; //Boolean timh gia to an yparxoyn dedomena sto frame
    int page; //Id selidas poy apothhkeyetai
    int ref_bit; //Boolean timh gia ton SC algorithmo
}frame; 

//Komvos listas gia thn ylopoihsh toy hash table
typedef struct node {
   int framenum;
   int pagenum;
   struct node *next;
}hash_block;

//Vasikes synarthseis listas:
void push(hash_block ** head, int frame, int page) {
    hash_block * new_node;
    new_node = (hash_block *) malloc(sizeof(hash_block));

    new_node->framenum = frame;
    new_node->pagenum = page;
    new_node->next = *head;
    *head = new_node;
}

int delete(hash_block**head,int page) {

   hash_block* current = *head;
   hash_block* previous = NULL;

    if (*head == NULL) {
        return -1;
    }

    while (current->pagenum != page){
        if (current->next == NULL){
            return -1;
        }else{
            previous = current;
            current = current->next;
        }
    }
    if (current == *head){
        *head = (*head)->next;
        free(current);
    }else{
        previous->next = current->next; 
        free(current);
    }
    
   return 1;
}

bool search_list(hash_block* head, int page){
    hash_block* current = head;  
    while (current != NULL) 
    { 
        if (current->pagenum == page){
            return true; 
        }
        current = current->next; 
    } 
    return false;

}
void deleteList(struct node** head_ref) 
{ 
   struct node* current = *head_ref; 
   struct node* next; 
   while (current != NULL)  
   { 
       next = current->next; 
       free(current); 
       current = next; 
   } 
   *head_ref = NULL; 
} 

#endif
