#pragma once
#include "BinNode.h" //����������ڵ���
template <typename T> class BinTree { //������ģ����
protected:
   Rank _size; BinNodePosi<T> _root; //��ģ�����ڵ�
   virtual Rank updateHeight(BinNodePosi<T> x); //���½ڵ�x�ĸ߶�
   void updateHeightAbove(BinNodePosi<T> x); //���½ڵ�x�������ȵĸ߶�
public:
   BinTree() : _size(0), _root(NULL) {} //���캯��
   ~BinTree() { if (0 < _size) remove(_root); } //��������
   Rank size() const { return _size; } //��ģ
   bool empty() const { return !_root; } //�п�
   BinNodePosi<T> root() const { return _root; } //����
   BinNodePosi<T> insert(T const&); //������ڵ�
   BinNodePosi<T> insert(T const&, BinNodePosi<T>); //��������
   BinNodePosi<T> insert(BinNodePosi<T>, T const&); //�����Һ���
   BinNodePosi<T> attach(BinTree<T>*&, BinNodePosi<T>); //����������
   BinNodePosi<T> attach(BinNodePosi<T>, BinTree<T>*&); //����������
   Rank remove(BinNodePosi<T>); //����ɾ��
   BinTree<T>* secede(BinNodePosi<T>); //��������
   template < typename VST> //������
   void travLevel(VST & visit) { if (_root) _root->travLevel(visit); } //��α���
   template < typename VST> //������
   void travPre(VST & visit) { if (_root) _root->travPre(visit); } //�������
   template < typename VST> //������
   void travIn(VST & visit) { if (_root) _root->travIn(visit); } //�������
   template < typename VST> //������
   void travPost(VST & visit) { if (_root) _root->travPost(visit); } //�������
   bool operator<(BinTree<T> const& t) //�Ƚ������������в��䣩
      { return _root && t._root && lt(_root, t._root); }
   bool operator==(BinTree<T> const& t) //�е���
      { return _root && t._root && (_root == t._root); }
}; //BinTree