#include"BinTree.h"
template <typename T> Rank BinTree<T>::updateHeight(BinNodePosi<T> x) //更新节点x高度
{ return x->height = 1 + max(stature(x->lc), stature(x->rc)); } //具体规则，因树而异

template <typename T> void BinTree<T>::updateHeightAbove(BinNodePosi<T> x) //更新高度
{ while (x) { updateHeight(x); x = x->parent; } } //从x出发，覆盖历代祖先。可优化

template <typename T> BinNodePosi<T> BinTree<T>::insert(T const& e)
{ _size = 1; return _root = new BinNode<T>(e); } //将e当作根节点插入空的二叉树

template <typename T> BinNodePosi<T> BinTree<T>::insert(T const& e, BinNodePosi<T> x)
{ _size++; x->insertAsLC(e); updateHeightAbove(x); return x->lc; } // e插入为x的左孩子

template <typename T> BinNodePosi<T> BinTree<T>::insert(BinNodePosi<T> x, T const& e)
{ _size++; x->insertAsRC(e); updateHeightAbove(x); return x->rc; } // e插入为x的右孩子

template < typename T> //将S当作节点x的左子树接入二叉树，S本身置空
BinNodePosi<T> BinTree<T>::attach(BinTree<T>*&S, BinNodePosi<T> x) { // x->lc == NULL
   if (x->lc = S->_root) x->lc->parent = x; //接入
   _size += S->_size; updateHeightAbove(x); //更新全树规模与x所有祖先的高度
    S->_root = NULL; S->_size = 0; release(S); S = NULL; return x; //释放原树，返回接入位置

}

template <typename T> //将S当作节点x的右子树接入二叉树，S本身置空
BinNodePosi<T> BinTree<T>::attach(BinNodePosi<T> x, BinTree<T>*&S) { // x->rc == NULL
   if (x->rc = S->_root) x->rc->parent = x; //接入
   _size += S->_size; updateHeightAbove(x); //更新全树规模与x所有祖先的高度
   S->_root = NULL; S->_size = 0; release(S); S = NULL; return x; //释放原树，返回接入位置
}

template < typename T> //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
Rank BinTree<T>::remove(BinNodePosi<T> x) { // assert: x为二叉树中的合法位置
   FromParentTo(*x) = NULL; //切断来自父节点的指针
   updateHeightAbove(x->parent); //更新祖先高度
   Rank n = removeAt(x); _size -= n; return n; //删除子树x，更新规模，返回删除节点总数
}

template <typename T> //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
static Rank removeAt(BinNodePosi<T> x) { // assert: x为二叉树中的合法位置
   if (!x) return 0; //递归基：空树
   Rank n = 1 + removeAt(x->lc) + removeAt(x->rc); //递归释放左、右子树
   release(x->data); release(x); return n; //释放被摘除节点，并返回删除节点总数
} // release()负责释放复杂结构，与算法无直接关系，具体实现详见代码包

template < typename T> //二叉树子树分离算法：将子树x从当前树中摘除，将其封装为一棵独立子树返回
BinTree<T>*BinTree<T>::secede(BinNodePosi<T> x) { // assert: x为二叉树中的合法位置
   FromParentTo(*x) = NULL; //切断来自父节点的指针
   updateHeightAbove(x->parent); //更新原树中所有祖先的高度
   BinTree<T>*S = new BinTree<T>; S->_root = x; x->parent = NULL; //新树以x为根
   S->_size = x->size(); _size -= S->_size; return S; //更新规模，返回分离出来的子树
}

