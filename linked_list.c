#include "linked_list.h"

//--------- I am soo sorry if i did not comment this script as good as the others i just di not have the time for it before the deadline -------------//

atomic_int nrOfNodes = 0;
pthread_rwlock_t Lock;

void list_init(Node** head, size_t size){
    pthread_rwlock_init(&Lock, NULL);
    mem_init(size);
    *head = NULL;
}

void list_insert(Node** head, uint16_t data){
  pthread_rwlock_wrlock(&Lock);
  Node* theNode = mem_alloc(sizeof(Node));

  if(theNode==NULL){
    printf("ERROR");
    pthread_rwlock_unlock(&Lock);
    return;
  }

  theNode->data = data;
  theNode->next = NULL;

  if(*head == NULL){
    *head = theNode;
    pthread_rwlock_unlock(&Lock);
    return;
  }

  Node* walkerNode = *head;
  while(walkerNode->next != NULL){
    walkerNode = walkerNode->next;
  }

  walkerNode->next = theNode;
  nrOfNodes += 1;
  pthread_rwlock_unlock(&Lock);
}

void list_insert_after(Node* prev_node, uint16_t data){
  pthread_rwlock_wrlock(&Lock);
  Node* theNode = mem_alloc(sizeof(Node));
  theNode->data = data;
  theNode->next = prev_node->next;
  prev_node->next = theNode;
  nrOfNodes += 1;
  pthread_rwlock_unlock(&Lock);
}

void list_insert_before(Node** head, Node* next_node, uint16_t data){
  pthread_rwlock_wrlock(&Lock);
  Node* theNode = mem_alloc(sizeof(Node));
  theNode->data = data;
  
  if(*head == next_node){
    theNode->next = *head;
    *head = theNode;
  }
  else{
    Node* walkerNode = *head;
    while(walkerNode->next != NULL && walkerNode->next != next_node){
      walkerNode = walkerNode->next;
    }
    walkerNode->next = theNode;
    theNode->next = next_node;
  }
  nrOfNodes += 1;
  pthread_rwlock_unlock(&Lock);
}

void list_delete(Node** head, uint16_t data){
  pthread_rwlock_wrlock(&Lock);
  Node* atHead = *head;
  if(atHead == NULL){
    printf("det finns inga nodes");
  }
  else if((atHead)->data == data){
    Node* nexthead = (atHead)->next;
    mem_free((atHead));
    *head = nexthead;
    nrOfNodes -= 1;
  }
  else{
    Node* walkerNode = *head;
    Node* prevNode;
    while(walkerNode->next != NULL && walkerNode->data != data){
      prevNode = walkerNode;
      walkerNode = walkerNode->next;
    }
    if(walkerNode->next == NULL && walkerNode->data != data){
      printf("the node you look for does not exist");
    }
    else{
      prevNode->next = walkerNode->next;
      mem_free(walkerNode);
      nrOfNodes -= 1;
    }
  }
  pthread_rwlock_unlock(&Lock);
}

Node* list_search(Node** head, uint16_t data){
  pthread_rwlock_rdlock(&Lock);
  Node* walkerNode = *head;
  while(walkerNode->next != NULL && walkerNode->data != data){
    walkerNode = walkerNode->next;
  }
  if(walkerNode->next == NULL && walkerNode->data != data){
      printf("the node you look for does not exist");
      pthread_rwlock_unlock(&Lock);
      return NULL;
    }
  else{
    pthread_rwlock_unlock(&Lock);
    return walkerNode;
  }
}

void list_display(Node** head){
  pthread_rwlock_rdlock(&Lock);
  Node* walkerNode = *head;
  printf("[");
  while(walkerNode->next != NULL){
      printf("%d", walkerNode->data);
      printf(", ");
      walkerNode = walkerNode->next;
  }
  printf("%d", walkerNode->data);
  printf("]");
  pthread_rwlock_unlock(&Lock);
}

void list_display_range(Node** head, Node* start_node, Node* end_node){
  pthread_rwlock_rdlock(&Lock);
  if(start_node == NULL){
      start_node = *head;
  }

  if(end_node == NULL){
      list_display(&start_node);
      pthread_rwlock_unlock(&Lock);
      return;
  }

  Node* walkerNode = start_node;
  printf("[");

  while(walkerNode->next != NULL && walkerNode != end_node){
      printf("%d, ", walkerNode->data);
      walkerNode = walkerNode->next;
  }
  printf("%d", walkerNode->data);
  printf("]");
  pthread_rwlock_unlock(&Lock);
}

int list_count_nodes(Node** head){
  pthread_rwlock_wrlock(&Lock);
  atomic_int amount = 0;
  if(*head == NULL){
    pthread_rwlock_unlock(&Lock);
    return amount;
  }
  else{
    Node* walkerNode = *head;
    amount = 1;
    while(walkerNode->next != NULL){
        amount++;
        walkerNode = walkerNode->next;
    }
    pthread_rwlock_unlock(&Lock);
    return amount;
  }
}

void list_cleanup(Node** head){
  pthread_rwlock_wrlock(&Lock);
  Node* walker = *head;
  while(walker != NULL){
      Node* toDel = walker;
      walker = walker->next;
      mem_free(toDel);
  }
  mem_deinit();
  *head = NULL;
  nrOfNodes = 0;
  pthread_rwlock_unlock(&Lock);
  pthread_rwlock_destroy(&Lock);
}
