#include "Vector.h"

template <typename T>
void Vector<T>::copyFrom ( T const* A, Rank lo, Rank hi ) {
	_elem = new T[_capacity = 2 * ( hi - lo ) ];_size = 0;
	while ( lo < hi )
	_elem [_size++] = A[lo++];
}

template <typename T> void Vector<T>::expand() {
	if ( _size < _capacity ) return;
	if ( _capacity < DEFAULT_CAPACITY ) _capacity = DEFAULT_CAPACITY;
	T* oldElem = _elem;
	_elem = new T[_capacity <<= 1];
	for( int i = 0; i < _size; i++)
	_elem[i] = oldElem[i];
	delete [] oldElem;
}

template <typename T> void Vector<T>::shrink() {
	if ( _capacity < DEFAULT_CAPACITY << 1 ) return;
	if ( _size << 2 >_capacity ) return;
	T* oldElem = _elem;
	_elem = new T[_capacity >>= 1];
	for ( int i = 0; i < _size; i++)
	_elem[i] = oldElem[i];
	delete [] oldElem; 
}

template <typename T> bool Vector<T>::bubble ( Rank lo, Rank hi ) {
	bool sorted = true;
	while ( ++lo < hi )
	if ( _elem[lo - 1] > _elem[lo] ) {
		sorted = false;
		swap ( _elem[lo - 1],_elem[lo] );
	}
	return sorted;
}

template <typename T> void Vector<T>::bubbleSort ( Rank lo, Rank hi ) {
	while ( !bubble ( lo ,hi-- ) );
}

template <typename T> void Vector<T>::merge ( Rank lo, Rank mi, Rank hi ) {
	T* A = _elem + lo;
	int lb = mi - lo;
	T* B = new T[lb];
	for ( Rank i = 0; i < lb; B[i] = A[i++] );
	int lc = hi - mi; T*C = _elem + mi;
	for ( Rank i = 0, j = 0, k = 0;( j < lb ) || ( k < lc ); ) {
		if ( ( j < lb ) && ( ! ( k < lc ) || ( B[j] <= C[k] ) ) ) 
		A[i++] = B[j++];
		if ( ( k < lc ) && ( ! ( j< lb ) || ( C[k] < B[j] ) ) )
		A[i++] = C[k++];
	}
	delete [] B;
}

template <typename T> void Vector<T>::mergeSort ( Rank lo, Rank hi ) {
	if ( hi - lo < 2 ) 
	return;
	int mi = ( lo + hi ) / 2;
	mergeSort ( lo, hi );
	mergeSort ( mi, hi );
	merge ( lo, mi, hi );
}

template <typename T> Rank Vector<T>::partition ( Rank lo, Rank hi ) {
	swap ( _elem[lo], _elem[lo + rand() % ( hi - lo + 1 ) ] );
	T pivot = _elem[lo];
	while ( lo < hi ) {
		while ( ( lo < hi ) && ( pivot <= _elem[hi] ) )
		hi--;
		_elem[lo] = _elem[hi];
		while ( ( lo < hi ) && ( _elem[lo] <= pivot ) )
		lo++;
		_elem[hi] = _elem[lo];
	}
	_elem[lo] = pivot;
	return lo; 
}

template <typename T> void Vector<T>::quickSort ( Rank lo, Rank hi ) {
	if ( hi - lo < 2 )
	return;
	Rank mi = partition ( lo, hi - 1 );
	quickSort ( lo, mi );
	quickSort ( mi + 1, hi );
} 

template <typename T> void Vector<T>::heapSort ( Rank lo, Rank hi ) {
	     PQ_ComplHeap<T> H ( _elem + lo, hi - lo );
	while ( !H.empty() )
	_elem[--hi] = H.delMax();
}


template <typename T> T& Vector<T>::operator[] ( Rank r ) const
{
	return _elem[r];
 } 
template <typename T> Vector<T> & Vector<T> ::operator = ( Vector<T> const& ) {
	if ( _elem )
	delete []_elem;
	copyFrom ( _elem, 0, size() );
	return *this;
}

template <typename T> void Vector<T>::unsort ( Rank lo, Rank hi ) {
	T* V = _elem + lo;
	for ( Rank i = hi - lo; i > 0; i-- ) 
	swap ( V[i - 1], V[rand() % i] );
}

template <typename T> Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
	while ( ( lo < hi-- ) && ( e != _elem[hi] ) );
	return hi;
}

template <typename T> Rank Vector<T>::insert(Rank r, T const& e) {
	expand();
	for ( int i = _size; i > r; i--) _elem[i] = _elem[i-1];
	_elem[r] = e;
	_size++;
	return r; 
}

template <typename T> Rank Vector<T>::remove(Rank lo, Rank hi) {
	if ( lo == hi ) 
	return 0;
	while ( hi < _size ) _elem[lo++] = _elem[hi++]; 
	_size = lo;
	shrink();
	return hi - lo; 
}

template <typename T> T Vector<T>::remove(Rank r) {
	T e = _elem[r];
	remove ( r, r+1 );
	return e;
}

template <typename T> int Vector<T>::deduplicate() {
	int oldSize = _size;
	Rank i = 1;
	while ( i < size )
	( find ( _elem[i], 0, 1 ) < 0 ) ?
	i++ : remove ( i );
	return oldSize - _size;
}

template <typename T> void Vector<T>::traverse ( void ( *visit ) ( T& ) )
{
	for ( int i = 0; i < _size; i++ )
	visit ( _elem[i] );
}

template <typename T> template <typename VST>
void Vector<T>::traverse ( VST& visit )
{
	for ( int i = 0; i < _size; i++)
	visit ( _elem[i] );
}

template <typename T> int Vector<T>::disordered() const
{
	int n = 0;
	for ( int i = 1; i < _size; i++)
	if ( _elem[i - 1] > _elem[i] )
	n++;
	return n;
}

template <typename T> Rank Vector<T>::search ( T const& e, Rank lo, Rank hi ) const
{
	return ( rand() % 2 ) ? binSearch ( _elem, e, lo, hi ) : fibSearch ( _elem, e, lo, hi );
}

template <typename T> int Vector<T>::uniquify()
{
	Rank i = 0, j = 0;
	while ( ++j < _size )
	if ( _elem[i] != _elem[j] )
	_elem[++i] = _elem[j];
	_size = ++i;
	shrink;
	return j - i;
}


 


