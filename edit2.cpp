#define NULL 0
#define HEAD 0
  
struct Node {
    char c;
    Node *prev;
    Node *next;
};
Node nodes[8000000];
int last_node;  
Node *cur_node;
  
struct Line {
    Node *node;
    Line *prev;
    Line *next;
};
Line lines[80000];
int last_line;
Line *cur_line;
  
void init() {    
    last_line = 0;
    last_node = 0;
    cur_line = &lines[last_line++];
    cur_line->prev = NULL;
    cur_line->next = NULL;
     
    Node* new_node = &nodes[last_node++];
    new_node->c = HEAD;
    new_node->prev = NULL;
    new_node->next = NULL;
 
    cur_line->node = new_node;
    cur_node = new_node;
}
  
void put_char(char c) {
    Node* new_node = &nodes[last_node++];
    new_node->c = c;
 
    new_node->next = cur_node->next;
    if (cur_node->next != NULL)
        cur_node->next->prev = new_node;
     
    new_node->prev = cur_node;
    cur_node->next = new_node;
 
    cur_node = new_node;
}
  
void put_enter() {
    put_char('\n');
 
    Line* new_line = &lines[last_line++];
    new_line->next = cur_line->next;
    if (cur_line->next != NULL)
        cur_line->next->prev = new_line;
 
    new_line->prev = cur_line;
    cur_line->next = new_line;
 
    cur_line = new_line;
 
    Node* new_node = &nodes[last_node++];
    new_node->c = HEAD;
    new_node->prev = NULL;
     
    cur_line->node = new_node;
    new_node->next = cur_node->next;
    cur_node->next = NULL;
    if (new_node->next != NULL)
        new_node->next->prev = new_node;
    cur_node = new_node;
}
  
void backspace() {
    if (cur_node->c == HEAD && cur_line->prev != NULL){
        cur_line->prev->next = cur_line->next;
        if ( cur_line->next != NULL)
            cur_line->next->prev = cur_line->prev;
        cur_line = cur_line->prev;
         
        Node* tmp_node;
        for (tmp_node = cur_line->node; tmp_node->next != NULL && tmp_node->next->c != '\n'; tmp_node = tmp_node->next)
            ;
        tmp_node->next = cur_node->next;
        if (cur_node->next != NULL)
            cur_node->next->prev = tmp_node;
        cur_node = tmp_node;
    }
    else if (cur_node->c != HEAD){
        cur_node->prev->next = cur_node->next;
        if ( cur_node->next != NULL)
            cur_node->next->prev = cur_node->prev;
        cur_node = cur_node->prev;
    }
}
  
void move_cursor_up(int n) {
    for (
        int i = 0; i < n && cur_line->prev != NULL; i++, cur_line = cur_line->prev)
        ;
    Node* tmp_node = cur_line->node;
    while (cur_node->prev != NULL && tmp_node->next != NULL && tmp_node->next->c != '\n'){
        tmp_node = tmp_node->next;
        cur_node = cur_node->prev;
    }
    cur_node = tmp_node;
}
  
void move_cursor_down(int n) {
 
    for (register int i = 0; i < n && cur_line->next != NULL; i++, cur_line = cur_line->next)
        ;
    Node* tmp_node = cur_line->node;
    while (cur_node->prev != NULL && tmp_node->next != NULL && tmp_node->next->c != '\n'){
        tmp_node = tmp_node->next;
        cur_node = cur_node->prev;
    }
    cur_node = tmp_node;
}
  
void move_cursor_left(int n) {
    for (register int i = 0; i < n && cur_node->c != HEAD; i++, cur_node = cur_node->prev)
        ;
}
  
void move_cursor_right(int n) {
    for (register int i = 0; i < n && cur_node->next != NULL && cur_node->next->c != '\n'; i++, cur_node = cur_node->next)
        ;
}
  
void close(char* d) {
    while (cur_line->prev != NULL)
        cur_line = cur_line->prev;
    register int idx = 0;
    while (cur_line != NULL){
        for (cur_node = cur_line->node->next; cur_node != NULL; cur_node = cur_node->next){
            d[idx++] = cur_node->c;
        }
        cur_line = cur_line->next;
    }
}
