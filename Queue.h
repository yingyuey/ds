#include"../../List/List.h"
 template <typename T> class Queue : public List<T> { //����ģ����
 public: //ԭ�нӿ�һ������
    void enqueue(T const& e) { insertAsLast(e); } //��ӣ�β������
    T dequeue() { return remove(first()); } //���ӣ��ײ�ɾ��
    T & front() { return first()->data; } //����
};