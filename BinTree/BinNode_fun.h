#pragma once
#include"BinNode.h"
#include"../homework/hw2/Stack.h"
/******************************************************************************************
 * BinNode״̬�����ʵ��ж�
******************************************************************************************/
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x)) //����ӵ��һ������
#define HasBothChild(x) (HasLChild(x) && HasRChild(x)) //ͬʱӵ����������
#define IsLeaf(x) (!HasChild(x))

/******************************************************************************************
 * ��BinNode�����ض���ϵ�Ľڵ㼰ָ��
******************************************************************************************/
#define sibling(p) (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->lc) /*�ֵ�*/
#define uncle(x) (sibling((x)->parent)) /*����*/
#define FromParentTo(x) /*���Ը��׵�����*/ \
( IsRoot(x) ? _root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) )


template <typename T> BinNodePosi<T> BinNode<T>::insertAsLC(T const& e)
   { return lc = new BinNode(e, this); } //��e��Ϊ��ǰ�ڵ�����Ӳ��������

 template <typename T> BinNodePosi<T> BinNode<T>::insertAsRC(T const& e)
   { return rc = new BinNode(e, this); } //��e��Ϊ��ǰ�ڵ���Һ��Ӳ��������

 //����
template <typename T> template < typename VST> //Ԫ�����͡�������
void BinNode<T>::travPre(VST& visit) { //������xian������㷨ͳһ���
     switch (rand() % 2) { 
     case 1: travPre_I(this, visit); break; //������
     default: travPre_R(this, visit); break; //�ݹ��
     }
}

//����
template <typename T> template < typename VST> //Ԫ�����͡�������
void BinNode<T>::travIn(VST & visit) { //��������������㷨ͳһ���
   switch (rand() % 5) { //�˴������ѡ���������ԣ�������ѡ��
      case 1: travIn_I1(this, visit); break; //������#1
      case 2: travIn_I2(this, visit); break; //������#2
      case 3: travIn_I3(this, visit); break; //������#3
      case 4: travIn_I4(this, visit); break; //������#4
      default: travIn_R(this, visit); break; //�ݹ��
	 }
 }

//����
template <typename T> template < typename VST> //Ԫ�����͡�������
void BinNode<T>::travPost(VST& visit) { //������hou������㷨ͳһ���
    switch (rand() % 2) { 
    case 1: travPost_I(this, visit); break; //������
    default: travPost_R(this, visit); break; //�ݹ��
    }
}


template <typename T> BinNodePosi<T> BinNode<T>::succ() { //��λ�ڵ�v��ֱ�Ӻ��
   BinNodePosi<T> s = this; //��¼��̵���ʱ����
   if (rc) { //�����Һ��ӣ���ֱ�Ӻ�̱����������У�����ؾ���
      s = rc; //��������
      while (HasLChild(*s)) s = s->lc; //�����С���Ľڵ�

    }
    else { //����ֱ�Ӻ��Ӧ�ǡ�����ǰ�ڵ���������������е�������ȡ�������ؾ���
      while (IsRChild(*s)) s = s->parent; //������������֧�����ϳ����Ϸ��ƶ�
      s = s->parent; //����ٳ����Ϸ��ƶ�һ�������ִ�ֱ�Ӻ�̣�������ڣ�

    }
   return s;
}

template <typename T> template < typename VST> //Ԫ�����͡�������
void BinNode <T>::travLevel(VST & visit) { //��������α����㷨
   Queue<BinNodePosi<T>> Q; Q.enqueue(this); //���븨�����У����ڵ����
   while (!Q.empty()) { //�ڶ����ٴα��֮ǰ����������
      BinNodePosi<T> x = Q.dequeue(); visit(x->data); //ȡ�����׽ڵ㲢����֮
      if (HasLChild(*x)) Q.enqueue(x->lc); //�������
      if (HasRChild(*x)) Q.enqueue(x->rc); //�Һ������
    }
}

template < typename T> //�ӵ�ǰ�ڵ�����������֧�������룬ֱ��û�����֧�Ľڵ�
static void goAlongVine(BinNodePosi<T> x, Stack<BinNodePosi<T>>&S) {
   while (x) { S.push(x); x = x->lc; } //��ǰ�ڵ���ջ���漴������֧���룬����ֱ��������

}

//����������ݹ�棩
template < typename T, typename VST> //Ԫ�����͡�������
void travPre_R(BinNodePosi<T> x, VST & visit) { //��������������㷨���ݹ�棩
   if (!x) return;
   visit(x->data);
   travPre_R(x->lc, visit);
   travPre_R(x->rc, visit);
}

