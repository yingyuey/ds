 #include "ListNode.h" //�����б�ڵ���

template <typename T> class List 
{ //�б�ģ����
private:
    Rank _size; ListNodePosi<T> header, trailer; //��ģ��ͷ�ڱ���β�ڱ�
protected:
    void init(); //�б���ʱ�ĳ�ʼ��
    Rank clear(); //������нڵ�
    void copyNodes(ListNodePosi<T>, Rank); //�����б�����λ��p���n��
    ListNodePosi<T> merge(ListNodePosi<T>, Rank, List<T>&, ListNodePosi<T>, Rank); //�鲢
    void mergeSort(ListNodePosi<T>&, Rank); //�Դ�p��ʼ������n���ڵ�鲢����
    void selectionSort(ListNodePosi<T>, Rank); //�Դ�p��ʼ������n���ڵ�ѡ������
	void insertionSort(ListNodePosi<T>, Rank); //�Դ�p��ʼ������n���ڵ��������
    void radixSort(ListNodePosi<T>, Rank); //�Դ�p��ʼ������n���ڵ��������

 public:
// ���캯��
    List() { init(); } //Ĭ��
    List(List<T> const& L); //���帴���б�L
    List(List<T> const& L, Rank r, Rank n); //�����б�L���Ե�r�����n��
	List(ListNodePosi<T> p, Rank n); //�����б�����λ��p���n��
    // ��������
    ~List(); //�ͷţ�����ͷ��β�ڱ����ڵģ����нڵ�
 // ֻ�����ʽӿ�
    Rank size() const { return _size; } //��ģ
    bool empty() const { return _size <= 0; } //�п�
    ListNodePosi<T> operator[](Rank r) const; //���أ�֧��ѭ�ȷ��ʣ�Ч�ʵͣ�
    ListNodePosi<T> first() const { return header->succ; } //�׽ڵ�λ��
    ListNodePosi<T> last() const { return trailer->pred; } //ĩ�ڵ�λ��
    bool valid(ListNodePosi<T> p) //�ж�λ��p�Ƿ����Ϸ�
    { return p && (trailer != p) && (header != p); } //��ͷ��β�ڵ��ͬ��NULL
    ListNodePosi<T> find(T const& e) const //�����б����
    { return find(e, _size, trailer); }
    ListNodePosi<T> find(T const& e, Rank n, ListNodePosi<T> p) const; //�����������
    ListNodePosi<T> search(T const& e) const //�����б����
	{ return search(e, _size, trailer); }
    ListNodePosi<T> search(T const& e, Rank n, ListNodePosi<T> p) const; //�����������
    ListNodePosi<T> selectMax(ListNodePosi<T> p, Rank n); //��p����n-1�������ѡ�������
    ListNodePosi<T> selectMax() { return selectMax(header->succ, _size); } //���������
 // ��д���ʽӿ�
    ListNodePosi<T> insertAsFirst(T const& e); //��e�����׽ڵ����
    ListNodePosi<T> insertAsLast(T const& e); //��e����ĩ�ڵ����
    ListNodePosi<T> insert(ListNodePosi<T> p, T const& e); //��e����p�ĺ�̲���
    ListNodePosi<T> insert(T const& e, ListNodePosi<T> p); //��e����p��ǰ������
    T remove(ListNodePosi<T> p); //ɾ���Ϸ�λ��p���Ľڵ�,���ر�ɾ���ڵ�
    void merge(List<T>& L) { merge(header->succ, _size, L, L.header->succ, L._size); } //ȫ�б�鲢
    void sort(ListNodePosi<T>, Rank); //�б���������
    void sort() { sort(first(), _size); } //�б���������
    Rank deduplicate(); //����ȥ��
    Rank uniquify(); //����ȥ��
    void reverse(); //ǰ���ã�ϰ�⣩
 // ����
    void traverse(void (*)(T&)); //����ʵʩvisit����������ָ�룩
    template <typename VST> void traverse(VST&); //����ʵʩvisit��������������
}; //List

 template <typename T> void List<T>::init() { //�б��ʼ�����ڴ����б����ʱͳһ����
     header = new ListNode<T>; trailer = new ListNode<T>; //����ͷ��β�ڱ��ڵ�
     header->succ = trailer; header->pred = NULL; //��ǰ����
     trailer->pred = header; trailer->succ = NULL; //�������
     _size = 0; //��¼��ģ
}
 template < typename T> //�����±����������ͨ����ֱ�ӷ����б�ڵ㣨O(r)Ч�ʣ��䷽�㣬����ã�
  ListNodePosi<T> List<T>::operator[](Rank r) const { // 0 <= r < size
  ListNodePosi<T> p = first(); //���׽ڵ����
  while (0 < r--) p = p->succ; //˳����r���ڵ㼴��
  return p; //Ŀ��ڵ�
 }

 template < typename T> //�������б��ڽڵ�p��������trailer����n�����棩ǰ���У��ҵ�����e�������
 ListNodePosi<T> List<T>::find(T const& e, Rank n, ListNodePosi<T> p) const {
     while (0 < n--) //��0 <= n <= Rank(p) < _size������p�������n��ǰ������������
       if (e == (p = p->pred)->data) return p; //����ȶԣ�ֱ�����л�ΧԽ��
     return NULL; //pԽ����߽���ζ�������ڲ���e������ʧ��
 } //ʧ��ʱ������NULL

 template <typename T> ListNodePosi<T> List<T>::insertAsFirst(T const& e)
    { _size++; return header->insertAsSucc(e); } // e�����׽ڵ����
 template <typename T> ListNodePosi<T> List<T>::insertAsLast(T const& e)
    { _size++; return trailer->insertAsPred(e); } // e����ĩ�ڵ����
 template <typename T> ListNodePosi<T> List<T>::insert(ListNodePosi<T> p, T const& e)
    { _size++; return p->insertAsSucc(e); } // e����p�ĺ�̲���
 template <typename T> ListNodePosi<T> List<T>::insert(T const& e, ListNodePosi<T> p)
    { _size++; return p->insertAsPred(e); } // e����p��ǰ������

 template < typename T> //��e������ǰ�ڵ�֮ǰ�����ڵ�ǰ�ڵ������б������ڱ�ͷ�ڵ�header��
 ListNodePosi<T> ListNode<T>::insertAsPred(T const& e) {
    ListNodePosi<T> x = new ListNode(e, pred, this); //�����½ڵ�
    pred->succ = x; pred = x; //������������
    return x; //�����½ڵ��λ��
 }

 template < typename T> //��e���浱ǰ�ڵ�֮������ڵ�ǰ�ڵ������б������ڱ�β�ڵ�trailer��
 ListNodePosi<T> ListNode<T>::insertAsSucc(T const& e) {
    ListNodePosi<T> x = new ListNode(e, this, succ); //�����½ڵ�
    succ->pred = x; succ = x; //������������
    return x; //�����½ڵ��λ��
 }

 template < typename T> //�б��ڲ������������б�����λ��p���n��
 void List<T>::copyNodes(ListNodePosi<T> p, Rank n) { // p�Ϸ�����������n-1������
    init(); //����ͷ��β�ڱ��ڵ㲢����ʼ��
    while (n--) { insertAsLast(p->data); p = p->succ; } //������p��n��������Ϊĩ�ڵ����
 }

 template < typename T> //�����б�����λ��p���n�assert: pΪ�Ϸ�λ�ã���������n-1����̽ڵ㣩
 List<T>::List(ListNodePosi<T> p, Rank n) { copyNodes(p, n); }
 template < typename T> //���帴���б�L
 List<T>::List(List<T> const& L) { copyNodes(L.first(), L._size); }
 template <typename T> //����L���Ե�r�����n�assert: r+n <= L._size��
 List<T>::List(List<T> const& L, Rank r, Rank n) {
    ListNodePosi<T> p = L.first();
    while (0 < r--) p = p->succ;
    copyNodes(p, n);
 }

 template <typename T> T List<T>::remove(ListNodePosi<T> p) { //ɾ���Ϸ��ڵ�p����������ֵ
    T e = p->data; //���ݴ�ɾ���ڵ����ֵ���ٶ�T���Ϳ�ֱ�Ӹ�ֵ��
    p->pred->succ = p->succ; p->succ->pred = p->pred; //��̡�ǰ��
    delete p; _size--; //�ͷŽڵ㣬���¹�ģ
    return e; //���ر��ݵ���ֵ
 }

 template <typename T> List<T>::~List() //�б�������
 { clear(); delete header; delete trailer; } //����б��ͷ�ͷ��β�ڱ��ڵ�

 template <typename T> Rank List<T>::clear() { //����б�
    Rank oldSize = _size;
    while (0 < _size) remove(header->succ); //����ɾ���׽ڵ㣬ֱ���б���
    return oldSize;
 }

 template <typename T> Rank List<T>::deduplicate() {
    Rank oldSize = _size; ListNodePosi<T> p = first();
    for (Rank r = 0; p != trailer; p = p->succ) //O(n)
      if (ListNodePosi<T> q = find(p->data, r, p))
          remove(q); //��ʱq��p��ͬ����ɾ��ǰ�߸�Ϊ����
       else r++; //rΪ����ǰ׺�ĳ���
    return oldSize - _size; //ɾ��Ԫ������
 }

 template <typename T> void List<T>::traverse(void (*visit)(T&)) //��������ָ����Ʊ���
 {  for (ListNodePosi<T> p = header->succ; p != trailer; p = p->succ) visit(p->data);  }
 template <typename T> template < typename VST> //Ԫ�����͡�������
 void List<T>::traverse(VST & visit) //��������������Ʊ���
 {  for (ListNodePosi<T> p = header->succ; p != trailer; p = p->succ) visit(p->data);  }

 template <typename T> Rank List<T>::uniquify() { //�����޳��ظ�Ԫ�أ�Ч�ʸ���
    if (_size < 2) return 0; //ƽ���б���Ȼ���ظ�
    Rank oldSize = _size; //��¼ԭ��ģ
    ListNodePosi<T> p = first(); ListNodePosi<T> q; //pΪ��������㣬qΪ����
    while (trailer != (q = p->succ)) //����������ڵĽڵ��(p, q)
       if (p->data != q->data) p = q; //�����죬��ת����һ����
       else remove(q); //������ͬ��ֱ��ɾ�����ߣ�����������������ӵ����ɾ��
    return oldSize - _size; //�б��ģ�仯��������ɾ��Ԫ������
 }