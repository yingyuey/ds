constexpr auto MAXSIZE = 20;
constexpr auto OK = 1;
constexpr auto ERROR = 0;
#include <iostream>
#include"../../vector.xxx/vector.h"

template <typename T> class Stack :public Vector<T>
{
public:
    Rank _size;
    void push(T const& e) { insert(size(), e); }
    T pop() { return remove(size() - 1); }
    T& top() { return (*this)[size() - 1]; }
    Rank size() const { return _size; }
    bool empty() const { return !_size; }
};


typedef int State;
typedef int SElemType;

typedef struct {
    SElemType data[MAXSIZE];
    int top;
}SqStack;

State push(SqStack* S, SElemType e) {
    if (S->top == MAXSIZE - 1) {   //���������±���ص㣬��ָ��ָ�����һ��Ԫ��ʱΪMAXSIZE-1����ʱջ��
        printf("ջ�����޷���ջ\n");
        return ERROR;   //ջ��˵���ռ������Ѿ�����������ջ
    }
    else {    //���ջ������ִ����ӹ���
        S->top++;   //ջ��ָ��+1ָ��һ���µĶ����ռ�
        S->data[S->top] = e;   //������ָ�������µĿյ�ջ���ռ�Ԫ����Ϊָ��Ԫ�أ�����ȳ���
        return OK;
    }
}

//��ջ
State pop(SqStack* S, SElemType* e) {
    if (S->top == -1) {   //��ջ��ָ��ָ��-1��˵��ջ�գ����޷���ջ
        printf("ջ�գ��޷���ջ\n");
        return ERROR;
    }
    else {   //���ջ�ǿ���ִ�г�ջ����
        *e = S->data[S->top];   //����ǰջ��Ԫ�ص�ָ�븳���ɹ����ز鿴��e
        S->top--;   //ջ��Ԫ�س�ջ��ջ��ָ��������һ�񣬱�ʾ�µ�ջ��Ԫ��
        return OK;
    }
}

#define N_OPTR 9
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE }Operator;

const char pri[N_OPTR][N_OPTR] = {
    '>', '>', '<', '<', '<', '<', '<', '>', '>',
    '>', '>', '<', '<', '<', '<', '<', '>', '>',
    '>', '>', '>', '>', '<', '<', '<', '>', '>',
    '>', '>', '>', '>', '<', '<', '<', '>', '>',
    '<', '<', '<', '<', '<', '>', '>', '<', '>',
    '>', '>', '>', '>', '>', '>', ' ', '>', '>',
    '<', '<', '<', '<', '<', '<', '<', '=', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    '<', '<', '<', '<', '<', '<', '<', ' ', '='
};

