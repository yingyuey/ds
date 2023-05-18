#pragma once
#include "../vector.xxx/vector.h" //��������
#include "Graph.h" //����ͼADT

template <typename Tv> struct Vertex { //�������Ϊ���������δ�ϸ��װ��
   Tv data; int inDegree, outDegree; VStatus status; //���ݡ����������״̬
   Rank dTime, fTime; //ʱ���ǩ
   Rank parent; int priority; //�ڱ������еĸ��ڵ㡢���ȼ���
   Vertex(Tv const& d = (Tv)0) : //�����¶���
      data(d), inDegree(0), outDegree(0), status(UNDISCOVERED), dTime(-1),
      fTime(-1), parent(-1), priority(INT_MAX) {} //�ݲ�����Ȩ�����
};

template <typename Te> struct Edge { //�߶���Ϊ���������δ�ϸ��װ��
   Te data; int weight;
   EType type; //���ݡ�Ȩ�ء�����
   Edge(Te const& d, int w) : data(d), weight(w), type(UNDETERMINED) {} //����
};

template < typename Tv, typename Te> //�������͡�������
class GraphMatrix : public Graph<Tv, Te> { //�������������ڽӾ�����ʽʵ�ֵ�ͼ
private:
   Vector<Vertex<Tv>> V; //���㼯��������
   Vector<Vector<Edge<Te>*>> E; //�߼����ڽӾ���
public:
   GraphMatrix() { n = e = 0; } //����
   ~GraphMatrix() { //����
      for (Rank v = 0; v < n; v++) //���ж�̬������
         for (Rank u = 0; u < n; u++) //�߼�¼
           delete E[v][u]; //�������
	}
// ����Ļ�����������ѯ��v�����㣨0 <= v < n��
   virtual Tv & vertex(Rank v) { return V[v].data; } //����
   virtual Rank inDegree(Rank v) { return V[v].inDegree; } //���
   virtual Rank outDegree(Rank v) { return V[v].outDegree; } //����
   virtual Rank firstNbr(Rank v) { return nextNbr(v, n); } //�׸��ڽӶ���
   virtual Rank nextNbr(Rank v, Rank u) //����ڶ���u����һ�ڽӶ��㣨�����ڽӱ�Ч�ʸ��ߣ�
      { while ((-1 != --u) && !exists(v, u)); return u; } //����������̽
   virtual VStatus& status(Rank v) { return V[v].status; } //״̬
   virtual Rank& dTime(Rank v) { return V[v].dTime; } //ʱ���ǩdTime
   virtual Rank & fTime(Rank v) { return V[v].fTime; } //ʱ���ǩfTime
   virtual Rank & parent(Rank v) { return V[v].parent; } //�ڱ������еĸ���
   virtual int& priority(Rank v) { return V[v].priority; } //�ڱ������е����ȼ���
// ����Ķ�̬����
   virtual Rank insert(Tv const& vertex) { //���붥�㣬���ر��
      for (Rank u = 0; u < n; u++) E[u].insert(NULL); n++; //������Ԥ��һ��Ǳ�ڵĹ�����
      E.insert(Vector<Edge<Te>*>(n, n, (Edge<Te>*)NULL)); //�����¶����Ӧ�ı�����
      return V.insert(Vertex<Tv>(vertex)); //������������һ������
	}
   virtual Tv remove(Rank v) { //ɾ����v�����㼰������ߣ�0 <= v < n��
      for (Rank u = 0; u < n; u++) //����
         if (exists(v, u)) //����
            { delete E[v][u]; V[u].inDegree--; e--; } //����ɾ��
      E.remove(v); n--; //ɾ����v��
      Tv vBak = vertex(v); V.remove(v); //ɾ������v
      for (Rank u = 0; u < n; u++) //����
        if (Edge<Te>* x = E[u].remove(v)) //���
           { delete x; V[u].outDegree--; e--; } //����ɾ��
      return vBak; //���ر�ɾ���������Ϣ
	}
// �ߵ�ȷ�ϲ���
   virtual bool exists(Rank v, Rank u) //��(v, u)�Ƿ����
     { return (v < n) && (u < n) && (E[v][u] != NULL); }
// �ߵĻ�����������ѯ����v��u֮������ߣ�0 <= v, u < n��exists(v, u)��
   virtual EType & type(Rank v, Rank u) { return E[v][u]->type; } //��(v, u)������
   virtual Te & edge(Rank v, Rank u) { return E[v][u]->data; } //��(v, u)������
   virtual int& weight(Rank v, Rank u) { return E[v][u]->weight; } //��(v, u)��Ȩ��
// �ߵĶ�̬����
   virtual void insert(Te const& edge, int w, Rank v, Rank u) { //����Ȩ��Ϊw�ı�(v, u)
     if (exists(v, u)) return; //ȷ���ñ��в�����
     E[v][u] = new Edge<Te>(edge, w); //�����±�
     e++; V[v].outDegree++; V[u].inDegree++; //���±߼������������Ķ���
	}
   virtual Te remove(Rank v, Rank u) { //ɾ������v��u֮������ߣ�exists(v, u)��
   Te eBak = edge(v, u); delete E[v][u];
   E[v][u] = NULL; //���ݺ�ɾ���߼�¼
   e--; V[v].outDegree--; V[u].inDegree--; //���±߼������������Ķ���
   return eBak; //���ر�ɾ���ߵ���Ϣ
	}
}; //Graph


