//int number[101], i = 0, p = 1;
//char symbol[101], s[256], t[256];
//void push()
//{
//	symbol[++p] = s[i];
//}
//void pop()
//{
//	switch (symbol[p--])
//	{
//	    case'+':number[p] += number[p + 1]; break;
//		case'-':number[p] -= number[p + 1]; break;
//		case'*':number[p] *= number[p + 1]; break;
//		case'/':number[p] /= number[p + 1]; break;
//	}
//}
//bool can()
//{
//	if ((s[i] == '+' || s[i] == '-') && symbol[p] != '(') return 1;
//	if ((s[i] == '*' || s[i] == '/')
//		&& (symbol[p] == '*' || symbol[p] == '/'))return 1;
//	return 0;
//}
//
//int main()
//{
//	printf("string:"); gets_s(s);
//	s[strlen(s)] = ')'; symbol[p] = '(';
//	while (i < strlen(s))
//	{
//		while (s[i] == '(')
//		{
//			push(); i++;
//		}
//		int x = 0;
//		while (s[i] >= '0' && s[i] <= '9')
//			x = x * 10 + s[i++] - '0';
//		number[p] = x;
//		do
//		{
//			if (s[i] == ')')
//			{
//				while (symbol[p] != '(')pop();
//				number[--p] = number[p + 1];
//			}
//			else
//			{
//				while (can())pop();
//				push();
//			}
//			i++;
//		} while (i < strlen(s) && s[i - 1] == ')');
//	}
//	printf("Result=%d", number[0]);
//	return 0;
//}

#pragma warning(disable : 4996)
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctype.h>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS
#include"Stack.h"


//����ƥ���㷨
bool paren(const char exp[], int lo, int hi)
{
	Stack<char> S;
	for(int i=lo;i<=hi;i++)
		switch (exp[i])
		{
		case'(':case'[':case'{':S.push(exp[i]); break;
		case')':if ((S.empty()) || ('(' != S.pop()))return false; break;
		case']':if ((S.empty()) || ('[' != S.pop()))return false; break;
		case'}':if ((S.empty()) || ('{' != S.pop()))return false; break;
		default:break;
		}
	return S.empty();
}

//readNumber
void readNumber(char*& p, Stack<double>& stk)
{
	stk.push((double)(*p - '0'));//ת��Ϊfloat��ջ
	while (isdigit(*(++p)))//��λ�����
		stk.push(stk.pop() * 10 + (*p - '0'));
	if ('.' != *p)return;//ֻ��������������ջ
	float fraction = 1;//ʮλС��->��λС��
	while (isdigit(*(++p)))
		stk.push(stk.pop() * (*p - '0') * (fraction /= 10));
}

//oderBetween
Operator optr2rank(char op)
{
	switch (op) {
	case '+': return ADD;
	case '-': return SUB;
	case '*': return MUL;
	case '/': return DIV;
	case '^': return POW;
	case '!': return FAC;
	case '(': return L_P;
	case ')': return R_P;
	case '\0': return EOE;//��ʼ������ֹ��
	default:exit(-1);//δ֪�����
	}
}
char orderBetween(char op1, char op2)//�Ƚ������֮������ȼ�
{
	return pri[optr2rank(op1)][optr2rank(op2)];
}

//append
void append(char*& rpn, double opnd)//������������RPNĩβ
{
	int n=strlen(rpn); //RPN��ǰ����
	char buf[64];
	if (opnd != (float)(int)opnd) 
		sprintf_s(buf, "%.2f10", opnd);//�����ʽ��������ʽ
	else 
		sprintf_s(buf, " %d \0", (int)opnd);
	rpn = (char*)realloc(rpn, sizeof(char) * (n+strlen(buf) + 1)); 
	strcat(rpn, buf);//��չ�ռ�
}
void append(char*& rpn, char optr) {
	int n = strlen(rpn);
	rpn = (char*)realloc(rpn, sizeof(char) * (n + 3));
	sprintf(rpn + n, "%c", optr); 
	rpn[n + 2] = '\0';//����ָ���������
}

//calcu
double calcu(char op, double pOp)
{
	float t = 1;
	while (pOp > 0)
	{
		t *= pOp;
		pOp--;
	}
	return t;
}
double calcu(double a, char op, double b)
{
	switch (op)
	{
	case'+':return a + b; cout<<a<<'+'<<b<<'='<<a+b<<endl; break;
	case'-':return a - b; break;
	case'*':return a * b; break;
	case'/': {
		if (0 == b)
		{
			exit(-1);
		}
		else {
			return a / b;
		}break;
	}
	case'^':return pow(a, b); break;
	default:exit(-1);
	}
}

//��ֵ�㷨
double evaluate(char* S, char*& RPN)
{
	Stack<double> opnd; Stack<char> optr;//������ջ�������ջ
	char* expr = S;
	optr.push('\0');
	while (!optr.empty())//�������ջ�ǿ�֮ǰ�����������ʽ�еĸ��ַ�
	{
		if (isdigit(*S)) {//����ǰ�ַ�Ϊ������
			readNumber(S, opnd); 
			append(RPN, opnd.top());//���������������RPNĩβ
		}
		else 
		{//����=Ϊ�����
			switch (orderBetween(optr.top(), *S))
			{
			case'<':optr.push(*S); S++; break;//�Ƴ٣��������ջ
			case'=':optr.pop(); S++; break;//��ȣ������Ž�����һ�ַ�
			case'>': 
			{//���ȼ����ߣ����㣬���������ջ
				char op = optr.pop(); append(RPN, op);
				if ('!' == op) 
				{//����һԪ�����
					float pOpnd = opnd.pop();//ֻ��ȡ��һ�������� 
					opnd.push(calcu(op, pOpnd));//һԪ��������ջ
				}
				else 
				{
					float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop();
					opnd.push(calcu(pOpnd1, op, pOpnd2));
				}
				break;
			}
			default:exit(-1);
			}
		}
	}
	return opnd.pop();
}

char* removeSpace(char* s)
{ //�޳�s[]�еİ׿ո�
	char* p = s, * q = s;
	while (true)
	{
		while (isspace(*q))
		{
			q++;
		}
		if ('\0' == *q)
		{
			*p = '\0';
			return s;
		}
		*p++ = *q++;
	}
}


int main()
{
	//1
	char* rpn = (char*)malloc(sizeof(char) * 1);
	rpn[0] = '\0';
	char s[50];
	cin >> s;
	double value=evaluate(s, rpn);
	cout << value << endl;
	free(rpn);
	rpn = NULL;
	system("pause");

	return 0;
}