//#define MEM_LEAK_CHECK
#include "debug.h"
#include <vector>
#include <cstring>
#include <ext/hash_map>

#define HASHNS __gnu_cxx

template <class T>
inline void CopyEnd(const T& s1, T& s2){
        copy( s1.begin(), s1.end(), insert_iterator<T>(s2, s2.end()));
}

template <class T>
inline void Copy(const T& s1, T& s2){
        s2.clear();
        copy( s1.begin(), s1.end(), insert_iterator<T>(s2, s2.begin()));
}

template <class T>
inline void Intersect(const T& s1, const T &s2, T& r){
	r.clear();
        set_intersection( s1.begin(), s1.end(), s2.begin(), s2.end(), insert_iterator<T> (r, r.begin()));
}

template <class T>
inline void Union(const T& s1, const T &s2, T& r){
        r.clear();
        set_union( s1.begin(), s1.end(), s2.begin(), s2.end(), insert_iterator<T> (r, r.begin()));
}

double frandom( double value ){
        return (double)rand()/RAND_MAX * value;
}

int irandom( int value ){
        return rand() % value;
}



#define Iset	Set
#define Tset	Set
typedef vector<int>	SET;
typedef SET::iterator	SIt;
class Sets;

class Set{
public:
	SET	S;
	
	void insert( int x ){
		S.push_back(x);
		//sort( S.begin(), S.end() ); // need to change
	}

	void sub( Set &s, Set &r ){
		SIt it1, it2;

		r.S.clear();
		it1=S.begin();
		it2=s.S.begin(); 

		while(it1!=S.end() && it2!=s.S.end()){
			if( *it1 == *it2 ){
				it1++;
				it2++;
			}
			else if( *it1 < *it2 ){
				r.insert( *it1 );
				it1++;
			}
			else	it2++;	// *it2 < *it1 
		}
		for( ; it1!=S.end(); it1++ )
			r.insert( *it1 );
	}

	void getTset( Tset* data, Tset* complement, Tset &T ){
		Tset	TS[2];
		int	idx=0;
		int	n;

		for(SIt it=S.begin(); it!=S.end(); it++){
			if( *it > 0 ){
				Union( data[*it].S, TS[idx].S, TS[1-idx].S );
				idx = 1-idx;
			}
			else{
				n = -(*it);
				Union( complement[n].S, TS[idx].S, TS[1-idx].S );
				idx = 1-idx;
			}
		}
		T = TS[idx];
	}

	bool operator==( Set &s ){ 
		SIt it1, it2;
		if(S.size() != s.S.size())
			return false;
		for(it1=S.begin(), it2=s.S.begin(); it1!=S.end(); it1++, it2++)
			if(*it1 != *it2)  return false;
		return true;
	}
	
	bool operator!=( Set &s ){ 
		SIt it1, it2;
		if(S.size() != s.S.size())
			return true;
		for(it1=S.begin(), it2=s.S.begin(); it1!=S.end(); it1++, it2++)
			if(*it1 != *it2)  return true;
		return false;
	}
	 
	void sort_del_dup(){
		int	x;
		SIt	it;

		sort(S.begin(), S.end());
		it = S.begin();
		x = *it;
		it++;
		while( it != S.end() ){
			while( it!=S.end() && *it == x )
				it = S.erase(it);	
			if( it!=S.end() ){
				x = *it;
				it++;
			}
		}
	}

	void operator+=( Set &s ){
       		CopyEnd( s.S, S );
		this->sort_del_dup();
	}
 	
	bool operator< ( Set &s ){ 
		SIt it1, it2;

		if(S.size() < s.S.size())	return true;
		else if(S.size() > s.S.size())	return false;
		else{ 
			for(it1=S.begin(), it2=s.S.begin(); it1!=S.end(); it1++, it2++)
				if( *it1 < *it2 )	return true;	
				else if( *it1 > *it2 )	return false;
			return false; //==
		}
	}

