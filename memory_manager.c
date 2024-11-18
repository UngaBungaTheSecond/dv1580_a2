#include"memory_manager.h"

void* memory; // Pointer to the total allocated memory
size_t memorySize; // size of that memory
int nrofBlocks; // amount of total blocks
int fullBlocks; // amount of total blocks (that contains sum thin)

memoryBlock *TheBlocks; // pointer to the blocks
int maxNrOfBlocks;

pthread_mutex_t lock;

void mem_init(size_t size){ // initilize the blocks
    pthread_mutexattr_t recurv;
    pthread_mutexattr_init (&recurv);
    pthread_mutexattr_settype(&recurv, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&lock,&recurv);

    maxNrOfBlocks = 50;
    TheBlocks = malloc(maxNrOfBlocks * sizeof(memoryBlock)); // 
    memory = malloc(size); // the memory
    TheBlocks[0] = (memoryBlock){memory, size, false}; // allocate one block with all the memory and then split it later into samller blocks to work with
    nrofBlocks = 1;
    memorySize = size;
}

void* mem_alloc(size_t size){ // first check if space free or not, check if exeisting free allocated space to take, lastly make a "new space for them" after the last one
    pthread_mutex_lock(&lock);
    int i = 0;
    if(nrofBlocks >=  maxNrOfBlocks) {blockExpand();}
    if(size > memorySize){printf("no space here for that");} // prints no memory when there is no space avilable
    while (i< nrofBlocks && (TheBlocks[i].taken == true || TheBlocks[i].blocksize < size)){i++;}
    if(i >= nrofBlocks) {
        pthread_mutex_unlock(&lock);
        return NULL;
    }
    fullBlocks++;
    if(TheBlocks[i].blocksize == size){
        TheBlocks[i].taken = true;
        pthread_mutex_unlock(&lock);
        return TheBlocks[i].start;
    }
    else{
        void* newStart = (void*)((char*)TheBlocks[i].start + size); // pointer to where the new block should start
        TheBlocks[nrofBlocks]=(memoryBlock){newStart, TheBlocks[i].blocksize - size, false};
        TheBlocks[i].blocksize = size;
        TheBlocks[i].taken = true;
        nrofBlocks++;
        pthread_mutex_unlock(&lock);
        return TheBlocks[i].start;
    }
}

void mem_free(void* block){ //does not neccicerly free up the memory but marks it free and mark it as one less full block, if its no full block it merges everything back into one continus flow of memroy block
    pthread_mutex_lock(&lock);
    int i =0;
    while(i < nrofBlocks && TheBlocks[i].start != block) {i++;}
    if(i>= nrofBlocks){
        pthread_mutex_unlock(&lock);
        return;
        }
    TheBlocks[i].taken = false;
    fullBlocks--;
    if(fullBlocks <= 0){
        TheBlocks[0] = (memoryBlock) {memory, memorySize, false};
        nrofBlocks = 1;
    }

    pthread_mutex_unlock(&lock);
}

void* mem_resize(void* block, size_t size){ // first free's the blocks allocated memory and fixes a better fit for the block
    pthread_mutex_lock(&lock);
    mem_free(block);
    void* hehe = mem_alloc(size);
    pthread_mutex_unlock(&lock);
    return hehe;
}

void mem_deinit(){ // deinitializion to free the memory allocated of the heap to get rid of memory leaks
    free(memory); 
    free(TheBlocks);
}


//experiments
void blockExpand()
{
    maxNrOfBlocks += 200;
    memoryBlock* tempBlocksArray = calloc(maxNrOfBlocks, sizeof(memoryBlock));

    for (int i = 0; i < nrofBlocks; i++)
    {
        tempBlocksArray[i] = TheBlocks[i];
    }

    free(TheBlocks);
    TheBlocks = tempBlocksArray;
}