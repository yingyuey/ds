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
    if (S->top == MAXSIZE - 1) {   //根据数组下标的特点，当指针指向最后一个元素时为MAXSIZE-1，此时栈满
        printf("栈满，无法入栈\n");
        return ERROR;   //栈满说明空间已满已经不可以再入栈
    }
    else {    //如果栈非满则执行添加过程
        S->top++;   //栈顶指针+1指向一个新的顶部空间
        S->data[S->top] = e;   //将现在指向的这个新的空的栈顶空间元素置为指定元素（后进先出）
        return OK;
    }
}

//出栈
State pop(SqStack* S, SElemType* e) {
    if (S->top == -1) {   //当栈顶指针指向-1，说明栈空，则无法出栈
        printf("栈空，无法出栈\n");
        return ERROR;
    }
    else {   //如果栈非空则执行出栈程序
        *e = S->data[S->top];   //将当前栈顶元素的指针赋给可供返回查看的e
        S->top--;   //栈顶元素出栈后，栈顶指针向下走一格，表示新的栈顶元素
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