	bool contain( Set &s ){
		SIt it1, it2;

		if( S.size() < s.S.size() )
			return false;
		it1=S.begin();
		it2=s.S.begin();
		while( it2 != s.S.end() ){
			while( *it1<*it2 && it1!=S.end() )
				it1++;
			if( it1==S.end() || *it1 != *it2 )
				return false;
			it2++;
		}
		return true;
	}

	int size(){ return S.size(); }

	void show( char *seperation, bool letter ){
		SIt it;
		cout << "{";
		for(it=S.begin(); it!=S.end(); it++){
			if(it != S.begin()) 
				cout << seperation;
			if( letter )
				cout << char(*it+'A');
			else
				cout << *it;
		}
		cout << "}";
	}
};



typedef vector<Set>		SETS;
typedef vector<Set>::iterator	SSIt;

class Sets{
public:
	SETS	SS;

	int size(){
		return SS.size();
	}

	/*void operator=( Sets &ss ){
		Copy( ss.SS, SS );
	}*/

	void insert( Set &s ){
		SS.push_back( s );
	}
	
	bool add( Set &s ){
		SSIt	it;
		int	ssz;

		ssz = s.S.size();

		it=SS.begin();
		while(it != SS.end()){
			if( it->S.size() == ssz ){	// no subsumption
				it++;
				continue;
			}
			if( s.contain(*it) )		// subsumed by s
				it = SS.erase(it);
			else if( it->contain(s) )	// subsumed by *it
				return false;
			else	it++;
		}
		SS.push_back(s);
		return true;
	}

	void show( char *sepOut, char *sepIn, bool fLetters ){
		SSIt it;
		cout << "{";
		for(it=SS.begin(); it!=SS.end(); it++){
			if(it != SS.begin()) 
				cout << sepOut;
			it->show( sepIn, fLetters );	// show letters
		}
		cout << "}";
	}
};



int hashVal( Set &tset, int base ){ 
	SIt it;
	int ret = base;
	for(it=tset.S.begin(); it!=tset.S.end(); it++)
		ret = (ret + *it) % 32768; // need to consider
	return ret;
}	



class Class{
public:
	int	tsup;
	Set	tset;
	Set	iset;
		
	void show( bool only_items, Set &WS, bool fLetters, char *sep ){
		if( !only_items ){
			//Set T;
			//WS.sub( tset, T );
			//T.show( ",", false ); // do not show letters	
			tset.show( ",", false );
			cout << ":\t";
		}
		iset.show( "|", fLetters );
	}
};



typedef HASHNS::hash_multimap<int, Class, HASHNS::hash<int>, equal_to<int> >	myHashTable;
typedef pair<myHashTable::iterator, myHashTable::iterator>	myHashFind;
typedef myHashTable::iterator					myHashIt;
typedef myHashTable::value_type myHTPair;

#define HASH_SIZE	1000
class Classes{
public:
	myHashTable	HT;
        Set             whole;
	Tset		*pData;
	Tset		*pComplement;
	bool		fFormTsets;

	Classes(int sz=HASH_SIZE): HT(sz){ fFormTsets = false; }

	void formTsets(){
		if( fFormTsets )
			return;

		cout << endl << "Please Wait forming Tsets ..." << endl << endl;
		int num = 1;

		for(myHashIt hit=HT.begin(); hit!=HT.end(); hit++, num++){
			if( num%1000 == 0 )
				cout << "num: " << num << endl;
			hit->second.iset.getTset( pData, pComplement, hit->second.tset );
		}
		fFormTsets = true;
	}

	void show( bool only_items, bool fLetters, bool fOutput ){
		myHashIt  it;

		if( !only_items && fOutput )
			formTsets();

		cout << endl << endl << "XUG Classes:" << endl << endl;

		if( fOutput ){
			for(it=HT.begin(); it!=HT.end(); it++){
				if(it != HT.begin())
					cout << "\n";
				it->second.show( only_items, whole, fLetters, "|" );
			}
			cout << endl << endl;
		}
		cout << "#Tset: " << HT.size()  << endl;
	}

