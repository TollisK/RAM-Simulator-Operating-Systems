#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include "main.h"

//Synarthsh metatrophs dekae3adiko se dekadiko
int hextodec(char *hexVal) 
{
    int len = strlen(hexVal); 
    int base = 1; 
    int dec_val = 0; 
    for (int i=len-1; i>=0; i--) 
    {    
        if (hexVal[i]>='0' && hexVal[i]<='9') 
        { 
            dec_val += (hexVal[i] - 48)*base; 
            base = base * 16; 
        } 
        else if (hexVal[i]>='a' && hexVal[i]<='f') 
        { 
            dec_val += (hexVal[i] - 55-32)*base; 
            base = base*16; 
        } 
    } 
    return dec_val; 
} 

//Epistrofh toy xronoy ekteleshs toy programmatos se millisecond
unsigned long long current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec/**(unsigned long long)1000*/ + te.tv_usec; // calculate milliseconds
    return milliseconds;
}

//Synarthsh Least Recently Used
int LRU(frame ram[],int ram_size){
    long long minimum = ram[0].time;
    int pos = 0;
	for(int i = 1; i < ram_size; i++){ //Gia kathe frame
		if(ram[i].time < minimum){
			minimum = ram[i].time; //Vriskoyme to frame poy dhmioyrghthke prwto
			pos = i; //Kratame to noymero gia thn epistrofh toy
		}
	}
	return pos;
}

//Synarthsh Second Chance
int SC(frame ram[],int ram_size){
    long long minimum = ram[0].time;
    int pos = 0;
	for(int i = 1; i < ram_size; i++){ //Gia kathe frame
        if(ram[i].ref_bit==0){ //Vriskoyme to frame poy dhmioyrghthke prwto ektos an exei to ref bit toy 1
            if(ram[i].time < minimum){
                minimum = ram[i].time;
                pos = i;
            }
        }
	}
    for(int i = 0; i < ram_size; i++){ //Mhdenismos reference bit twn frame poy tha eprepe na vgei 
        if(ram[i].ref_bit==1 && ram[i].time<minimum){
            ram[i].ref_bit = 0;
        }
	}
	return pos;
}

//Hash function
unsigned int hash_func(unsigned int x,int ram_size) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x%ram_size;
}

int main(int argc, char **argv){
    //Ta orismata ths main se metavlhtes
    int ram_size = atoi(argv[2]); //Arithmos twn frame
    int q = atoi(argv[3]); //Plhthos twn arithmwn enalla3
    hash_block* hash[ram_size]; //Pinakas me deiktes se lista gia thn ylopoihsh enos hash table
    frame ram[ram_size]; //Pinakas apo frames gia thn ylopoihsh mias mnhmhs RAM
    int h;
    for(h=0;h<ram_size;h++){ //Arxikopoihsh twn xrhsimwn metavlhtwn
        hash[h] = NULL;
        ram[h].full=0;
    }
    //Ta arxeia poy prepei na diavasoyme
    FILE *bzip;
    FILE *gcc;
    //Open files
    bzip = fopen("bzip.trace","r");
    gcc = fopen("gcc.trace","r");
    //Data
    int writes = 0;
    int reads = 0;
    int page_faults=0;
    int page_counter=0;

    //Gia to diavasma twn trace arxeiwn
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char* pch;
    int y=0;


    char str[5];
    int num=19;
    int hash_num = 0;
    int flg=0;
    int algpage;

    //To MAX_PAGE einai define sto main.h me default timh 10000
    while(page_counter<MAX_PAGE){ //Mexri tis anafores poy theloyme na diavasoyme
        //Gia to arxeio gcc.trace
        if ((read = getline(&line, &len, bzip)) != -1 && y%(q*2) < q){ //Gia thn epomenh seira kai enw einai h seira toy
            pch = strtok (line," "); //Diavazei thn anafora
            strncpy(str,pch,5); //Topothetei to page id se metavlhth

            pch = strtok (NULL, " "); //R h W

            num = hextodec(str); //Metatroph se dakadiko
            hash_num = hash_func(num,ram_size); //Eyresh ths theshs topotethshs analoga to hash function

            if(search_list(hash[hash_num],num)){ //An yparxei hdh sto hash table, dhaldh kai sthn ram
                ram[hash[hash_num]->framenum].time = current_timestamp();//Ananewnei ton xrono sthn ram
                ram[hash[hash_num]->framenum].ref_bit = 1; //Reference bit gia ton algorithmo Second Chance se 1
                if(strcmp(pch,"W\n")==0){ //Anenwsh kai toy write/read
                    ram[hash[hash_num]->framenum].write = 1;
                }
                else{
                    ram[hash[hash_num]->framenum].write = 0;
                }
            }
            else{
                //Data 
                page_faults++;
                reads++;
                //Vriskei mia kenh thesh sthn ram
                flg = -1;
                for(h=0;h<ram_size;h++){//Gia kathe thesh ths ram
                    if(ram[h].full==0){
                        flg = h;
                        break;
                    }
                }
                if(flg!=-1){ //An yparxei kenh thesh
                    push(&hash[hash_num],flg,num); //Prostesh sto hash table
                    //Arxikopoihsh dedomenwn
                    ram[flg].full = 1;
                    ram[flg].time = current_timestamp();
                    ram[flg].page = num;
                    ram[flg].ref_bit = 0;
                    ram[flg].in_disc = 1;
                    //An einai W kanei thn metavlhth write 1
                    if(strcmp(pch,"W\n")==0){
                        ram[flg].write = 1;
                    }
                    else{
                        ram[flg].write = 0;
                    }
                }
                else{ //An den vrhke kenh thesh
                    int alg; //thesh ths ram poy prepei na vgei
                    if(!strcmp(argv[1],"LRU")) //Analoga to orisma trexei kai ton algorithmo poy zhthte
                        alg = LRU(ram,ram_size);
                    else if(!strcmp(argv[1],"SC"))
                        alg = SC(ram,ram_size);
                    else{
                        printf("Wrong algorithm input!\n");
                        return -1;
                    }
                    algpage = ram[alg].page; //Page id 
                    if(ram[alg].write==1){
                        writes++;
                    }
                    int new_hash_num = hash_func(algpage,ram_size); //Vriskei thn thesh toy sto hash table

                    //Arxikopoihsh dedomenwn
                    ram[alg].full=1;
                    ram[alg].time = current_timestamp();
                    ram[alg].page = num;
                    ram[alg].ref_bit = 0;
                    ram[alg].in_disc = 1;
                    
                    //An einai W kanei thn metavlhth write 1
                    if(strcmp(pch,"W\n")==0){
                        ram[alg].write = 1;
                    }
                    else{
                        ram[alg].write = 0;
                    }
                    //Vgazei to page apo to hash table
                    delete(&hash[new_hash_num],algpage);
                    //Prosthetei to neo page sto hash table
                    push(&hash[hash_num],alg,num);
                }
            }
            y++;
        }
        else if((read = getline(&line, &len, gcc)) != -1){ //Gia to arxeio bzip.trace
            pch = strtok (line," ");//Diavazei thn anafora
            strncpy(str,pch,5);//Topothetei to page id se metavlhth

            pch = strtok (NULL, " ");//R h W 

            num = hextodec(str);//Metatroph se dakadiko
            hash_num = hash_func(num,ram_size);//Eyresh ths theshs topotethshs analoga to hash function

            if(search_list(hash[hash_num],num)){//An yparxei hdh sto hash table, dhaldh kai sthn ram
                ram[hash[hash_num]->framenum].time = current_timestamp();//Ananewnei ton xrono sthn ram
                ram[hash[hash_num]->framenum].ref_bit = 1;//Reference bit gia ton algorithmo Second Chance se 1
                if(strcmp(pch,"W\n")==0){ //Anenwsh kai toy write/read
                    ram[hash[hash_num]->framenum].write = 1;
                }
                else{
                    ram[hash[hash_num]->framenum].write = 0;
                }
            }
            else{
                //Data 
                page_faults++;
                reads++;
                //Vriskei mia kenh thesh sthn ram
                flg = -1;
                for(h=0;h<ram_size;h++){ //Gia kathe thesh ths ram
                    if(ram[h].full==0){
                        flg = h;
                        break;
                    }
                }
                if(flg!=-1){//An yparxei kenh thesh
                    push(&hash[hash_num],flg,num);//Prostesh sto hash table
                    //Arxikopoihsh dedomenwn
                    ram[flg].full = 1;
                    ram[flg].time = current_timestamp();
                    ram[flg].page = num;
                    ram[flg].ref_bit = 0;
                    ram[flg].in_disc = 1;
                    //An einai W kanei thn metavlhth write 1
                    if(strcmp(pch,"W\n")==0){
                        ram[flg].write = 1;
                    }
                    else{
                        ram[flg].write = 0;
                    }
                }
                else{  //An den vrhke kenh thesh
                    int alg;  //thesh ths ram poy prepei na vgei
                    if(!strcmp(argv[1],"LRU"))//Analoga to orisma trexei kai ton algorithmo poy zhthte
                        alg = LRU(ram,ram_size);
                    else if(!strcmp(argv[1],"SC"))
                        alg = SC(ram,ram_size);
                    else{
                        printf("Wrong algorithm input!\n");
                        return -1;
                    }
                    if(ram[alg].write==1){
                        writes++;
                    }
                    algpage = ram[alg].page;//Page id 

                    int new_hash_num = hash_func(algpage,ram_size);//Vriskei thn thesh toy sto hash table
                    
                    //Arxikopoihsh dedomenwn
                    ram[alg].full=1;
                    ram[alg].time = current_timestamp();
                    ram[alg].page = num;
                    ram[alg].ref_bit = 0;
                    ram[alg].in_disc = 1;
                    //An einai W kanei thn metavlhth write 1
                    if(strcmp(pch,"W\n")==0){
                        ram[alg].write = 1;
                    }
                    else{
                        ram[alg].write = 0;
                    }
                    //Vgazei to page apo to hash table
                    delete(&hash[new_hash_num],algpage);
                    //Prosthetei to neo page sto hash table
                    push(&hash[hash_num],alg,num);
                }
            }
            y++;
        }
        else{
            printf("END\n\n");
            break;
        }
        page_counter++;
    }
    //Ektypwsh dedomenwn
    printf("Maximum data read: %d\n",MAX_PAGE);
    printf("Number of page faults: %d\n",page_faults);
    printf("Number of pages read: %d\n",reads);
    printf("Data writes: %d\n",writes);
    printf("Number of frames = %d\n",ram_size);
    for(int b=0;b<ram_size;b++){
        deleteList(&hash[b]);
    }
    return 0;
}

