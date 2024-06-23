#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 20
#define MAX2 100000
/*
	@brief dugumlerin yapisi
	@param data okunan kelime
	@param next sonraki dugumu isaret eder
*/
typedef struct Node{
	char data[MAX];
	struct Node *next;
}Node;
/*
	@brief stack yapisi
	@param top stackin en ustteki elemani
*/
typedef struct Stack{
	Node *top;
}Stack;
/*
	@brief kuyrugun yapisi
	@param stackArr kuyruktaki stacklerin dizisi
	@param front kuyrugun basindaki elemanin yeri
	@param rear kuyrugun sonundaki elemanin yeri
*/
typedef struct Queue{
	Stack stackArr[MAX2];
    int front;
    int rear;
}Queue;
Stack* createStack();//stack olusturur
Queue* createQueue(Stack*);//kuyruk olusturu
void push(char*, Stack*);//stacke eleman ekler
char* pop(Stack*);//stackten eleman cikarir
char* peek(Stack*);//stackin son eklenen elemaninin degerini getirir
int isEmptyStack(Stack *);//stack bos mu kontrolu
int isEmptyQueue(Queue*);//kuyruk bos mu kontrolu
int isFullQueue(Queue*);////kuyruk dolu mu kontrolu
void enqueue(Queue*, Stack *);//kuyruga stack ekler
void copyStack(Stack *, Stack *);//stack kopyalar
Stack* dequeue(Queue*);//kuruktan stack cikarir
int searchList(char**,char*,int);//kullanilan kelimelerin arasinda istenilen kelime var mi kontrol eder
void printStack(Stack *);//stack i yazdirir
void printStackLast(Stack *);//bulunan yolu yazdirmak icin kullanilan stack fonksiyonu
int diff(char*, char*, int);//kelimelerin harfleri arasinda 1 fark var mi kontrolu
void printQueue(Queue*);//kuyrugu yazdirir
int main(){
	char source[MAX], dest[MAX];//source: kaynak kelime, dest: hedef kelime
	printf("Word Ladder\n\n");
	printf("Source: ");
	scanf(" %s", &source);
	printf("Destination: ");
	scanf(" %s", &dest);
	printf("\nSource: %s Destination: %s \n",source,dest);
	if(strlen(source)==strlen(dest)){//hedef ve kaynak kelimenin uzunlugu esit mi diye bakýlýr degilse zaten yol bulunamaz
		int i;
		char temp[MAX];//dosyadan okunan kelimeler icin kullanilir
		int length = strlen(source);// kaynak kelimenin uzunlugu
		FILE *fp = fopen("dictionary.txt","r");
		if(!fp){
	        printf("File could not be opened!");
	        printf("File name: %s\n", "dictionary.txt");
			return;
	    }
	    i=0;
	    Stack *stack = createStack();	    
	    Queue* queue = createQueue(stack);
	    while(i==0&&!feof(fp)){//kaynak kelime dosyada var mý kontrolu yapilir
	        fscanf(fp,"%s\n",temp);
			if(strcmp(source,temp)==0){				 		
				i=1;
			} 			 
	    }
	    if(i==1){//kaynak kelime dosyada varsa hedef kelime dosyada var mi kontrolu yapilir
	    	i=0;
		    fseek(fp,0,SEEK_SET);
		    while(i==0&&!feof(fp)){
		        fscanf(fp,"%s\n",temp);
				if(strcmp(dest,temp)==0){				 		
					i=1;
				} 			 
		    }	
		}	        
	    if(i==1){//kaynak kelime de hedef kelime de doyada varsa aralarindaki yolu bulmak icin buraya girilir
		    char tempSource[MAX];//kaynak kelimeyi tutar sonrada dosyadan okunan kelimeyi tutarki o kelimeden 1 harf farkli kelimeleri bulabilsin	    
			char **usedWords;//kullanilan kelimeleri tutar
		    int flag=1,count=1,numberOfUsedWords=0;//flag: hedef kelimenin bulnunp bulunmadýgýnýn kontrolu icin kullanilir, count: dosyada 1 harf farkli ve kullanilmayan kelime vvar mi kontrolu icin kullanilir
		    //numberOfUsedWords:dosyadaki kullanilan kelime ssayisini tutar
		    strcpy(tempSource,source);
		    usedWords = (char**)malloc(MAX2*sizeof(char*));//kullanilan kelimeler listesi icin yer acar
			for(i=0;i<20000;i++)
				usedWords[i] = (char*)malloc(MAX*sizeof(char));	
			if(strcmp(dest,source)==0){
				printf("These words are same.");
			} 
			else{
				FILE *fp2 = fopen("sameLen2.txt","w+");//programin hizlanmasi icin ayni uzunluktaki kelimeleri baska dosyaya yazar
		    	fseek(fp,0,SEEK_SET);
		    	while(!feof(fp)){		    		
			        fscanf(fp,"%s\n",temp);
			        if(strlen(temp)==length)
						fprintf(fp2, "%s\n", temp);			 
			    }
				strcpy(usedWords[numberOfUsedWords],source);//kaynak kelime kullanilan kelime listesine yazilir
				push(source,stack);//kaynak kelime stacke push edilir
				do{//ayni uzunluýktaki kelimelerin bulundugu dosya icerisinde yol aranir
					count=0;				
					fseek(fp2,0,SEEK_SET);
				   	while(!feof(fp2)){
				        fscanf(fp2,"%s\n",temp);				        
				        if(diff(tempSource, temp, length) && searchList(usedWords,temp,numberOfUsedWords) == 0){//kelimeler arasindaki fark 1 ve daha once kullannilmis mi kontrolu					        		
				       		strcpy(usedWords[numberOfUsedWords],temp);//kelime kosullardan gecince kullanilan kelime listesine yazilir
							numberOfUsedWords++;
							count++;
							Stack *cStack= createStack();//yeni stack olusturulur
							copyStack(cStack,stack);//onceki stack kopyalanir																					 		
							push(temp,cStack);//yeni olusan stacke okunan kelime push edilir
							enqueue(queue,cStack);	//yeni olusan stack kuyruga enqueue edilir					 	
						}															 
				    }								    
				    //printQueue(queue);//kuyruk yazdirilir						
					stack = dequeue(queue);//kuyruktan bir stack dequeue edilir					
					if(stack != NULL){					
						if(strcmp(peek(stack),dest)==0){//kelime hedefle uyusuyorsa yol yazdirilr ve donguden cikilir
							printf("\nMin path:\n");
					    	printStackLast(stack);
					    	flag = 0;	
						}
						else{
							strcpy(tempSource, peek(stack));//	kelime hedefle uyusmuyorsa yeni tempSource a atanýr ki bu kelime ile 1 harf farkli kelimeler bulunabilsin
						}
						count=1;	
	           		}
					else{
					   	count=0;
					   	printf("\nConnection could not found.\n");
					}		            										
				}while(flag && count);		
			}  
			for(i=0;i<20000;i++)//kullanilan kelime listesi free edilir
				free(usedWords[i]); 
			free(usedWords);	 	
		}
		else{
			printf("\nThe dictionary does not contain this word.\n");
		}
		fclose(fp);
	} 
	else
		printf("\nThe number of letters in the source and target words is not the same.\n");	
	return 0;
}
/*
	@brief stack olusturu
	@param stack olusturulan stack
	@param top stackin ust elemanini isaret eder
	@return stack olusturulan stacki dondurur
*/
Stack* createStack(){
	Stack *stack = (Stack *)malloc(sizeof(Stack));
	stack->top=NULL;
	return stack;
}
/*
	@brief kuyruk olusturur
	@param queue olusturulan kuyruk
	@param front kuyrugun basindaki elemanin yeri, eleman olmadigi icin -1
	@param rear kuyrugun sonundaki elemanin yeri, eleman olmadigi icin -1
	@return queue olusturulan kuyrugu dondurur
*/
Queue* createQueue(Stack* stack){
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	queue->front=-1;
	queue->rear=-1;
	return queue;
}
/*
	@brief stacke kelime basar
	@param node basilan kelimenin dugumu
	@param data basilan kelime
	@return
*/
void push(char* data, Stack* stack){
	Node* node= (Node *)malloc(sizeof(Node));
	node->next=stack->top;
	strcpy(node->data,data);
	stack->top=node;
	return;
}
/*
	@brief stackten kelime ceker
	@param queue olusturulan kuyruk
	@return data stackten cekilen kelimeyi dondurur
*/
char* pop(Stack* stack){
	char* data = (char*)malloc(MAX * sizeof(char));
	if(!isEmptyStack(stack)){
		strcpy(data,stack->top->data);
		stack->top=stack->top->next;
		return data;	
	}
	return NULL;
}
/*
	@brief stacktin en ustteki elemanini getirir
	@param stack cekilecek kelimenin bulundugu stack
	@return stack->top->data stacktin en ustteki elemanini dondurur
*/
char* peek(Stack* stack){
	return stack->top->data;
}
/*
	@brief stack bos mu kontrol eder
	@param stack bos olup olmayacagi kontrol edilecek stack
	@return 1 stack bos
	@return 0 stack bos degil
*/
int isEmptyStack(Stack *stack){
	if(stack->top==NULL)
		return 1;
	else
		return 0;
}
/*
	@brief queue bos mu kontrol eder
	@param queue bos olup olmayacagi kontrol edilecek queue
	@return 1 queue bos
	@return 0 queue bos degil
*/
int isEmptyQueue(Queue* queue){
	if(queue->front==-1&&queue->rear==-1)
		return 1;
	else
		return 0;
}
/*
	@brief queue dolu mu kontrol eder
	@param queue dolu olup olmayacagi kontrol edilecek queue
	@return ((queue->rear + 1) % MAX2 == queue->front) sonuc 1 ise dolu 0 ise bos
*/
int isFullQueue(Queue* queue) {
    return ((queue->rear + 1) % MAX2 == queue->front);
}
/*
	@brief kuyruga stack ekler
	@param queue stack eklenecek kuyruk
	@param stack kuyruga eklenecek stack
	@return
*/
void enqueue(Queue* queue, Stack *stack){
	if(isFullQueue(queue)){
        printf("Kuyruk dolu!\n");
        return;
    }
    if(isEmptyQueue(queue)){
        queue->front = 0;
        queue->rear = 0;
    } 
	else{
        queue->rear = (queue->rear + 1) % MAX2;//circular olmasi icin
    }
    queue->stackArr[queue->rear] = *stack;	
}
/*
	@brief stacki kopyalar
	@param src kopyalanan stack
	@param dest src dest e kopyalanacak 
	@param aux yardimci stack , kopyalanirken stackin sirasi degismemesi icin kullanilir
	@return
*/
void copyStack(Stack *dest, Stack *src) {
	Stack* aux = createStack();
	char *data = (char*)malloc(MAX*sizeof(char));
    while(!isEmptyStack(src)){
    	strcpy(data,pop(src));
    	push(data,aux);
	}
	while(!isEmptyStack(aux)){
    	strcpy(data,pop(aux));
    	push(data,dest);
    	push(data,src);
	}
}
/*
	@brief kuyruktan stack ceker
	@param queue stack cekilecek olan queue
	@param stack kuyruktan cekilen stack
	@return stack kuyruktan cekilen stack dondurulur
*/
Stack* dequeue(Queue* queue){
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if(isEmptyQueue(queue)){
        printf("Queue is empty!\n");
        return NULL;
    }
    *stack = queue->stackArr[queue->front];
    if(queue->front == queue->rear){
        queue->front = -1;
        queue->rear = -1;
    } 
	else{
        queue->front = (queue->front + 1) % MAX2;//circular olmasi icin
    }
    return stack;
}
/*
	@brief kullanilan kelime listesini gezer
	@param usedWord kullanilmis kelimeleri tekrar kullanmamak icin kullanilan matris
	@param data listede varligi kontrol edilecek kelime
	@param numberOfUsedWords kullanilan kelime listesinin boyutu
	@return 1 kelime daha once kullanilmamis
	@return 1 kelime daha once kullanilmis
*/
int searchList(char **usedWords,char*data,int numberOfUsedWords){
	int i;
	for(i=0;i<numberOfUsedWords;i++){
		if(strcmp(usedWords[i],data)==0){
			return 1;
		}
	}
	return 0;
}
/*
	@brief stacki yazdirir
	@param stack yazdirilacak stack
	@param aux stack yazdirilirken push edilen elemanlar kaybolmasin ve kelimelerin sirasi bozulmasin diye kullanilan stack
	@param data stackten cekilen eklenen kelime
	@return
*/
void printStack(Stack *stack) {
    if (stack->top == NULL) {
        printf("Stack is empty.\n");
        return;
    }   
    Stack* aux = createStack();
	char *data = (char*)malloc(MAX*sizeof(char));
    while(!isEmptyStack(stack)){
    	strcpy(data,pop(stack));
		printf("| %s |\n", data);
    	push(data,aux);
	}
	printf("_______");
    printf("\n\n");
	while(!isEmptyStack(aux)){
    	strcpy(data,pop(aux));
    	push(data,stack);
	}
	free(aux);
	free(data);
}
/*
	@brief bulunan yolu yazdirmak icin kullanilir,stacki yazdirir ,usteki fonksiyondan farki stack seklinde yazdirmaz kelimeleri yanyana yazdirir 
	@param stack yazdirilacak stack
	@param aux stack yazdirilirken push edilen elemanlar kaybolmasin ve kelimelerin sirasi bozulmasin diye kullanilan stack
	@param data stackten cekilen eklenen kelime
	@return
*/
void printStackLast(Stack *stack) {
    if (stack->top == NULL) {
        printf("Stack is empty.\n");
        return;
    }   
    Stack* aux = createStack();
	char *data = (char*)malloc(MAX*sizeof(char));
    while(!isEmptyStack(stack)){
    	strcpy(data,pop(stack));	
    	push(data,aux);
	}
	while(!isEmptyStack(aux)){
    	strcpy(data,pop(aux));
		printf(" %s ", data);
    	push(data,stack);
	}
	printf("\n\n");
	free(aux);
	free(data);
}
/*
	@brief kelimelerin 1 harfi farkli mi kontrol eder 
	@param count farkli kelime sayisini tutar
	@return 1 1 harf farkli
	@return 0 1 den fazla kelime farkli
*/
int diff(char* word1, char* word2, int length){
	int i;
    int count = 0;
    for (i=0; i<length; i++){
        if(word1[i] != word2[i]){
            count++;
        }
        if (count > 1) {
            return 0;
        }
    }
    if(count==1)
    	return 1;
}
/*
	@brief kuyrugu yazdirir
	@param stack yazdirilacak stack
	@return
*/
void printQueue(Queue* queue){
    if (isEmptyQueue(queue)){
        printf("Queue is empty!\n");
        return;
    }
    printf("\nQUEUE\n______________________________\n");
    int i = queue->front;
    do{
    	printStack(&queue->stackArr[i]);
        i = (i + 1) % MAX2;
    }while (i != (queue->rear + 1) % MAX2);//farkli
    printf("\n");
}
