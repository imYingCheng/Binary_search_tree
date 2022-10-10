#include <stdio.h>
#include <stdlib.h>
#define MAX_DEPTH 30
FILE *input_file;
FILE *output_file;
int depth;
int subMax_data;
int subMin_data;
int start_path[MAX_DEPTH];
int end_path[MAX_DEPTH];
typedef struct treeNode *Node;
struct treeNode{
    int data;
    Node llink, rlink, parent;
};
void print_tree(Node *);
int find_target(Node *, int);
void find_subMin(Node *, Node *);
void find_subMax(Node *, Node *);
void delete_tree(Node *);
int path(Node *, int, int);
int query(Node *, int);
void delete(Node *, int);
void insert(Node *, int);
void process(Node *, int);
Node read_treeNode(int);
int main(){
    int m, n;
    int itr = 0;
    Node head;
    input_file = fopen("input_1.txt", "r");
    output_file = fopen("output_1.txt", "w");
    while(1){
        fscanf(input_file,"%d %d\n", &m, &n);
        if(m==0&&n==0)
            break;
        itr++;
        fprintf(output_file,"# %d\n", itr);
        head=read_treeNode(m);
        process(&head, n);
        //delete_tree(&head);
    }
    fclose(input_file);
    fclose(output_file);
    return 0;
}
Node read_treeNode(int m){
    if(m==0)
        return NULL;
    Node head;
    Node tmp;
    Node node;
    int data;
    //head = (Node)malloc(sizeof(struct treeNode));
    /*head->llink = NULL;
    head->rlink = NULL;
    head->parent = NULL;*/
    head = NULL;
    for (int i = 0; i < m;i++){
        fscanf(input_file, "%d ", &data);
        node = (Node)malloc(sizeof(struct treeNode));//initialize the node
        node->llink = NULL;
        node->rlink = NULL;
        node->parent = NULL;
        node->data = data;
        //printf("%d ", head->data);
        if(head==NULL){
            head = node;
        }
        else{
            tmp = head;
            int flag = find_target(&tmp, data);
            if(flag==0){            //the number has existed in the tree
                free(node);
                continue;
            }
            node->parent = tmp;
            if(flag==-1)            //tmp->data>node->data
                tmp->llink = node;
            else                    //tmp->data<node->data
                tmp->rlink = node;
        }
    }
    //tmp = head;
    //print_tree(&tmp);
    //printf("\n");
    return head;
}
/*void print_tree(Node *tmp){
    if(!(*tmp))
        return;
    print_tree(&(*tmp)->llink);
    printf("%d ", (*tmp)->data);
    print_tree(&(*tmp)->rlink);
}*/
void process(Node *head,int n){
    int x, y;
    char instruction;
    for (int i = 0; i < n;i++){
        fscanf(input_file, "%c ", &instruction);
        switch (instruction){
        case 'I':
            //printf("IB\n");
            fscanf(input_file, "%d\n", &x);
            insert(head, x);
            //printf("IA\n");
            break;
        case 'D':
            //printf("DB\n");
            fscanf(input_file, "%d\n", &x);
            delete (head, x);
            //printf("DA\n");
            break;
        case 'Q':
            //printf("QB\n");
            fscanf(input_file, "%d\n", &x);
            int q=query(head, x);
            if(q>0)
                fprintf(output_file, "%d\n", q);
            //printf("QA\n");
            break;
        case 'P':
            //printf("PB\n");
            fscanf(input_file, "%d %d\n", &x, &y);
            int p=path(head, x, y);
            if(p>=0)
                fprintf(output_file, "%d\n", p);
            //printf("PA\n");
            break;
            }
    }
    //printf("end process\n");
    return;
}
void insert(Node *head,int x){
    Node node;
    node = (Node)malloc(sizeof(struct treeNode));   //initialize the node
    node->data = x;
    node->llink = NULL;
    node->rlink = NULL;
    node->parent = NULL;
    if(!(*head)){
        (*head) = node;
        return;
    }
    Node tmp;
    tmp = *head;
    int flag = find_target(&tmp, x);
    if(flag==0)                                 //the number has existed in tree
        return;
    /*if(!flag){
        free(node);
        return;
    }*/
    node->parent = tmp;
    if(flag==-1)
        tmp->llink = node;
    else
        tmp->rlink = node;
    return;
}
void delete(Node *head,int x){
    if(!(*head))
        return;
    Node tmp = *head;
    int flag = find_target(&tmp, x);
    if(flag!=0)                                 //can't find the number
        return;
    //printf("%d\n", tmp->data);
    if((!(tmp->rlink))&&(!(tmp->llink))){       //delete the leaf node
        if((tmp->data)>(tmp->parent->data))
            tmp->parent->rlink = NULL;
        else
            tmp->parent->llink = NULL;
        free(tmp);
    }
    else if(tmp->llink){
        Node subMax = tmp->llink;
        //subMax_data = tmp->llink->data;
        //Node tmpp=tmp->llink;
        //find_subMax(&tmpp, &subMax);
        //printf("%d\n", subMax->data);
        while(subMax->rlink)                //find subMax
            subMax = subMax->rlink;
        //tmp->data = subMax->data;
        if(subMax->llink){                  //update
            //printf("have left\n");
            if((subMax->data)>(subMax->parent->data)){
                subMax->parent->rlink = subMax->llink;
                subMax->llink->parent = subMax->parent;
            }
            else{
                subMax->parent->llink = subMax->llink;
                subMax->llink->parent = subMax->parent;
            }
        }
        else{                               //update
            if((subMax->data)>(subMax->parent->data))
                subMax->parent->rlink = NULL;
            else
                subMax->parent->llink = NULL;
        }
        tmp->data = subMax->data;
        //printf("%d\n", tmp->data);
        free(subMax);
    }
    else{
        Node subMin = tmp->rlink;
        //Node tmpp=tmp->rlink;
        //subMin_data = tmp->rlink->data;
        //find_subMin(&tmpp, &subMin);
        //printf("%d\n", subMin->data);
        while(subMin->llink)                //find subMin
            subMin = subMin->llink;
        //tmp->data = subMin->data;
        if(subMin->rlink){                  //update
            //printf("have right\n");
            if((subMin->data)>(subMin->parent->data)){
                subMin->parent->rlink = subMin->rlink;
                subMin->rlink->parent = subMin->parent;
            }
            else{
                subMin->parent->llink = subMin->rlink;
                subMin->rlink->parent = subMin->parent;
            }
        }
        else{
            if((subMin->data)>(subMin->parent->data))
                subMin->parent->rlink = NULL;
            else
                subMin->parent->llink = NULL;
        }
        tmp->data = subMin->data;
        //printf("%d\n", tmp->data);
        free(subMin);
    }
    tmp = *head;
    //print_tree(&tmp);
    return;
}
int query(Node *head,int x){
    //printf("%d ", x);
    if(!(*head))
        return 0;
    Node tmp = *head;
    int flag = find_target(&tmp, x);
    //printf("%d\n", flag);
    if(flag==0)                 //find the number
        return depth;
    return 0;
}
int path(Node *head,int x,int y){
    if(!(*head))
        return -1;
    Node start=*head;
    Node end = *head;
    int flag_start = find_target(&start, x);
    int flag_end = find_target(&end, y);
    if(flag_start||flag_end)
        return -1;
    if(start==end){
        if(start->data>=0)
            return start->data;
        else
            return 0;
    }
    int index_start = -1, index_end = -1;
    do{                     //use array the store the path
        start_path[++index_start] = start->data;
        start = start->parent;
    } while (start != NULL);
    do{
        end_path[++index_end] = end->data;
        end = end->parent;
    } while (end != NULL);
    while(((index_start-1)>=0)&&((index_end-1)>=0)){
        if(start_path[index_start-1]!=end_path[index_end-1])
            break;
        index_start--;
        index_end--;
    }
    int path_length = 0;
    for (int i = index_start - 1; i >= 0;i--){
        if(start_path[i]>0)
            path_length += start_path[i];
    }
    for (int i = index_end; i >= 0;i--){
        if(end_path[i]>0)
            path_length += end_path[i];
    } 
    //printf("p: %d\n", path_length);
    return path_length;
}
void delete_tree(Node *head){
    if(!(*head))
        return;
    delete_tree(&(*head)->llink);
    delete_tree(&(*head)->rlink);
    free(*head);
}
/*void print_tree(Node *head){
    int front = 0, rear = 0;
    Node ptr = *head;
    Node queue[200];
    if(!(head))
        return;
    queue[++rear] = ptr;
    for (;;){
        ptr = queue[++front];
        printf("%d ", ptr->data);
        if(ptr->llink)
            queue[++rear] = ptr->llink;
        if(ptr->rlink)
            queue[++rear] = ptr->rlink;
        if(front==rear)
            break;
    }
    printf("\n");
}*/
void find_subMax(Node *tmp,Node *target){
    if(!(*tmp))
        return;
    find_subMax((&(*tmp)->llink),target);
    find_subMax((&(*tmp)->rlink),target);
    if((*tmp)->data>subMax_data){
        target = tmp;
        subMax_data = (*tmp)->data;
    }
}
void find_subMin(Node *tmp,Node *target){
    if(!(*tmp))
        return;
    find_subMin((&(*tmp)->llink),target);
    find_subMin((&(*tmp)->rlink),target);
    if((*tmp)->data<subMin_data){
        target = tmp;
        subMin_data = (*tmp)->data;
    }
}
int find_target(Node *tmp,int data){    //find the target
    depth = 0;
    while(1){
        depth++;
        if(data>((*tmp)->data)){
            if((*tmp)->rlink)
                *tmp = (*tmp)->rlink;
            else
                return 1;
        }
        else if(data<((*tmp)->data)){
            if((*tmp)->llink)
                *tmp = (*tmp)->llink;
            else
                return -1;
        }
        else
            return 0;
    }
}