//��������������棩
//�ӵ�ǰ�ڵ�����������֧�������룬ֱ��û�����֧�Ľڵ㣻��;�ڵ���������������
template < typename T, typename VST> //Ԫ�����͡�������
static void visitAlongVine(BinNodePosi<T> x, VST & visit, Stack<BinNodePosi<T>>&S) {
   while (x) {
      visit(x->data); //���ʵ�ǰ�ڵ�
      x = x->lc; //�����֧����һ��
    }
}
template < typename T, typename VST> //Ԫ�����͡�������
void travPre_I(BinNodePosi<T> x, VST & visit) { //��������������㷨��������#2��
   Stack<BinNodePosi<T>> S; //����ջ
   while (true) {
      visitAlongVine(x, visit, S); //�ӵ�ǰ�ڵ��������������
      if (S.empty()) break; //ֱ��ջ��
      x = S.pop(); //������һ�������
    }
}

//���������������#1��
template < typename T, typename VST> //Ԫ�����͡�������
void travIn_I1(BinNodePosi<T> x, VST & visit) { //��������������㷨��������#1��
   Stack<BinNodePosi<T>> S; //����ջ
   while (true) {
      goAlongVine(x, S); //�ӵ�ǰ�ڵ������������ջ
      if (S.empty()) break; //ֱ�����нڵ㴦�����
      x = x->rc; //ת��������
    }
}

//���������������#2��
template < typename T, typename VST> //Ԫ�����͡�������
void travIn_I2(BinNodePosi<T> x, VST & visit) { //��������������㷨��������#2��
Stack<BinNodePosi<T>> S; //����ջ
   while (true)
      if (x) {
         S.push(x); //���ڵ��ջ
         x = x->lc; //�������������
    }
        else if (!S.empty()) {
         x = S.pop(); //��δ���ʵ�������Ƚڵ���ջ
         visit(x->data); //���ʸ����Ƚڵ�
         x = x->rc; //�������ȵ�������
    }
        else
         break; //�������
    0014
}

//���������������#3��
template < typename T, typename VST> //Ԫ�����͡�������
void travIn_I3(BinNodePosi<T> x, VST & visit) { //��������������㷨��������#3�����踨��ջ��
   bool backtrack = false; //ǰһ���Ƿ�մ����������ݡ���ʡȥջ����O(1)�����ռ�
   while (true)
      if (!backtrack && HasLChild(*x)) //�����������Ҳ��Ǹոջ��ݣ���
         x = x->lc; //�������������
      else { //���򡪡�����������ոջ��ݣ��൱������������
         visit(x->data); //���ʸýڵ�
         if (HasRChild(*x)) { //�����������ǿգ���
            x = x->rc; //������������������
            backtrack = false; //���رջ��ݱ�־
        }
        else { //���������գ���
            if (!(x = x->succ())) break; //���ݣ����ִ�ĩ�ڵ�ʱ���˳����أ�
            backtrack = true; //�����û��ݱ�־
        }
    }
}

//����������ݹ�棩
template < typename T, typename VST> //Ԫ�����͡�������
void travIn_R(BinNodePosi<T> x, VST & visit) { //��������������㷨���ݹ�棩
   if (!x) return;
   travIn_R(x->lc, visit);
   visit(x->data);
   travIn_R(x->rc, visit);
}

//����������ݹ�棩
template < typename T, typename VST> //Ԫ�����͡�������
void travPost_R(BinNodePosi<T> x, VST & visit) { //��������������㷨���ݹ�棩
   if (!x) return;
   travPost_R(x->lc, visit);
   visit(x->data);
}

//��������������棩
template < typename T> //����Sջ���ڵ�Ϊ���������У��ҵ�������ɼ�Ҷ�ڵ�
static void gotoLeftmostLeaf(Stack<BinNodePosi<T>>&S) { //��;�����ڵ�������ջ
   while (BinNodePosi<T> x = S.top()) //�Զ����£�������鵱ǰ�ڵ㣨��ջ����
     if (HasLChild(*x)) { //����������
         if (HasRChild(*x)) S.push(x->rc); //�����Һ��ӣ�������ջ
         S.push(x->lc); //Ȼ���ת������
     }
     else //ʵ������
         S.push(x->rc); //������
   S.pop(); //����֮ǰ������ջ���Ŀսڵ�
}
template < typename T, typename VST>
void travPost_I(BinNodePosi<T> x, VST & visit) { //�������ĺ�������������棩
   Stack<BinNodePosi<T>> S; //����ջ
   if (x) S.push(x); //���ڵ���ջ
   while (!S.empty()) { // xʼ��Ϊ��ǰ�ڵ�
      if (S.top() != x->parent) ////��ջ����x֮������Ϊ���֣�
         gotoLeftmostLeaf(S); //�����������������ҵ�HLVFL���൱�ڵݹ����룩
      x = S.pop(); visit(x->data); //����ջ������ǰһ�ڵ�֮��̣���������֮
    }
}