template < typename Tv, typename Te> //�����������BFS�㷨��ȫͼ��
void Graph<Tv, Te>::bfs(Rank s) { // s < n
   reset(); Rank dClock = 0; //ȫͼ��λ
   for (Rank v = s; v < s + n; v++) //��s��˳�μ�����ж���
      if (UNDISCOVERED == status(v % n)) //һ��������δ������
         BFS(v % n, dClock); //��������������һ��BFS
} //��˿���������ȫͼ�������帴�Ӷ���Ȼ����ΪO(n+e)

template < typename Tv, typename Te> //�����������BFS�㷨��������ͨ��
void Graph<Tv, Te>::BFS(Rank v, Rank & dClock) { // v < n
   Queue<Rank> Q; status(v) = DISCOVERED; Q.enqueue(v); dTime(v) = dClock++; //������
   for (Rank fClock = 0; !Q.empty(); ) { //��Q���֮ǰ��������
     if (dTime(v) < dTime(Q.front())) //dTime�����ӣ���ζ�ſ����µ�һ�������
         dClock++, fClock = 0; //dTime������fTime��λ
      v = Q.dequeue(); //ȡ���׶���v����
      for (Rank u = firstNbr(v); -1 != u; u = nextNbr(v, u)) //����v��ÿһ���ھ�u
         if (UNDISCOVERED == status(u)) { //��u��δ�����֣�����֮
            status(u) = DISCOVERED; Q.enqueue(u); dTime(u) = dClock;
            type(v, u) = TREE; parent(u) = v; //�������ߣ���չBFS��
        }
            else //��u�ѱ����֣����������ѷ�����ϣ���
            type(v, u) = CROSS; //��(v, u)�����ڿ��
      status(v) = VISITED; fTime(v) = fClock++; //���ˣ�v�������
    } //for
} //BFS


template < typename Tv, typename Te> //�����������DFS�㷨��ȫͼ��
void Graph<Tv, Te>::dfs(Rank s) { // s < n
   reset(); Rank clock = 0; //ȫͼ��λ
   for (Rank v = s; v < s + n; v++) //��s��˳�μ�����ж���
      if (UNDISCOVERED == status(v % n)) //һ��������δ������
         DFS(v % n, clock); //��������������һ��DFS
} //��˿���������ȫͼ�������帴�Ӷ���Ȼ����ΪO(n+e)

template < typename Tv, typename Te> //�����������DFS�㷨��������ͨ��
void Graph<Tv, Te>::DFS(Rank v, Rank & clock) { // v < n
   dTime(v) = clock++; status(v) = DISCOVERED; //���ֵ�ǰ����v
   for (Rank u = firstNbr(v); -1 != u; u = nextNbr(v, u)) //����v��ÿһ���ھ�u
      switch (status(u)) { //������״̬�ֱ���
        case UNDISCOVERED: // u��δ���֣���ζ��֧�������ڴ���չ
            type(v, u) = TREE; parent(u) = v; DFS(u, clock); break;
        case DISCOVERED: // u�ѱ����ֵ���δ������ϣ�Ӧ�������ָ�������
            type(v, u) = BACKWARD; break;
        default: // u�ѷ�����ϣ�VISITED������ͼ�������ӳ�Ϯ��ϵ��Ϊǰ��߻���
            type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS; break;
    }
   status(v) = VISITED; fTime(v) = clock++; //���ˣ���ǰ����v����������
}

//��С֧����
template <typename Tv, typename Te> struct PrimPU { //���Prim�㷨�Ķ������ȼ�������
   virtual void operator()(Graph<Tv, Te>*g, Rank v, Rank u) {
      if (UNDISCOVERED == g->status(u)) //����vÿһ��δ�����ֵ��ڽӶ���u
        if (g->priority(u) > g->weight(v, u)) { //��Prim�������ɳ�
            g->priority(u) = g->weight(v, u); //�������ȼ�������
            g->parent(u) = v; //���¸��ڵ�
        }
    }
};

