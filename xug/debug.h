/****************************************************************
 debug package, implemented by Lizhuang Zhao, Feburary 7, 2006 
/****************************************************************/

#ifndef DEBUG_MEM_LEAK
#define DEBUG_MEM_LEAK

#include <iostream>
#include <fstream>
#include <assert.h>
#include <set>

using namespace __gnu_cxx;
using namespace std;

#define HASHNS __gnu_cxx
    
#define PR(x)	cout << #x " = " << (x) << endl;
#define PL	cout << "File " << __FILE__ << ", reached line " << __LINE__ << endl;

inline void open_file( ifstream& in, char *filename ){
	in.open( filename );
	if( !in ){
		cout << "Failed to open file: " << filename << endl;
		exit(1);
	}
}

//#define MEM_LEAK_CHECK
/******************************** MEM_LEAK_CHECK: START ********************************/
#ifdef  MEM_LEAK_CHECK

set<void*> regPointers;

void showRegPointers(){
	if( regPointers.empty() ){
		cout << "GREAT: all pointers are released." << endl;
	}
	else{
		cout << "ERROR: some pointers are still not released." << endl;	
		set<void*>::iterator it;
		for(it=regPointers.begin(); it!=regPointers.end(); it++){
			cout << "Pointer: " << *it << endl;
		}
	}
}





template<class T>
class Array{
	T  	*data;
	int	length;
	bool	thruNew;
public:
	class Ptr{ // nested pointer classes
		Array*	ptr;
		int	index;
	public:
		Ptr(): ptr(NULL), index(-1) {}

		Ptr( const Ptr &p ): ptr(p.ptr), index(p.index) {}

		const Ptr& operator=( Array &a ){
			ptr = &a;
			index = 0;
			return *this;					
		}

		const Ptr& operator=( void *p ){
			if( p == NULL ){
				ptr = NULL;
				index = -1;
				return *this;
			}
			cout << "ERROR: assignment with different type in line: " << __LINE__ << endl;
			exit(0);
		}
		// const Ptr& operator=( const Ptr& p ), use shallow copy
				
		const bool operator==( void *p ) const {
			if( ptr == NULL && p == NULL )
				return true;
			return false;
		}

		const bool operator!=( void *p ) const {
			return !( (*this) == p );
		}

		const bool operator==( const Ptr& p ) const {
			if( ptr == NULL && p.ptr == NULL )
				return true;
			if( ptr != NULL && p.ptr != NULL && ptr == p.ptr )
				return true;
			return false;
		}

		const bool operator!=( const Ptr& p ) const {
			return !( (*this) == p );
		}

		const Ptr operator+( int x ) const {
			ExitUnInitPointer(__LINE__);
			ptr->ExitIndexOutOfRange( index, __LINE__);
			ptr->ExitIndexOutOfRange( index+x, __LINE__);
			Ptr  p( *this );
			p.index += x;
			return p;
		}

		const Ptr operator-( int x ) const { 
			ExitUnInitPointer(__LINE__);
			ptr->ExitIndexOutOfRange( index, __LINE__);
			ptr->ExitIndexOutOfRange( index-x, __LINE__);
			Ptr  p( *this );
			p.index -= x;
			return p;
		}

		const Ptr& operator+=( int x ){ 
			ExitUnInitPointer(__LINE__);
			ptr->ExitIndexOutOfRange( index, __LINE__);
			ptr->ExitIndexOutOfRange( index+x, __LINE__);
			index += x;
			return *this;
		}

		const Ptr& operator-=( int x ){ 
			ExitUnInitPointer(__LINE__);
			ptr->ExitIndexOutOfRange( index, __LINE__);
			ptr->ExitIndexOutOfRange( index-x, __LINE__);
			index -= x;
			return *this;
		}

		const Ptr& operator++(){ // prefix
			ExitUnInitPointer(__LINE__);
			ptr->ExitIndexOutOfRange( index, __LINE__);
			ptr->ExitIndexOutOfRange( index+1, __LINE__);
			index++;
			return *this;
		}

		const Ptr operator++(int){ // postfix
			ExitUnInitPointer(__LINE__);
			ptr->ExitIndexOutOfRange( index, __LINE__);
			ptr->ExitIndexOutOfRange( index+1, __LINE__);
			Ptr  p( *this );
			index++;
			return p;
		}

		const Ptr& operator--(){ // prefix
			ExitUnInitPointer(__LINE__);
			ptr->ExitIndexOutOfRange( index, __LINE__);
			ptr->ExitIndexOutOfRange( index-1, __LINE__);
			index--; 
			return *this;
		}
	
		const Ptr operator--(int){ // postfix
			ExitUnInitPointer(__LINE__);
			ptr->ExitIndexOutOfRange( index, __LINE__);
			ptr->ExitIndexOutOfRange( index-1, __LINE__);
			Ptr  p( *this );
			index--; 
			return p;
		}
	
		T& operator *() const {
			ExitUnInitPointer(__LINE__);
			return (*ptr)[index]; 
		}
	
		T* operator->() const {
			ExitUnInitPointer(__LINE__);
			return &(*ptr)[index]; 
		}
	
		T& operator []( int x ){
			ExitUnInitPointer(__LINE__);
			return (*ptr)[ index + x ]; 
		}
		
		void ExitUnInitPointer( int line ) const {
			if( index == -1 ){
				cout << "ERROR: use NULL pointer improperly in line: " << line << endl;
				exit(0);
			}
		}		
	};
	
	Array( int l, bool fNew=false ): length(l), thruNew(fNew) {
		data = new  T[ length ];
		assert( data != NULL ); // should be thru exceptions
		if(thruNew)  regPointers.insert( data );
	}

	T &operator[]( int i ){
		ExitIndexOutOfRange( i, __LINE__ );
		return data[i];
	}
	
	~Array(){
		if(thruNew)  regPointers.erase( data );	
		delete[] data;
	}
	
	void ExitIndexOutOfRange( int index, int line ) const {
		if( index < 0 || index > length-1 ){
			cout << "ERROR: array index (" << index << ") is out of range [0--"<< length-1 << "] in line: " << line << endl;
			exit(0);
		}
	}
	// pointers should be declared before used	
	#define DEF_ARRAY( type, name, size )	Array<type>  name(size);
	#define ARRAY_PTR( type, ptr )		Array<type>::Ptr  ptr;  \
						Array<type>* ptr##_zhao_pointer; 
	#define	NEW_ARRAY( type, ptr,  size )	ptr##_zhao_pointer = new Array<type> (size, true);  \
						assert( ptr##_zhao_pointer != NULL );  \
						ptr = (*ptr##_zhao_pointer);
	#define	DEL_ARRAY( ptr )		delete  ptr##_zhao_pointer;  ptr##_zhao_pointer=NULL;	// prevent being deleted more times.
													// delete a NUll pointer will do nothing.
};

#else
	#define DEF_ARRAY( type, name, size )	type	name[size];
	#define ARRAY_PTR( type, ptr )		type*	ptr;	
	#define	NEW_ARRAY( type, ptr,  size )	ptr = new type[size];
	#define	DEL_ARRAY( ptr )		delete[] ptr;  ptr=NULL;
#endif
/********************************* MEM_LEAK_CHECK: END *********************************/

#endif // DEBUG_MEM_LEAK
