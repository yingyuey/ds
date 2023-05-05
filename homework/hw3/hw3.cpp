#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include"../../BinTree/BinTree.h"
//#include"../../vector.xxx/vector.h"
#include "../../Bitmap/Bitmap.h" //ʹ��λͼ�ṹʵ�ֶ����Ʊ��봮
#include<iostream>
#include<stdio.h>
#define HuffTree BinTree<HuffChar>

#define  N_CHAR (0x80 - 0x20) //���Կɴ�ӡ�ַ�Ϊ��
struct HuffChar { //Huffman�������ַ�
	char ch; unsigned int weight; //�ַ���Ƶ��
	HuffChar(char c = '^', unsigned int w = 0) : ch(c), weight(w) {};
	// �Ƚ������е�����������һ���������в��䣩
	bool operator< (HuffChar const& hc) { return weight > hc.weight; } //�˴������С�ߵ�
	bool operator== (HuffChar const& hc) { return weight == hc.weight; }
};

int* statistics(char* sample_text_file) { //ͳ���ַ�����Ƶ��
	int* freq = new int[N_CHAR];  //����ͳ����������ʣ����������¼���ַ����ִ���
	memset(freq, 0, sizeof(int) * N_CHAR); //����
	FILE* fp = fopen("I_have_a_dream.txt", "r"); //assert: �ļ������ҿ���ȷ��
	for (char ch; 0 < fscanf(fp, "%c", &ch); ) //���ɨ�������ļ��е�ÿ���ַ�
		if (ch >= 0x20) freq[ch - 0x20]++; //�ۼƶ�Ӧ�ĳ��ִ���
	fclose(fp); return freq;
}

//#include ".././../List/List.h" //��Listʵ��
//using HuffForest = List<HuffTree*>; //Huffmanɭ��

typedef Bitmap HuffCode;

//��ʼ��
//HuffForest * initForest(int* freq) 
//{ //����Ƶ��ͳ�Ʊ�Ϊÿ���ַ�����һ����
//   HuffForest * forest = new HuffForest; //��Listʵ�ֵ�Huffmanɭ��
//   for (int i = 0; i < N_CHAR; i++) { //Ϊÿ���ַ�
//      forest->insertAsLast(new HuffTree); //����һ�����������ַ�����Ƶ��
//      forest->last()->data->insert(HuffChar(0x20 + i, freq[i])); //��������
//	}
//   return forest;
//}

//#include"../../List/ListNode.h"
//HuffTree* minHChar(HuffForest* forest) {
	//�� Huffman ɭ�����ҳ�Ȩ����С�ģ������ַ�
//	ListNodePosi(HuffTree*) p = forest->first(); //���׽ڵ��������
//	ListNodePosi(HuffTree*) minChar = p; //��С Huffman �����ڵĽڵ�λ��
//	int minweight = p->data->root()->data.weight; //Ŀǰ����СȨ�� while (forest->valid(p = p->succ))
////�������нڵ�
//	if (minweight > p->data->root()->data.weight) //����ǰ�ڵ���������С����
//	{ minweight = p->data->root()->data.weight; minChar = p; } //���¼�¼
//	return forest->remove(minChar);//����ѡ���� Huffman ����ɭ����ժ����������
//}
//HuffTree* generateTree(HuffForest* forest)
//{// Huffman �����㷨
	//while (1 < forest->size()) {
	//	HuffTree* T1 = minHChar(forest); HuffTree* T2 = minHChar(forest);
	//	HuffTree* S = new HuffTree();
	//	S->insertAsRoot(HuffChar('^', T1->root()->data.weight + T2->root()->data.weight));
	//	S->attach(S->root(), T1); S->attach(S->root(), T2); forest->insertAsLast(5);
	//}
	//// assert ��ѭ������ʱ��ɭ����Ψһ���б��׽ڵ��У����ǿ����� Huffman ������
	//return forest->first()->data;
//}

//static void 
//generateCT (Bitmap * code, int length, HuffTable * table, BinNodePosi <HuffChar> v) {
//  if (IsLeaf(*v)) //����Ҷ�ڵ㣨���ж��ַ��������жϣ�
//      {  table->put(v->data.ch, code->bits2string(length)); return;  }
//   if (HasLChild(*v)) //Left = 0
//      { code->clear(length); generateCT(code, length + 1, table, v->lc); }
//   if (HasRChild(*v)) //Right = 1
//   { code->set(length); generateCT(code, length + 1, table, v->rc); }
//}
//HuffTable * generateTable(HuffTree * tree) { //�����ַ�����ͳһ������ɢ�б�ʵ�ֵı������
//   HuffTable * table = new HuffTable; Bitmap* code = new Bitmap;
//   generateCT(code, 0, table, tree->root()); release(code); return table;
//}; //release()�����ͷŸ��ӽṹ�����㷨��ֱ�ӹ�ϵ������ʵ����������

// int encode(HuffTable* table, Bitmap* codeString, char* s) { //���ձ�����Bitmap������
//   int n = 0; //�����صı��봮�ܳ�n
//  for (size_t m = strlen(s), i = 0; i < m; i++) { //���������е�ÿ���ַ�
//     char** pCharCode = table->get(s[i]); //ȡ�����Ӧ�ı��봮
//      if (!pCharCode) pCharCode = table->get(s[i] + 'a' - 'a'); //Сд��ĸתΪ��д
//      if (!pCharCode) pCharCode = table->get(' '); //�޷�ʶ����ַ�ͳһ�����ո�
//      printf("%s", *pCharCode); //�����ǰ�ַ��ı���
//      for (size_t m = strlen(*pCharCode), j = 0; j < m; j++) //����ǰ�ַ��ı��������봮
//		  '1' == *(*pCharCode + j) ? codeString->set(n++) : codeString->clear(n++);
//	}
//  printf("\n"); return n;
//} //�����Ʊ��봮��¼��λͼcodeString��

int main(int argc, char* argv[])
{ //Huffman�����㷨ͳһ����
	int* freq = statistics(argv[1]); //���������ļ���ͳ�Ƹ��ַ��ĳ���Ƶ��
	for (char ch = 0x41; ch <= 0x5A; ch++)
	{
		cout << char(ch - 0x61 + 'A' + 32) << ":" << freq[ch] << endl;
	}
	//HuffForest* forest = initForest(freq); release(freq); //����Huffmanɭ��
	//HuffTree* tree = generateTree(forest); release(forest); //����Huffman������
  //  HuffTable * table = generateTable(tree); //��Huffman������ת��Ϊ�����
  //  for (int i = 2; i < argc; i++) 
  //  { //���������д����ÿһ���Ĵ�
  //     Bitmap * codeString = new Bitmap; //�����Ʊ��봮
	 //  int n = encode(table, codeString, argv[i]); //�����ݱ�������ɣ�����Ϊn��
	 //  decode(tree, codeString, n); //����Huffman���������Գ���Ϊn�Ķ����Ʊ��봮����
	 //  release(codeString);
	 //}
  //  release(table); release(tree); return 0; //�ͷű����������
} //release()�����ͷŸ��ӽṹ�����㷨��ֱ�ӹ�ϵ������ʵ����������
