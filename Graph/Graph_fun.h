#pragma once
#include "../vector.xxx/vector.h" //引入向量
#include "Graph.h" //引入图ADT

template <typename Tv> struct Vertex { //顶点对象（为简化起见，并未严格封装）
   Tv data; int inDegree, outDegree; VStatus status; //数据、出入度数、状态
   Rank dTime, fTime; //时间标签
   Rank parent; int priority; //在遍历树中的父节点、优先级数
   Vertex(Tv const& d = (Tv)0) : //构造新顶点
      data(d), inDegree(0), outDegree(0), status(UNDISCOVERED), dTime(-1),
      fTime(-1), parent(-1), priority(INT_MAX) {} //暂不考虑权重溢出
};

template <typename Te> struct Edge { //边对象（为简化起见，并未严格封装）
   Te data; int weight;
   EType type; //数据、权重、类型
   Edge(Te const& d, int w) : data(d), weight(w), type(UNDETERMINED) {} //构造
};

template < typename Tv, typename Te> //顶点类型、边类型
class GraphMatrix : public Graph<Tv, Te> { //基于向量，以邻接矩阵形式实现的图
private:
   Vector<Vertex<Tv>> V; //顶点集（向量）
   Vector<Vector<Edge<Te>*>> E; //边集（邻接矩阵）
public:
   GraphMatrix() { n = e = 0; } //构造
   ~GraphMatrix() { //析构
      for (Rank v = 0; v < n; v++) //所有动态创建的
         for (Rank u = 0; u < n; u++) //边记录
           delete E[v][u]; //逐条清除
	}
// 顶点的基本操作：查询第v个顶点（0 <= v < n）
   virtual Tv & vertex(Rank v) { return V[v].data; } //数据
   virtual Rank inDegree(Rank v) { return V[v].inDegree; } //入度
   virtual Rank outDegree(Rank v) { return V[v].outDegree; } //出度
   virtual Rank firstNbr(Rank v) { return nextNbr(v, n); } //首个邻接顶点
   virtual Rank nextNbr(Rank v, Rank u) //相对于顶点u的下一邻接顶点（改用邻接表效率更高）
      { while ((-1 != --u) && !exists(v, u)); return u; } //逆向线性试探
   virtual VStatus& status(Rank v) { return V[v].status; } //状态
   virtual Rank& dTime(Rank v) { return V[v].dTime; } //时间标签dTime
   virtual Rank & fTime(Rank v) { return V[v].fTime; } //时间标签fTime
   virtual Rank & parent(Rank v) { return V[v].parent; } //在遍历树中的父亲
   virtual int& priority(Rank v) { return V[v].priority; } //在遍历树中的优先级数
// 顶点的动态操作
   virtual Rank insert(Tv const& vertex) { //插入顶点，返回编号
      for (Rank u = 0; u < n; u++) E[u].insert(NULL); n++; //各顶点预留一条潜在的关联边
      E.insert(Vector<Edge<Te>*>(n, n, (Edge<Te>*)NULL)); //创建新顶点对应的边向量
      return V.insert(Vertex<Tv>(vertex)); //顶点向量增加一个顶点
	}
   virtual Tv remove(Rank v) { //删除第v个顶点及其关联边（0 <= v < n）
      for (Rank u = 0; u < n; u++) //所有
         if (exists(v, u)) //出边
            { delete E[v][u]; V[u].inDegree--; e--; } //逐条删除
      E.remove(v); n--; //删除第v行
      Tv vBak = vertex(v); V.remove(v); //删除顶点v
      for (Rank u = 0; u < n; u++) //所有
        if (Edge<Te>* x = E[u].remove(v)) //入边
           { delete x; V[u].outDegree--; e--; } //逐条删除
      return vBak; //返回被删除顶点的信息
	}
// 边的确认操作
   virtual bool exists(Rank v, Rank u) //边(v, u)是否存在
     { return (v < n) && (u < n) && (E[v][u] != NULL); }
// 边的基本操作：查询顶点v与u之间的联边（0 <= v, u < n且exists(v, u)）
   virtual EType & type(Rank v, Rank u) { return E[v][u]->type; } //边(v, u)的类型
   virtual Te & edge(Rank v, Rank u) { return E[v][u]->data; } //边(v, u)的数据
   virtual int& weight(Rank v, Rank u) { return E[v][u]->weight; } //边(v, u)的权重
// 边的动态操作
   virtual void insert(Te const& edge, int w, Rank v, Rank u) { //插入权重为w的边(v, u)
     if (exists(v, u)) return; //确保该边尚不存在
     E[v][u] = new Edge<Te>(edge, w); //创建新边
     e++; V[v].outDegree++; V[u].inDegree++; //更新边计数与关联顶点的度数
	}
   virtual Te remove(Rank v, Rank u) { //删除顶点v和u之间的联边（exists(v, u)）
   Te eBak = edge(v, u); delete E[v][u];
   E[v][u] = NULL; //备份后删除边记录
   e--; V[v].outDegree--; V[u].inDegree--; //更新边计数与关联顶点的度数
   return eBak; //返回被删除边的信息
	}
}; //Graph


