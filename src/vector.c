/* vector.c —— 你要实现的地方 */

#include "vector.h"
#include<stdlib.h>

int vector_init(vector *v, size_t capacity) {
    if(v==NULL){
        return -1;
    }
    v->data=NULL;
    v->end=NULL;
    v->cap=NULL;
    if(capacity==0){
        return 0;
    }
    if(capacity>SIZE_MAX/sizeof(int)){
        return -1;
    }
    v->data=(int *)malloc(capacity*sizeof(int));
    if(v->data==NULL){
        return -1;       
    }
    v->end=v->data;
    v->cap=v->data+capacity;
    return 0;
}

void vector_destroy(vector *v) {
    if(v==NULL){
        return;
    }
    free(v->data);
    v->data=NULL;
    v->end=NULL;
    v->cap=NULL;
}

size_t size(const vector *v) {
    if(v==NULL||v->data==NULL){
        return 0;
    }
    return (size_t)(v->end-v->data);
}

size_t capacity(const vector *v) {
    if(v==NULL||v->data==NULL){
        return 0;
    }
    return (size_t)(v->cap - v->data);
}

int empty(const vector *v) {
    return size(v)==0;
}

int get(const vector *v, size_t index, int *out) {
    if(v==NULL||v->data==NULL||out==NULL){
        return -1;
    }
    if(index >=(size_t)(v->end - v->data)){
        return -1;
    }
    *out=*(v->data+index);
    return 0;
}

int set(vector *v, size_t index, int value) {
    if(v==NULL||v->data==NULL){
        return -1;
    }
    if(index >=(size_t)(v->end - v->data)){
        return -1;
    }
    *(v->data+index)=value;
    return 0;
}

int front(const vector *v, int *out) {
    return get(v,0,out);
}

int back(const vector *v, int *out) {
    size_t s=size(v);
    if(s==0){
        return -1;
    }
    return get(v,s-1,out);
}

int push_back(vector *v, int value) {
    if(v==NULL){
        return -1;
    }
    if(v->end==v->cap){
      size_t old_size=v->end - v->data;
      size_t new_capacity=(old_size==0)?1:(v->cap - v->data)*2;
      if(new_capacity>SIZE_MAX/sizeof(int)){
        return -1;
      }
       int *new_data=(int*)realloc(v->data,new_capacity*sizeof(int));
       if(new_data==NULL){
        return -1;
      }
       v->data=new_data;
       v->end=new_data+old_size;
       v->cap=new_data+new_capacity;
     }
   *(v->end)=value;
    v->end++;
    return 0;
}

int pop_back(vector *v, int *out) {
    if(v==NULL||v->data==NULL||out==NULL){
        return -1;
    }
    if(v->end==v->data){
        return -1;
    }
    *out=*(v->end -1);
    v->end--;
    return 0;
}

int reserve(vector *v, size_t new_capacity) {
    if(v==NULL){
        return -1;
    }
    size_t capacity;
    size_t old_size;
    if(v->data==NULL){
        capacity=0;
        old_size=0;
    }else if(v->data!=NULL){
        capacity=v->cap - v->data;
        old_size=v->end - v->data;
    }
    if(new_capacity<=capacity){
        return 0;
    }
    if(new_capacity>SIZE_MAX/sizeof(int)){
        return -1;
    }
    int *new_data =(int *)realloc(v->data,new_capacity*sizeof(int));
    if(new_data==NULL){
        return -1;
    }
    v->data=new_data;
    v->end=new_data+old_size;
    v->cap=new_data+new_capacity;
    return 0;
}

int shrink_to_fit(vector *v) {
    if(v==NULL){
        return -1;
    }
    if(v->data==NULL){
        return 0;
    }
    size_t old_size=v->end - v->data;
    if(old_size==0){
        free(v->data);
        v->data=NULL;
        v->end=NULL;
        v->cap=NULL;
        return 0;
    }
    if((size_t)(v->cap - v->data)==old_size){
        return 0;
    }
    int *new_data=(int*)realloc(v->data,old_size*sizeof(int));
    if(new_data==NULL){
        return -1;
    }
    v->data=new_data;
    v->end=new_data+old_size;
    v->cap=new_data+old_size;
    return 0;

}

void clear(vector *v) {
    if(v==NULL||v->data==NULL){
        return;
    }
    v->end=v->data;
}