//���·��
template <typename Tv, typename Te> struct DijkPU { //���Dijkstra�㷨�Ķ������ȼ�������
   virtual void operator()(Graph<Tv, Te>*g, Rank v, Rank u) {
      if (UNDISCOVERED == g->status(u)) //����vÿһ��δ�����ֵ��ڽӶ���u����Dijkstra����
         if (g->priority(u) > g->priority(v) + g->weight(v, u)) { //���ɳ�
            g->priority(u) = g->priority(v) + g->weight(v, u); //�������ȼ�������
            g->parent(u) = v; //��ͬʱ���¸��ڵ�
        }
    }
};

//template < typename Tv, typename Te> //����DFS�����������㷨
//Stack<Tv>* Graph<Tv, Te>::tSort(Rank s) { // assert: 0 <= s < n
//   reset(); Rank clock = 0; //ȫͼ��λ
//   Stack<Tv>*S = new Stack<Tv>; //��ջ��¼���򶥵�
//   for (Rank v = s; v < s + n; v++) //��s��˳�μ�����ж���
//      if (UNDISCOVERED == status(v % n)) //һ��������δ������
//         if (!TSort(v, clock, S)) { //��������������һ��TSort
//            while (!S->empty()) //��һ��ͨ���༴��ͼ����DAG
//               S->pop();
//            break; //�򲻱ؼ������㣬��ֱ�ӷ���
//    }
//   return S; //������ΪDAG����S�ڸ������Զ�������򣻷��򣨲������������򣩣�S��
//} //��˿���������ȫͼ�������帴�Ӷ���Ȼ����ΪO(n+e)
//
//template < typename Tv, typename Te> //����DFS�����������㷨�����ˣ�
//bool Graph<Tv, Te>::TSort(Rank v, Rank & clock, Stack<Tv>*S) { // v < n
//   dTime(v) = ++clock; status(v) = DISCOVERED; //���ֶ���v
//   for (Rank u = firstNbr(v); -1 != u; u = nextNbr(v, u)) //ö��v�������ھ�u
//      switch (status(u)) { //����u��״̬�ֱ���
//         case UNDISCOVERED:
//            parent(u) = v; type(v, u) = TREE;
//            if (!TSort(u, clock, S)) //�Ӷ���u��������������
//               return false; //��u��������������������ȫͼ�����ˣ����ʷ��ز�����
//            break;
//         case DISCOVERED:
//            type(v, u) = BACKWARD; //һ�����ֺ���ߣ���DAG������
//            return false; //�������룬�ʷ��ز�����
//         default: // VISITED (digraphs only)
//            type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
//            break;
//    }
//   status(v) = VISITED; S->push(vertex(v)); //���㱻���ΪVISITEDʱ���漴��ջ
//   return true; // v������������������
//}
//
//
//template <typename Tv, typename Te> void Graph<Tv, Te>::bcc(Rank s) { //����DFS��BCC�ֽ��㷨
//   reset(); Rank clock = 0; Rank v = s; Stack<Rank> S; //ջS���Լ�¼�ѷ��ʵĶ���
//   do
//      if (UNDISCOVERED == status(v)) { //һ������δ���ֵĶ��㣨����ͨ������
//         BCC(v, clock, S); //���Ӹö����������һ��BCC
//         S.pop(); //�������غ󣬵���ջ�����һ�����㡪����ǰ��ͨ������
//    }
//   while (s != (v = (++v % n)));
//}
//#define hca(x) (fTime(x)) //���ô˴����õ�fTime[]�䵱hca[]
//template < typename Tv, typename Te> //�������͡�������
//void Graph<Tv, Te>::BCC(Rank v, Rank & clock, Stack<Rank>&S) { // assert: 0 <= v < n
//   hca(v) = dTime(v) = ++clock; status(v) = DISCOVERED; S.push(v); // v�����ֲ���ջ
//   for (int u = firstNbr(v); -1 != u; u = nextNbr(v, u)) //ö��v�������ھ�u
//      switch (status(u)) { //����u��״̬�ֱ���
//         case UNDISCOVERED:
//            parent(u) = v; type(v, u) = TREE; BCC(u, clock, S); //�Ӷ���u������
//            if (hca(u) < dTime(v)) //�������غ�������u��ͨ������ߣ���ָ��v��������
//               hca(v) = min(hca(v), hca(u)); //��v������
//            else //������vΪ�ؽڵ㣨u���¼���һ��BCC�������ж����ʱ��������ջS�Ķ�����
//               while (u != S.pop()); //������ǰBCC�У���v�⣩�����нڵ㣬������Ҫ����һ������
//            break;
//         case DISCOVERED:
//            type(v, u) = BACKWARD; //���(v, u)�������ա�ԽСԽ�ߡ���׼��
//            if (u != parent(v)) hca(v) = min(hca(v), dTime(u)); //����hca[v]
//            break;
//         default: //VISITED (digraphs only)
//            type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
//            break;
//    }
//   status(v) = VISITED; //��v�ķ��ʽ���
//}
//#undef hca