	bool add( int tsup, Set &I, int hashval ){ 
   		myHashIt   	hi;
		myHashFind 	p;
   		int 		h, ires;
		Class		c;
		int		Tsize, Isize;

		Tsize = tsup;
		Isize = I.size();

   		h = HT.hash_funct()( hashval ); // need to change
		p = HT.equal_range( h );
                                         
		if( p.first==p.second ) 
			goto ADD;

   		/*for(hi=p.first; hi!=p.second; hi++){
			if( hi->second.tset != c.tset )
				continue;

			if( I.contain(hi->second.iset) ){
				hi->second.iset = I;
				return true;
			}
			else	return false;
   		}*/
   		for(hi=p.first; hi!=p.second; hi++){
			//if( hi->second.tset.size() != Tsize )
			if( hi->second.tsup != Tsize )
				continue;

			if( I.contain(hi->second.iset) ){
				hi->second.iset = I;
				return true;
			}
			if( hi->second.iset.contain(I) )
				return false;
   		}
		////////////
		ADD:; 
		c.iset = I;
		c.tsup = Tsize;
   		HT.insert( myHTPair(h,c) );
		return true;
	}
};



class Input
{
        #define MAX_LINE        100000
        #define MAX_WORD        1000
        char      buf[MAX_LINE], sub[MAX_WORD];
        int       len, k;
        ifstream  infile;
                                                                                                                                                              
public:
        void open(char *file){
                infile.open(file);
        }
                                                                                                                                                              
        void close(){
                infile.close();
        }
        
	bool getLine(){
		if(infile.eof())
			return false;
                infile.getline(buf, MAX_LINE, '\n');
                len = strlen( buf );
                assert(len <= MAX_LINE);
                k=0;
		return true;
        }
                                                                                                                                                              
        char* getWord( char sep ){ // '\t'
                int p=0;
               
		while( buf[k]==sep && k<len )
			k++;
		if( buf[k]=='\n' || buf[k]==0xd || k>=len )
			return NULL;

                do{
                        sub[p++] = buf[k++];
                }while( buf[k]!=sep && buf[k]!=0xd && k<len ); //oxd for ^M
                k++;
                sub[p] = '\0';
                assert( p <= MAX_WORD );
                return (char *)sub;
        }
};



class DB
{
public:
	Tset	*data;
	Tset	*complement;
	Set	whole;
	int	itemNum, tranNum;
	bool	fConverted;
	char	convertedName[50];

	Set &operator[]( int i ){
		return data[i];
	}
	
	DB(){
		itemNum = 0;
		tranNum = 0;
		data = NULL;
		complement = NULL;
		fConverted = false;
	}

	~DB(){
		if( data != NULL ){ delete[] data; data = NULL; }
		if( complement != NULL ){ delete[] complement; complement = NULL; }
	}

        void synInit( int iNum, int tNum, float density ){
		Tset	ws;
		SIt	it;
		int i, j, length;

		itemNum = iNum;
		tranNum = tNum;
                data = new Tset[itemNum+1];
		assert( data != NULL );
                complement = new Tset[itemNum+1];
		assert( complement != NULL );

		for(i=0; i<tranNum; i++)
			ws.insert(i+1);
	
                for(i=1; i<=itemNum; i++){
			length = int( irandom(tranNum)*2*density ); 
			if( length == 0 )		length = 1;
			else if(length > tranNum)	length = tranNum;

			data[i] = ws;
			it = data[i].S.begin();
			while( data[i].S.size() > length ){
				if( frandom(1.0) > density )
					it = data[i].S.erase(it);
				else	it++;
				if( it == data[i].S.end() )
					it = data[i].S.begin();
			}
		}
		getWholeSet_syn();

		for(i=1; i<=itemNum; i++)
			whole.sub( data[i], complement[i] );
        }