template < typename Tv, typename Te> //广度优先搜索BFS算法（全图）
void Graph<Tv, Te>::bfs(Rank s) { // s < n
   reset(); Rank dClock = 0; //全图复位
   for (Rank v = s; v < s + n; v++) //从s起顺次检查所有顶点
      if (UNDISCOVERED == status(v % n)) //一旦遇到尚未发现者
         BFS(v % n, dClock); //即从它出发启动一次BFS
} //如此可完整覆盖全图，且总体复杂度依然保持为O(n+e)

template < typename Tv, typename Te> //广度优先搜索BFS算法（单个连通域）
void Graph<Tv, Te>::BFS(Rank v, Rank & dClock) { // v < n
   Queue<Rank> Q; status(v) = DISCOVERED; Q.enqueue(v); dTime(v) = dClock++; //起点入队
   for (Rank fClock = 0; !Q.empty(); ) { //在Q变空之前，反复地
     if (dTime(v) < dTime(Q.front())) //dTime的增加，意味着开启新的一代，因此
         dClock++, fClock = 0; //dTime递增，fTime复位
      v = Q.dequeue(); //取出首顶点v，并
      for (Rank u = firstNbr(v); -1 != u; u = nextNbr(v, u)) //考查v的每一个邻居u
         if (UNDISCOVERED == status(u)) { //若u尚未被发现，则发现之
            status(u) = DISCOVERED; Q.enqueue(u); dTime(u) = dClock;
            type(v, u) = TREE; parent(u) = v; //引入树边，拓展BFS树
        }
            else //若u已被发现，或者甚至已访问完毕，则
            type(v, u) = CROSS; //将(v, u)归类于跨边
      status(v) = VISITED; fTime(v) = fClock++; //至此，v访问完毕
    } //for
} //BFS


template < typename Tv, typename Te> //深度优先搜索DFS算法（全图）
void Graph<Tv, Te>::dfs(Rank s) { // s < n
   reset(); Rank clock = 0; //全图复位
   for (Rank v = s; v < s + n; v++) //从s起顺次检查所有顶点
      if (UNDISCOVERED == status(v % n)) //一旦遇到尚未发现者
         DFS(v % n, clock); //即从它出发启动一次DFS
} //如此可完整覆盖全图，且总体复杂度依然保持为O(n+e)

template < typename Tv, typename Te> //深度优先搜索DFS算法（单个连通域）
void Graph<Tv, Te>::DFS(Rank v, Rank & clock) { // v < n
   dTime(v) = clock++; status(v) = DISCOVERED; //发现当前顶点v
   for (Rank u = firstNbr(v); -1 != u; u = nextNbr(v, u)) //考查v的每一个邻居u
      switch (status(u)) { //并视其状态分别处理
        case UNDISCOVERED: // u尚未发现，意味着支撑树可在此拓展
            type(v, u) = TREE; parent(u) = v; DFS(u, clock); break;
        case DISCOVERED: // u已被发现但尚未访问完毕，应属被后代指向的祖先
            type(v, u) = BACKWARD; break;
        default: // u已访问完毕（VISITED，有向图），则视承袭关系分为前向边或跨边
            type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS; break;
    }
   status(v) = VISITED; fTime(v) = clock++; //至此，当前顶点v方告访问完毕
}

//最小支撑树
template <typename Tv, typename Te> struct PrimPU { //针对Prim算法的顶点优先级更新器
   virtual void operator()(Graph<Tv, Te>*g, Rank v, Rank u) {
      if (UNDISCOVERED == g->status(u)) //对于v每一尚未被发现的邻接顶点u
        if (g->priority(u) > g->weight(v, u)) { //按Prim策略做松弛
            g->priority(u) = g->weight(v, u); //更新优先级（数）
            g->parent(u) = v; //更新父节点
        }
    }
};

