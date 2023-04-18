 #include "ListNode.h" //引入列表节点类

template <typename T> class List 
{ //列表模板类
private:
    Rank _size; ListNodePosi<T> header, trailer; //规模、头哨兵、尾哨兵
protected:
    void init(); //列表创建时的初始化
    Rank clear(); //清除所有节点
    void copyNodes(ListNodePosi<T>, Rank); //复制列表中自位置p起的n项
    ListNodePosi<T> merge(ListNodePosi<T>, Rank, List<T>&, ListNodePosi<T>, Rank); //归并
    void mergeSort(ListNodePosi<T>&, Rank); //对从p开始连续的n个节点归并排序
    void selectionSort(ListNodePosi<T>, Rank); //对从p开始连续的n个节点选择排序
	void insertionSort(ListNodePosi<T>, Rank); //对从p开始连续的n个节点插入排序
    void radixSort(ListNodePosi<T>, Rank); //对从p开始连续的n个节点基数排序

 public:
// 构造函数
    List() { init(); } //默认
    List(List<T> const& L); //整体复制列表L
    List(List<T> const& L, Rank r, Rank n); //复制列表L中自第r项起的n项
	List(ListNodePosi<T> p, Rank n); //复制列表中自位置p起的n项
    // 析构函数
    ~List(); //释放（包含头、尾哨兵在内的）所有节点
 // 只读访问接口
    Rank size() const { return _size; } //规模
    bool empty() const { return _size <= 0; } //判空
    ListNodePosi<T> operator[](Rank r) const; //重载，支持循秩访问（效率低）
    ListNodePosi<T> first() const { return header->succ; } //首节点位置
    ListNodePosi<T> last() const { return trailer->pred; } //末节点位置
    bool valid(ListNodePosi<T> p) //判断位置p是否对外合法
    { return p && (trailer != p) && (header != p); } //将头、尾节点等同于NULL
    ListNodePosi<T> find(T const& e) const //无序列表查找
    { return find(e, _size, trailer); }
    ListNodePosi<T> find(T const& e, Rank n, ListNodePosi<T> p) const; //无序区间查找
    ListNodePosi<T> search(T const& e) const //有序列表查找
	{ return search(e, _size, trailer); }
    ListNodePosi<T> search(T const& e, Rank n, ListNodePosi<T> p) const; //有序区间查找
    ListNodePosi<T> selectMax(ListNodePosi<T> p, Rank n); //在p及其n-1个后继中选出最大者
    ListNodePosi<T> selectMax() { return selectMax(header->succ, _size); } //整体最大者
 // 可写访问接口
    ListNodePosi<T> insertAsFirst(T const& e); //将e当作首节点插入
    ListNodePosi<T> insertAsLast(T const& e); //将e当作末节点插入
    ListNodePosi<T> insert(ListNodePosi<T> p, T const& e); //将e当作p的后继插入
    ListNodePosi<T> insert(T const& e, ListNodePosi<T> p); //将e当作p的前驱插入
    T remove(ListNodePosi<T> p); //删除合法位置p处的节点,返回被删除节点
    void merge(List<T>& L) { merge(header->succ, _size, L, L.header->succ, L._size); } //全列表归并
    void sort(ListNodePosi<T>, Rank); //列表区间排序
    void sort() { sort(first(), _size); } //列表整体排序
    Rank deduplicate(); //无序去重
    Rank uniquify(); //有序去重
    void reverse(); //前后倒置（习题）
 // 遍历
    void traverse(void (*)(T&)); //依次实施visit操作（函数指针）
    template <typename VST> void traverse(VST&); //依次实施visit操作（函数对象）
}; //List

 template <typename T> void List<T>::init() { //列表初始化，在创建列表对象时统一调用
     header = new ListNode<T>; trailer = new ListNode<T>; //创建头、尾哨兵节点
     header->succ = trailer; header->pred = NULL; //向前链接
     trailer->pred = header; trailer->succ = NULL; //向后链接
     _size = 0; //记录规模
}
 template < typename T> //重载下标操作符，以通过秩直接访问列表节点（O(r)效率，虽方便，勿多用）
  ListNodePosi<T> List<T>::operator[](Rank r) const { // 0 <= r < size
  ListNodePosi<T> p = first(); //从首节点出发
  while (0 < r--) p = p->succ; //顺数第r个节点即是
  return p; //目标节点
 }

 template < typename T> //在无序列表内节点p（可能是trailer）的n个（真）前驱中，找到等于e的最后者
 ListNodePosi<T> List<T>::find(T const& e, Rank n, ListNodePosi<T> p) const {
     while (0 < n--) //（0 <= n <= Rank(p) < _size）对于p的最近的n个前驱，从右向左
       if (e == (p = p->pred)->data) return p; //逐个比对，直至命中或范围越界
     return NULL; //p越出左边界意味着区间内不含e，查找失败
 } //失败时，返回NULL

 template <typename T> ListNodePosi<T> List<T>::insertAsFirst(T const& e)
    { _size++; return header->insertAsSucc(e); } // e当作首节点插入
 template <typename T> ListNodePosi<T> List<T>::insertAsLast(T const& e)
    { _size++; return trailer->insertAsPred(e); } // e当作末节点插入
 template <typename T> ListNodePosi<T> List<T>::insert(ListNodePosi<T> p, T const& e)
    { _size++; return p->insertAsSucc(e); } // e当作p的后继插入
 template <typename T> ListNodePosi<T> List<T>::insert(T const& e, ListNodePosi<T> p)
    { _size++; return p->insertAsPred(e); } // e当作p的前驱插入

 template < typename T> //将e紧靠当前节点之前插入于当前节点所属列表（设有哨兵头节点header）
 ListNodePosi<T> ListNode<T>::insertAsPred(T const& e) {
    ListNodePosi<T> x = new ListNode(e, pred, this); //创建新节点
    pred->succ = x; pred = x; //设置正向链接
    return x; //返回新节点的位置
 }

 template < typename T> //将e紧随当前节点之后插入于当前节点所属列表（设有哨兵尾节点trailer）
 ListNodePosi<T> ListNode<T>::insertAsSucc(T const& e) {
    ListNodePosi<T> x = new ListNode(e, this, succ); //创建新节点
    succ->pred = x; succ = x; //设置逆向链接
    return x; //返回新节点的位置
 }

 template < typename T> //列表内部方法：复制列表中自位置p起的n项
 void List<T>::copyNodes(ListNodePosi<T> p, Rank n) { // p合法，且至少有n-1个真后继
    init(); //创建头、尾哨兵节点并做初始化
    while (n--) { insertAsLast(p->data); p = p->succ; } //将起自p的n项依次作为末节点插入
 }

 template < typename T> //复制列表中自位置p起的n项（assert: p为合法位置，且至少有n-1个后继节点）
 List<T>::List(ListNodePosi<T> p, Rank n) { copyNodes(p, n); }
 template < typename T> //整体复制列表L
 List<T>::List(List<T> const& L) { copyNodes(L.first(), L._size); }
 template <typename T> //复制L中自第r项起的n项（assert: r+n <= L._size）
 List<T>::List(List<T> const& L, Rank r, Rank n) {
    ListNodePosi<T> p = L.first();
    while (0 < r--) p = p->succ;
    copyNodes(p, n);
 }

 template <typename T> T List<T>::remove(ListNodePosi<T> p) { //删除合法节点p，返回其数值
    T e = p->data; //备份待删除节点的数值（假定T类型可直接赋值）
    p->pred->succ = p->succ; p->succ->pred = p->pred; //后继、前驱
    delete p; _size--; //释放节点，更新规模
    return e; //返回备份的数值
 }

 template <typename T> List<T>::~List() //列表析构器
 { clear(); delete header; delete trailer; } //清空列表，释放头、尾哨兵节点

 template <typename T> Rank List<T>::clear() { //清空列表
    Rank oldSize = _size;
    while (0 < _size) remove(header->succ); //反复删除首节点，直至列表变空
    return oldSize;
 }

 template <typename T> Rank List<T>::deduplicate() {
    Rank oldSize = _size; ListNodePosi<T> p = first();
    for (Rank r = 0; p != trailer; p = p->succ) //O(n)
      if (ListNodePosi<T> q = find(p->data, r, p))
          remove(q); //此时q与p雷同，但删除前者更为简明
       else r++; //r为无重前缀的长度
    return oldSize - _size; //删除元素总数
 }

 template <typename T> void List<T>::traverse(void (*visit)(T&)) //借助函数指针机制遍历
 {  for (ListNodePosi<T> p = header->succ; p != trailer; p = p->succ) visit(p->data);  }
 template <typename T> template < typename VST> //元素类型、操作器
 void List<T>::traverse(VST & visit) //借助函数对象机制遍历
 {  for (ListNodePosi<T> p = header->succ; p != trailer; p = p->succ) visit(p->data);  }

 template <typename T> Rank List<T>::uniquify() { //成批剔除重复元素，效率更高
    if (_size < 2) return 0; //平凡列表自然无重复
    Rank oldSize = _size; //记录原规模
    ListNodePosi<T> p = first(); ListNodePosi<T> q; //p为各区段起点，q为其后继
    while (trailer != (q = p->succ)) //反复考查紧邻的节点对(p, q)
       if (p->data != q->data) p = q; //若互异，则转向下一区段
       else remove(q); //否则（雷同）直接删除后者，不必如向量那样间接地完成删除
    return oldSize - _size; //列表规模变化量，即被删除元素总数
 }