	bool converted( char *filename ){
		FILE	*fp;
		int 	*Trans;
		int	len, i, j;

		len = strlen( filename );
		assert( len < 50 );
		strcpy( convertedName, filename );
		for(i=0; i<len; i++)
			if( convertedName[i] == '.' )  break;
		strcpy( convertedName+i, ".mung" );

		fp = fopen( convertedName, "rb" );
		if( fp == NULL ){
			fConverted = false;
			return false;
		}
		fConverted = true;

		fread( &tranNum, 1, sizeof(int), fp );
		Trans = new int[tranNum];
		assert( Trans != NULL );

		fread( Trans, tranNum, sizeof(int), fp );
		for(i=0; i<tranNum; i++)
			whole.S.push_back( Trans[i] );

		fread( &itemNum, 1, sizeof(int), fp );
                data = new Tset[itemNum+1];
		assert( data != NULL );
                complement = new Tset[itemNum+1];
		assert( complement != NULL );

		cout << "itemNum: " << itemNum << endl;
		cout << "tranNum: " << tranNum << endl;

		for(j=1; j<=itemNum; j++){
			fread( &len, 1, sizeof(int), fp );
			fread( Trans, len, sizeof(int), fp );
			for(i=0; i<len; i++)
				data[j].S.push_back( Trans[i] );
			whole.sub( data[j], complement[j] );
		}
		delete[] Trans;
		fclose( fp );
		return true;
	}

	void saveConvertedFile(){
		FILE	*fp;
		int 	*Trans;
		int	len, i, j;
		SIt	it;

		fp = fopen( convertedName, "wb" );
		assert( fp != NULL );

		Trans = new int[tranNum];
		assert( Trans != NULL );

		fwrite( &tranNum, 1, sizeof(int), fp );
		for(it=whole.S.begin(), i=0; it!=whole.S.end(); it++, i++)
			Trans[i] = *it;
		fwrite( Trans, tranNum, sizeof(int), fp );

		fwrite( &itemNum, 1, sizeof(int), fp );
		for(j=1; j<=itemNum; j++){
			len = data[j].size();
			fwrite( &len, 1, sizeof(int), fp );
			for(it=data[j].S.begin(), i=0; it!=data[j].S.end(); it++, i++)
				Trans[i] = *it;
			fwrite( Trans, len, sizeof(int), fp );
		}
		delete[] Trans;
		fclose( fp );
		return;
	}

	void readsize( char *filename ){
		Input	in;
		char*	p;
		int	x, t;
		FILE	*fp;

		if( converted(filename) )
			return;	

		fp = fopen( filename, "rb" );
		if( fp == NULL ){
			cout << "Failed to open file: " << filename << endl << endl;
			exit(0);
		}
		else	fclose(fp);
		// get item number
		itemNum = 0;
		tranNum = 0;
		in.open( filename );
		while( in.getLine() ){
			p = in.getWord(' '); // transaction #
			if( p == NULL )
				break;
			p = in.getWord(' '); // transaction #
			p = in.getWord(' '); // length
			p = in.getWord(' '); 
			while( p != NULL ){
				x = atoi(p);
				if( x > itemNum )
					itemNum = x;
				p = in.getWord(' ');
			}
			tranNum++;
		}
		in.close();
		//itemNum++; // include 0
		cout << "itemNum: " << itemNum << endl;
		cout << "tranNum: " << tranNum << endl;
	}

