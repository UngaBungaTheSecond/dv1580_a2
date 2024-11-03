#include "linked_list.h"

//--------- I am soo sorry if i did not comment this script as good as the others i just di not have the time for it before the deadline -------------//

int nrOfNodes = 0;

void list_init(Node** head, size_t size){
    mem_init(size);
    *head = NULL;
}

void list_insert(Node** head, uint16_t data){
  Node* theNode = mem_alloc(sizeof(Node));
  theNode->data = data;
  theNode->next = NULL;
  if(theNode==NULL){printf("ERROR");}
  if(*head == NULL){
    *head = theNode;
  }
  else{
    Node* walkerNode = *head;
    while(walkerNode->next != NULL){
      walkerNode = walkerNode->next;
    }
    walkerNode->next = theNode;
  }
}

void list_insert_after(Node* prev_node, uint16_t data){
  Node* theNode = mem_alloc(sizeof(Node));
  theNode->data = data;
  theNode->next = prev_node->next;
  prev_node->next = theNode;
}

void list_insert_before(Node** head, Node* next_node, uint16_t data){
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
}

void list_delete(Node** head, uint16_t data){
  if(*head == NULL){
    printf("det finns inga nodes");
  }
  else if((*head)->data == data){
    Node* nexthead = (*head)->next;
    mem_free((*head));
    *head = nexthead;
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
    }
  }
}

Node* list_search(Node** head, uint16_t data){
  Node* walkerNode = *head;
  while(walkerNode->next != NULL && walkerNode->data != data){
    walkerNode = walkerNode->next;
  }
  if(walkerNode->next == NULL && walkerNode->data != data){
      printf("the node you look for does not exist");
      return NULL;
    }
  else{
    return walkerNode;
  }
}

void list_display(Node** head){
  Node* walkerNode = *head;
  printf("[");
  while(walkerNode->next != NULL){
      printf("%d", walkerNode->data);
      printf(", ");
      walkerNode = walkerNode->next;
  }
  printf("%d", walkerNode->data);
  printf("]");
}

void list_display_range(Node** head, Node* start_node, Node* end_node){
  if(start_node == NULL){
      start_node = *head;
  }

  if(end_node == NULL){
      list_display(&start_node);
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
}

int list_count_nodes(Node** head){
  int amount = 0;
  Node* walkerNode = *head;
  if(*head == NULL){
    return amount;
  }
  else{
    amount++;
    while(walkerNode->next != NULL){
      amount++;
      walkerNode = walkerNode->next;
    }
    return amount;
  }
}

void list_cleanup(Node** head){
  Node* walker = *head;
    while(walker != NULL){
        Node* toDel = walker;
        walker = walker->next;

        mem_free(toDel);
    }
    mem_deinit();
    *head = NULL;
}