//最短路径
template <typename Tv, typename Te> struct DijkPU { //针对Dijkstra算法的顶点优先级更新器
   virtual void operator()(Graph<Tv, Te>*g, Rank v, Rank u) {
      if (UNDISCOVERED == g->status(u)) //对于v每一尚未被发现的邻接顶点u，按Dijkstra策略
         if (g->priority(u) > g->priority(v) + g->weight(v, u)) { //做松弛
            g->priority(u) = g->priority(v) + g->weight(v, u); //更新优先级（数）
            g->parent(u) = v; //并同时更新父节点
        }
    }
};

//template < typename Tv, typename Te> //基于DFS的拓扑排序算法
//Stack<Tv>* Graph<Tv, Te>::tSort(Rank s) { // assert: 0 <= s < n
//   reset(); Rank clock = 0; //全图复位
//   Stack<Tv>*S = new Stack<Tv>; //用栈记录排序顶点
//   for (Rank v = s; v < s + n; v++) //从s起顺次检查所有顶点
//      if (UNDISCOVERED == status(v % n)) //一旦遇到尚未发现者
//         if (!TSort(v, clock, S)) { //即从它出发启动一次TSort
//            while (!S->empty()) //任一连通域（亦即整图）非DAG
//               S->pop();
//            break; //则不必继续计算，故直接返回
//    }
//   return S; //若输入为DAG，则S内各顶点自顶向底排序；否则（不存在拓扑排序），S空
//} //如此可完整覆盖全图，且总体复杂度依然保持为O(n+e)
//
//template < typename Tv, typename Te> //基于DFS的拓扑排序算法（单趟）
//bool Graph<Tv, Te>::TSort(Rank v, Rank & clock, Stack<Tv>*S) { // v < n
//   dTime(v) = ++clock; status(v) = DISCOVERED; //发现顶点v
//   for (Rank u = firstNbr(v); -1 != u; u = nextNbr(v, u)) //枚举v的所有邻居u
//      switch (status(u)) { //并视u的状态分别处理
//         case UNDISCOVERED:
//            parent(u) = v; type(v, u) = TREE;
//            if (!TSort(u, clock, S)) //从顶点u处出发深入搜索
//               return false; //若u及其后代不能拓扑排序（则全图亦必如此），故返回并报告
//            break;
//         case DISCOVERED:
//            type(v, u) = BACKWARD; //一旦发现后向边（非DAG），则
//            return false; //不必深入，故返回并报告
//         default: // VISITED (digraphs only)
//            type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
//            break;
//    }
//   status(v) = VISITED; S->push(vertex(v)); //顶点被标记为VISITED时，随即入栈
//   return true; // v及其后代可以拓扑排序
//}
//
//
//template <typename Tv, typename Te> void Graph<Tv, Te>::bcc(Rank s) { //基于DFS的BCC分解算法
//   reset(); Rank clock = 0; Rank v = s; Stack<Rank> S; //栈S用以记录已访问的顶点
//   do
//      if (UNDISCOVERED == status(v)) { //一旦发现未发现的顶点（新连通分量）
//         BCC(v, clock, S); //即从该顶点出发启动一次BCC
//         S.pop(); //遍历返回后，弹出栈中最后一个顶点——当前连通域的起点
//    }
//   while (s != (v = (++v % n)));
//}
//#define hca(x) (fTime(x)) //利用此处闲置的fTime[]充当hca[]
//template < typename Tv, typename Te> //顶点类型、边类型
//void Graph<Tv, Te>::BCC(Rank v, Rank & clock, Stack<Rank>&S) { // assert: 0 <= v < n
//   hca(v) = dTime(v) = ++clock; status(v) = DISCOVERED; S.push(v); // v被发现并入栈
//   for (int u = firstNbr(v); -1 != u; u = nextNbr(v, u)) //枚举v的所有邻居u
//      switch (status(u)) { //并视u的状态分别处理
//         case UNDISCOVERED:
//            parent(u) = v; type(v, u) = TREE; BCC(u, clock, S); //从顶点u处深入
//            if (hca(u) < dTime(v)) //遍历返回后，若发现u（通过后向边）可指向v的真祖先
//               hca(v) = min(hca(v), hca(u)); //则v亦必如此
//            else //否则，以v为关节点（u以下即是一个BCC，且其中顶点此时正集中于栈S的顶部）
//               while (u != S.pop()); //弹出当前BCC中（除v外）的所有节点，可视需要做进一步处理
//            break;
//         case DISCOVERED:
//            type(v, u) = BACKWARD; //标记(v, u)，并按照“越小越高”的准则
//            if (u != parent(v)) hca(v) = min(hca(v), dTime(u)); //更新hca[v]
//            break;
//         default: //VISITED (digraphs only)
//            type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
//            break;
//    }
//   status(v) = VISITED; //对v的访问结束
//}
//#undef hca