	void readfile( char *filename ){
		Input	in;
		char*	p;
		int	x, t;

		if( fConverted ){
			cout << "binary vertical file: " << convertedName << " exists, done with reading." << endl << endl;
			return;
		}

                data = new Tset[itemNum+1];
		assert( data != NULL );
                complement = new Tset[itemNum+1];
		assert( complement != NULL );

		in.open( filename );
		int LN=0;
		while( in.getLine() ){
			//cout << "LN: " << LN++ << endl;

			p = in.getWord(' ');
			if( p == NULL )
				break;
			p = in.getWord(' ');
			t = atoi(p);
			p = in.getWord(' ');

			p = in.getWord(' ');
			while( p != NULL ){
				x = atoi(p);
				data[x].insert(t);
				p = in.getWord(' ');
			}
		}
		in.close();
		getWholeSet_real();

		for(int i=1; i<=itemNum; i++ )
			whole.sub( data[i], complement[i] );

		saveConvertedFile();
		cout << "done with reading and re-formating file: " << filename << "." << endl << endl;
	}

        void show( bool fLetters ){
                for(int i=1; i<=itemNum; i++){
			if( fLetters )	printf( "t(%c): ", 'A'+i );
			else		cout << i << ": ";
			data[i].show( ",", false );// do not show letters
                        cout << endl;
                }
        }

	int getWholeSet_real(){ 
                for(int i=1; i<=tranNum; i++)
			whole.insert(i);
		cout << endl << "whole set size: " << whole.size();
		cout << endl << endl;
		return whole.size();
	}

	int getWholeSet_syn(){ 
		int  x;
		SIt  it;

                for(int i=1; i<=itemNum; i++)
        		CopyEnd( data[i].S, whole.S );

		sort(whole.S.begin(), whole.S.end());
		it = whole.S.begin();
		x = *it;
		it++;
		while( it != whole.S.end() ){
			while( it!=whole.S.end() && *it == x )
				it = whole.S.erase(it);	
			if( it!=whole.S.end() ){
				x = *it;
				it++;
			}
		}
		cout << endl << "whole set size: " << whole.S.size();
		cout << endl << endl;
		return whole.S.size();
	}
};



struct Node{
	Iset	I; // OR relationship
	Tset	T;
	bool	merged;

	Node(){ merged = false; }

	bool complement( Node &n ){
		for(SIt it1=I.S.begin(); it1!=I.S.end(); it1++ )
		for(SIt it2=n.I.S.begin(); it2!=n.I.S.end(); it2++ )
			if( (*it1 + *it2) == 0 )
				return true;
		return false;
	}

	void clear(){
        	T.S.clear();
        	I.S.clear();
		merged = false;
	}
};
       
typedef vector<Node>		NodeSet;
typedef NodeSet::iterator	NSIT;
                                                                                                                                                              
struct dec_order{
        bool operator()(Node x, Node y){
                if( x.T.S.size() == y.T.S.size() )
                        return *(x.I.S.begin()) < *(y.I.S.begin());
                return x.T.S.size() > y.T.S.size();
        }
};
                                                                                                                                                              
struct inc_order{
        bool operator()(Node x, Node y){
                if( x.T.S.size() == y.T.S.size() )
                        return *(x.I.S.begin()) < *(y.I.S.begin());
                return x.T.S.size() < y.T.S.size();
        }
};



class EquivClasses 
{
	bool fNeg;
public:
	Classes	classes;
	int	min_sup, max_sup, max_item;
	bool	fPrint1, fPrintf;
	int	wholeSize;

	EquivClasses(){
		fPrint1 = true;
		fNeg = false;
	}

	void getClasses( DB &db, int minsup, int maxsup, int maxitem, bool fNegative );
	void EXPAND( Iset C, NodeSet &NS, int basesup, int basehash );
};



void EquivClasses::getClasses( DB &db, int minsup, int maxsup, int maxitem, bool fNegative ){
        Iset      C;
        Node      node;
        NodeSet   NS;
        NSIT      it;
        int       Num = 0;
	int	  basesup=0;
       	int	  basehash=0;

        min_sup = minsup;
        max_sup = maxsup;
        max_item = maxitem;
	classes.pData = db.data;
	classes.pComplement = db.complement;
	fNeg = fNegative;

	if( db.itemNum == 0 )
		return;

	wholeSize = db.whole.size();
	classes.whole = db.whole;

        for( int i=1; i<=db.itemNum; i++ ){
		if( db[i].S.size() >= min_sup && db[i].S.size() <= max_sup){
                	Node node;
			Num++;
                	node.T = db[i];
                	node.I.insert(i);
			NS.push_back( node );
		}
		if( fNegative )
		if( (wholeSize - db[i].size()) >= min_sup && (wholeSize - db[i].size()) <= max_sup ){
			Node  node;
               		Num++;
			node.T = db.complement[i];	
			node.I.insert(-i);
			NS.push_back( node );
		}
        }

	if( Num == 0 ){
		cout << endl << "No t(I)'s is qualified for the range (min_sup -> max_sup)!" << endl;
		return;
	}

	EXPAND( C, NS, basesup, basehash );
}



void EquivClasses::EXPAND( Iset C, NodeSet &NS, int basesup, int basehash ) // base is used for extraset
{
        NSIT    nit1, nit2, tmpit;
        Node    node;
        NodeSet ns1, ns2;
        Iset    C1;
	Tset	base1;
	int	basesup1;
	int	basehash1;      
	int     Total, PrintNum=0;
	bool	fPrint2 = false;
 
	if( fPrint1 ){
		fPrint1 = false;
		fPrint2 = true;
	}

        sort(NS.begin(), NS.end(), dec_order());

        // sibling merging
        for(nit1=NS.begin(); nit1!=NS.end(); nit1++){
                if( nit1->merged )
                        continue;
                node.merged = false;
                node.T = nit1->T;
                node.I = nit1->I;

                tmpit = nit1;
                tmpit++;
                while( tmpit!=NS.end() && tmpit->T.S.size()==node.T.S.size() ){
                        if( !tmpit->merged && tmpit->T==node.T ){
                                tmpit->merged = true;
				node.I += tmpit->I;
			}
                        tmpit++;
                }
                ns1.push_back( node );
        }

	Total = ns1.size();

        for(nit1=ns1.begin(); nit1!=ns1.end(); nit1++){
		if( fPrint2 )	cout << "Done(" << Total << "): " << ++PrintNum << endl;
		
		basesup1 = basesup + nit1->T.size();
		basehash1 = hashVal( nit1->T, basehash );
                if( basesup1 > max_sup )
                        continue;

		Union( C.S, nit1->I.S, C1.S ); // no chance to overlap

		for(nit2=nit1, nit2++; nit2!=ns1.end(); nit2++){
			if( nit1->T.contain( nit2->T ) ){ // pruning on/off, LABEL-1
				C1 += nit2->I;
			}
		}

                //if( !classes.add( base1, C1 )) // possible because of sibling subsumption pruning
                if( !classes.add( basesup1, C1, basehash1 )) // possible because of sibling subsumption pruning
			continue;

                if( C1.S.size() >= max_item )
                       continue;

                node.clear();
                ns2.clear();
                
		for(nit2=nit1, nit2++; nit2!=ns1.end(); nit2++){
			if( fNeg && nit1->complement(*nit2) )
				continue;

			node.I = nit2->I;
			nit2->T.sub( nit1->T, node.T );
			if( !node.T.S.empty() ) // refer to LABEL-1
				ns2.push_back( node );
                }

                if( ns2.size() > 0 )
                        EXPAND( C1, ns2, basesup1, basehash1 );
        }
}
                                                           


void parseInput( int argc, char* argv[], 
	char	* inputf,
	int 	& itemNum, 
	int 	& tranNum,
	float	& density, 
	int	& min_sup,
	int	& max_sup,
	int	& max_item,
	bool	& synData,
	bool	& fOnlyItems, 
	bool	& fLetters,
	bool	& fOutput, 
	bool	& fNegative
){
        if(argc < 2){
                cout << endl << "Usage: " << argv[0] << " [-fFile]|[-iItem -tTransaction -dDensity] -sMinSup -mMaxItem" << endl << endl;
                cout << "==================== OPTIONS =====================" << endl << endl;
		cout << "Input File:          -fString"       << endl;
		cout << "-------------------------------- OR" << endl;
                cout << "Item Number:         -iInteger"      << endl;
                cout << "Transaction Number:  -tInteger"      << endl;
                cout << "Transaction Density: -dFloat"        << endl;
		cout << endl;
                cout << "Transaction min_sup: -sInteger"      << endl;
                cout << "Transaction max_sup: -xInteger"      << endl;
                cout << "Maximal Items:       -mInteger"      << endl;
                cout << "Class Items Only:    -o"             << endl;
                cout << "No output:           -n"             << endl;
                cout << "Complementary Set:   -c"             << endl;
		cout << endl;
                exit(1);
        }

        for(int i=1; i<argc; i++){
                if( argv[i][0] == '-' )
                switch( argv[i][1] ){
			case 'f': strcpy( inputf, argv[i]+2 ); 
				  synData  = false; 
				  fLetters = false;		       break;
                        case 'i': itemNum = atoi( argv[i]+2 );         break;
                        case 't': tranNum = atoi( argv[i]+2 );         break;
                        case 'd': density = atof( argv[i]+2 );         break;
                        case 's': min_sup = atoi( argv[i]+2 );         break;
                        case 'x': max_sup = atoi( argv[i]+2 );         break;
                        case 'm': max_item = atoi( argv[i]+2 );        break;
                        case 'o': fOnlyItems = true;                   break;
                        case 'n': fOutput = false;                     break;
                        case 'c': fNegative = true;                    break;
                        default: cout << "Error Input Options: " << argv[i] << endl; exit(0); break;
                }
                else{	cout << "Error Input Options: " << argv[i] << endl;  exit(0); }
        }

	cout << endl;
	for(int i=0; i<argc; i++)
		cout << argv[i] << " ";
	cout << endl << endl;

	if(synData){
        	cout << "itemNum:  " << itemNum << endl;
        	cout << "tranNum:  " << tranNum << endl;
        	cout << "density:  " << density << endl;
	}
        cout << "min_sup:  " << min_sup << endl;
	cout << "max_sup:  " << max_sup << endl;
        cout << "max_item: " << max_item<< endl;
	cout << endl;
}



int main(int argc, char *argv[])
{
	char	inputf[100];
	int 	itemNum;
	int	tranNum;
	float	density;
	int	min_sup = 1;
	int	max_sup = 10000000;
	int	max_item = 100000;
	bool	synData = true;
	bool	fOnlyItems = false; 
	bool	fOutput = true; 
	bool	fLetters = true;
	bool	fNegative = false;

	EquivClasses	XUGs; 
	DB		db;

	parseInput( argc, argv, inputf, itemNum, tranNum, density, min_sup, max_sup, max_item, 
		    synData, fOnlyItems, fLetters, fOutput, fNegative );
	
	srand( time(NULL) );
	//srand( 130 );

	if(synData){
		db.synInit( itemNum, tranNum, density );
		cout << "done with synthetic data generation!" << endl << endl;
	}
	else{
		db.readsize( inputf );
		db.readfile( inputf );
		cout << "done with file reading and re-formating!" << endl << endl;
	}

	if( fOutput ){	
		db.show( fLetters );
		XUGs.fPrint1 = false;
	}

	XUGs.getClasses( db, min_sup, max_sup, max_item, fNegative ); 
	XUGs.classes.show( fOnlyItems, fLetters, fOutput );

	#ifdef MEM_LEAK_CHECK	
		showRegPointers();	
	#endif
	
	return 0;
}

