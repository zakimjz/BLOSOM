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
#define CONTAIN		1
#define BE_COND		2
typedef vector<int>	SET;
typedef SET::iterator	SIt;
class Sets;

class Set{
public:
	SET	S;
	
	void clear(){
		S.clear();
	}

	void insert( int x ){
		S.push_back(x);
		//sort( S.begin(), S.end() ); // need to change
	}

	int size() const {
		return S.size();
	}

	void sub( Set &s, Set &r ){
		SIt it1, it2;

		r.clear();
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

	void getTset( bool fUnion, Tset* data, Tset* complement, Tset &T ){
		Tset	TS[2];
		int	idx=0;
		int	n;

		if( fUnion ){
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
		}
		else{
			if( (*S.begin()) > 0 )
				TS[idx] = data[ *S.begin() ];
			else	TS[idx] = complement[ -(*S.begin()) ];

			for(SIt it=S.begin(); it!=S.end(); it++){
				if( *it > 0 ){
					Intersect( data[*it].S, TS[idx].S, TS[1-idx].S );
					idx = 1-idx;
				}
				else{
					n = -(*it);
					Intersect( complement[n].S, TS[idx].S, TS[1-idx].S );
					idx = 1-idx;
				}
			}
		}
		T = TS[idx];
	}

	bool operator==( Set &s ){ 
		SIt it1, it2;
		if(S.size() != s.size())
			return false;
		for(it1=S.begin(), it2=s.S.begin(); it1!=S.end(); it1++, it2++)
			if(*it1 != *it2)  return false;
		return true;
	}
	
	bool operator!=( Set &s ){ 
		SIt it1, it2;
		if(S.size() != s.size())
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

		if( it != S.end() ){
			x = *it;
			it++;
		}

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

		if(S.size() < s.size())	return true;
		else if(S.size() > s.size())	return false;
		else{ 
			for(it1=S.begin(), it2=s.S.begin(); it1!=S.end(); it1++, it2++)
				if( *it1 < *it2 )	return true;	
				else if( *it1 > *it2 )	return false;
			return false; //==
		}
	}

	bool contain( Set &s ){
		SIt it1, it2;

		if( S.size() < s.size() )
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

	int hashVal(){
		SIt it;
		int ret = 0;
		for(it=S.begin(); it!=S.end(); it++)
			ret = (ret + *it) % 32768; 
		return ret;
	}

	int sum(){
		SIt it;
		int ret = 0;
		for(it=S.begin(); it!=S.end(); it++)
			ret += *it; 
		return ret;
	}

	void show( char *seperation, bool letter ){
		SIt it;
		for(it=S.begin(); it!=S.end(); it++){
			if(it != S.begin()) 
				cout << seperation;
			if( letter )
				cout << char(*it+'A');
			else
				cout << *it;
		}
	}
};



typedef vector<Set>		SETS;
typedef vector<Set>::iterator	SSIt;

class Sets{
public:
	SETS	SS;

	void clear(){
		SS.clear();
	}

	int size(){
		return SS.size();
	}

	void insert( Set &s ){
		SS.push_back( s );
	}
	
	bool add( Set &s ){
		SSIt	it;
		int	ssz;

		ssz = s.size();

		it=SS.begin();
		while(it != SS.end()){
			if( it->size() == ssz ){	// no subsumption
				it++;
				continue;
			}
			//Intersect( it->S, s.S, r );
			if( it->contain(s) )	// subsumed by s
				it = SS.erase(it);
			//else if( r.size() == itsz )// subsumed by *it (impossible)
			else	it++;
		}
		SS.push_back(s);
		return true;
	}

	bool L2add( Set &s, Sets *pIsets ){ // for level 2
		SSIt	it;
		int	ret, ssz;

		if( L2complement(s, pIsets) )
			return false;

		ssz = s.size();

		it=SS.begin();
		while(it != SS.end()){
			if( it->size() != ssz && it->contain(s) ){
				it = SS.erase(it); // subsumed by s
			}
			else
			{ 
				ret = L2contain( *it, s, pIsets );
				if( ret == CONTAIN )		it = SS.erase(it); // subsumed by s at level 2
				else if( ret == BE_COND )	return false;	   // s is subsumed at level 2
				else it++;
			}
		}
		SS.push_back(s);
		return true;
	}

	bool L2complement( Set &s, Sets *p ){
		SIt	it1, it2;

		for(it1=s.S.begin(); it1!=s.S.end(); it1++)
		if( p[*it1].size() == 1 )
		for(it2=it1+1; it2!=s.S.end(); it2++)
		if( p[*it2].size() == 1 ){
			SSIt sit1 = p[*it1].SS.begin();
			SSIt sit2 = p[*it2].SS.begin();
			if( sit1->size()==1 && sit2->size()==1 ){
				SIt it3 = (*sit1).S.begin();
				SIt it4 = (*sit2).S.begin();
				if( (*it3 + *it4) == 0 )
					return true;
			}
		}
		return false;
	}

	int L2contain( Set &v1, Set &v2, Sets* p ){ // p points Isets indexed by v1 and v2
		//return 0; // switch of L2 subsumption checking

		if( L2cover(v2,v1,p) )		return CONTAIN;
		else if( L2cover(v1,v2,p) )	return BE_COND;
		else				return 0;
	}

	// e.g. {A|B,A|C}&{H} is covered by {A,D}&{H}
	// need to handle if one SSS just cover part of the other, e.g. {C} just cover part of {A|B, A|C}

	bool L2cover( Set &v1, Set &v2, Sets *p ){ // any Isets indexed by v1 should cover some Isets indexed by v2
		SIt	it1, it2;

		if(v1.size() > v2.size())	return false;
		for(it1=v1.S.begin(); it1!=v1.S.end(); it1++){
			for(it2=v2.S.begin(); it2!=v2.S.end(); it2++){
				if( p[*it1].cover( p[*it2]) )
					break;
			}
			if(it2 == v2.S.end())
				return false;
		}
		return true;
	}

	bool cover( Sets &s ){ // can cover the larger sets, e.g, {C} cover {C|F, C|B}, their sizes relationship may be > or < 
		SSIt	it1, it2;

		for(it1=s.SS.begin(); it1!=s.SS.end(); it1++){ // all the s sets should be covered
			for(it2=SS.begin(); it2!=SS.end(); it2++){
				if( it1->contain(*it2) )
					break;
			}
			if(it2 == SS.end())
				return false;
		}
		return true;
	}

	void show( char *sepOut, char *sepIn, bool fLetters ){
		SSIt it;
		//if( SS.size() > 1 )	cout << "{";
		cout << "{";
		for(it=SS.begin(); it!=SS.end(); it++){
			if(it != SS.begin()) 
				cout << sepOut;
			it->show( sepIn, fLetters );	// show letters
		}
		cout << "}";
		//if( SS.size() > 1 )	cout << "}";
	}
};



class Class{
public:
	Set	tset;	// generally not used
	Sets	isets;
	int 	sum, sup;

	void clear(){
		tset.clear();
		isets.clear();
	}
	
	void show( bool only_items, bool fLetters, char *sep ){
		Set T;
		if( !only_items ){
			tset.show( " ", false ); // do not show letters	
			cout << ":\t";
		}
		isets.show( ", ", sep, fLetters );
	}
};



struct POS{ SIt  Cur, Begin, End; };
class itMod{
public:
	vector<POS>		pos;
	vector<POS>::iterator	p;
	POS	n;
	SSIt	sit;
	bool	none;

	void clear(){
		pos.clear();
	}

	void input( Sets &c ){
		for(sit=c.SS.begin(); sit!=c.SS.end(); sit++){
			n.Cur = n.Begin = sit->S.begin();
			n.End = sit->S.end();
			pos.push_back( n ); // save postions
		}
		none = false;
	}

	bool getComb( Set &s ){
		if( none )
			return false;

		s.clear();
		p = pos.begin();

		Loop:;
		s.insert( *(p->Cur) );
		p->Cur++;

		if( p->Cur==p->End ){
			p->Cur = p->Begin;
			p++;
			if( p==pos.end() ){
				none = true;
				return true;
			}
			goto Loop;
		}
		else	p++;

		for(; p!= pos.end(); p++)
			s.insert( *(p->Cur) );
		return true;
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
	bool		fUnion;
	bool		fLevel2; // for level 2
	Sets		*pIsets; // for level 2
	Tset		*pData;
	Tset		*pComplement;
	bool		fFormTsets;

	Classes(int sz=HASH_SIZE): HT(sz){
		fLevel2 = false;
		fFormTsets = false;
	}

	void formTsets(){
		SSIt  sit;

		if( fFormTsets )
			return;

		cout << endl << "Please Wait forming Tsets ..." << endl << endl;
		int num = 1;

		for(myHashIt hit=HT.begin(); hit!=HT.end(); hit++, num++){
			if( num%1000 == 0 )
				cout << "num: " << num << endl;
			sit = hit->second.isets.SS.begin(); 
			sit->getTset( fUnion, pData, pComplement, hit->second.tset );
		}
		fFormTsets = true;
	}

	void show( bool only_items, bool fLetters, bool fOutput ){
		myHashIt  it;
		int sum = 0;

		if( !only_items && fOutput )
			formTsets();

		if( fUnion )	cout << endl << endl << "MUG Classes:" << endl << endl;
		else		cout << endl << endl << "MNG Classes:" << endl << endl;

		if( fOutput ){
			for(it=HT.begin(); it!=HT.end(); it++){
				if(it != HT.begin())
					cout << "\n";
				if( fUnion )	it->second.show( only_items, fLetters, "|" );
				else		it->second.show( only_items, fLetters, "&" );
				sum += it->second.isets.size();
			}
			cout << endl << endl;
		}
		else{
			for(it=HT.begin(); it!=HT.end(); it++)
				sum += it->second.isets.size();
		}
		cout << "#Tset: " << HT.size()  << endl;
		cout << "#Iset: " << sum << endl << endl;
	}

	void show2( Sets *isets, bool only_items, bool fLetters, bool fUnionOut, bool fOutput ){
		myHashIt  hit;
		SSIt 	  sit;
		SIt 	  it;
		char	  inc[2], outc[2];
		int sum = 0;

		formTsets();

		if( fUnionOut ){	
			cout << endl << endl << "MDG Classes:" << endl << endl;
			strcpy( inc, "&" );	
			strcpy( outc, "|" );
		}	
		else{
			cout << endl << endl << "MCG Classes:" << endl << endl;
			strcpy( inc, "|" );	
			strcpy( outc, "&" );
		}	

		if( fOutput ){
			for(hit=HT.begin(); hit!=HT.end(); hit++){
				if(hit != HT.begin())
					cout << "\n";
				if( !only_items ){
					hit->second.tset.show( " ", false ); // do not show letters, SAT8
					cout << ":\t";
				}
				for(sit=hit->second.isets.SS.begin(); sit!=hit->second.isets.SS.end(); sit++){
					if(sit != hit->second.isets.SS.begin())
						cout << ", ";
					for(it=sit->S.begin(); it!=sit->S.end(); it++){
						if( it != sit->S.begin() )
							cout << outc;
						isets[*it].show( ",", inc, fLetters );
					}
				}
				sum += hit->second.isets.size();
			}
			cout << endl << endl;
		}
		else{
			for(hit=HT.begin(); hit!=HT.end(); hit++)
				sum += hit->second.isets.size();
		}
		cout << "#Tset: " << HT.size()  << endl;
		cout << "#Iset: " << sum << endl << endl;
	}

	bool combine(  Sets &c, Sets &isets ){
		itMod	itmod;
		Set	s;
		bool	ret = false;

		itmod.input( c );

		while( itmod.getComb(s) ){
			sort( s.S.begin(), s.S.end() );
			if( fLevel2 ){
				if( isets.L2add(s, pIsets) )
					ret = true;
			}
			else{
				if( isets.add(s) )
					ret = true;
			}
		}
		return ret;
	}

	bool combine1(  Sets &c, Class &cl ){
		itMod	itmod;
		Set	s;
		bool	ret = false;

		itmod.input( c );

		while( itmod.getComb(s) ){
			sort( s.S.begin(), s.S.end() );
			if( fLevel2 ){
				if( cl.isets.L2add( s, pIsets ) )
					ret = true;
			}
			else{	
				cl.isets.insert( s );
				ret = true;
			}
		}
		return ret;
	}

	bool add( Sets &Is, int sum, int sup ){  
   		myHashIt   	hi;
		myHashFind 	p;
   		int 		h, ires;
		Class		c;

   		h = HT.hash_funct()( sum % 32768 ); // possibly need to change
		p = HT.equal_range( h );
                                         
		if( p.first==p.second )
			goto ADD;

   		for(hi=p.first; hi!=p.second; hi++){
			if( hi->second.sum != sum || hi->second.sup != sup )
				continue;
			/*if( hi->second.tset != c.tset ){
				//cout << "Got You" << endl;
				continue;
			}*/
			return combine(  Is, hi->second.isets );// Is updating is unnecessary, no subsumption (tested)
   		}

		ADD:; 
		c.sup = sup;
		c.sum = sum;
		c.isets.clear();
		if( combine1( Is, c ) ){
   			HT.insert( myHTPair(h,c) );
			return true;
		}
		return false;
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
	Sets	*isets; // for db2 show
	Set	whole;
	int	itemNum, tranNum;
	bool	fConverted;
	char	convertedName[50];
	
	~DB(){
		if( data != NULL ){  delete[] data;  data = NULL; }
		if( complement != NULL ){  delete[] complement;  complement = NULL; }
		if( isets != NULL ){ delete[] isets; isets = NULL; }
	}

	Set &operator[]( int i ){
		return data[i];
	}
	
	DB(){
		itemNum = 0;
		tranNum = 0;
		data = NULL;
		complement = NULL;
		isets = NULL;
		fConverted = false;
	}

	void ClassInit( Classes &cs ){ // for second level relationship
		myHashIt  it;
		int i;
		//clear();
		cs.formTsets();

		itemNum = cs.HT.size();
                data  = new Tset[itemNum+1];
                complement = new Tset[itemNum+1];
                isets = new Sets[itemNum+1];
		whole = cs.whole; // getWholeSet();
		for(it=cs.HT.begin(),i=1; it!=cs.HT.end(); it++,i++){
			data[i] = it->second.tset;	// SAT8
			whole.sub( it->second.tset, complement[i] );	// SAT8
			isets[i] = it->second.isets;	// for db2 show
		}

	}

        void synInit( int iNum, int tNum, float density ){
		Tset	ws;
		SIt	it;
		int i, j, length;

		itemNum = iNum;
		tranNum = tNum;
		isets = NULL;
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
			while( data[i].size() > length ){
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
		whole.clear();
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

		isets = NULL;
                data = new Tset[itemNum+1];
		assert( data != NULL );
                complement = new Tset[itemNum+1];
		assert( complement != NULL );

		in.open( filename );
		while( in.getLine() ){
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
		//getWholeSet_syn();

		for(int i=1; i<=itemNum; i++ )
			whole.sub( data[i], complement[i] );

		saveConvertedFile();
		cout << "done with reading and re-formating file: " << filename << "." << endl << endl;
	}

        void show( bool fLetters ){
                for(int i=1; i<=itemNum; i++){
			if( fLetters )	printf( "t(%c): ", 'A'+i );
			else		cout << i << ": ";
			//cout << "size: " << data[i].S.size() << "  ";
			data[i].show( " ", false );// do not show letters
                        cout << endl;
                }
        }

	int getWholeSet_real(){  // need to pay attention to the zeros, which may change 50543/70446 for chess300.ascii with minsup=60
                for(int i=1; i<=tranNum; i++)
			whole.insert(i);
		cout << endl << "whole set size: " << whole.size();
		cout << endl << endl;
		return whole.size();
	}

	int getWholeSet_syn(){ 
		int  x;
		SIt  it;

		whole.clear();
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
		tranNum = whole.size();
		cout << endl << "whole set size: " << whole.size();
		cout << endl << endl;
		return whole.size();
	}
}; 



class Node
{
public:
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
        	T.clear();
        	I.clear();
		merged = false;
	}
};
      

 
typedef vector<Node>		NodeSet;
typedef NodeSet::iterator	NSIT;
                                                                                                                                                              
struct dec_order{ bool operator()(Node x, Node y){ return x.T.size() > y.T.size(); }};                                                                                                                                                              
struct inc_order{ bool operator()(Node x, Node y){ return x.T.size() < y.T.size(); }};



class EquivClasses 
{
	int max_sup, min_sup, max_item;
	bool fNeg;
public:
	Classes	classes;
	Sets*	isets;
	bool	fPrint1, fPrintf;
	/////////////////////////
	Set	whole; // temp
	int	wholeSize;

	EquivClasses(){
		fPrint1 = true;
		fNeg = false;
	}

	void getClasses( DB &db, int minsup, int maxsup, int maxitem, bool fUnion, bool fNegative );
	void EXPAND( bool fUnion, const Sets &C, int sum, int sup, NodeSet &NS );
};

                                                                                                                                                              

void EquivClasses::getClasses( DB &db, int minsup, int maxsup, int maxitem, bool fUnion, bool fNegative ){// mug/mng
	NodeSet ns;
	NSIT    nit;
	Sets	C;
	Set	PT;
        int	i, Num = 0;

	max_sup = maxsup;
	min_sup = minsup;
	max_item = maxitem;

	if( db.itemNum == 0 )
		return;

	if( db.isets != NULL )
		classes.fLevel2 = true;
        classes.pIsets = db.isets; // only valid for level 2
        classes.whole = db.whole;
	classes.pData = db.data;
	classes.pComplement = db.complement;
	fNeg = fNegative;

        whole = db.whole; // temp
	wholeSize = whole.size();

        for(i=1; i<=db.itemNum; i++ ){
		if( db[i].size() >= min_sup && db[i].size() <= max_sup ){// need to distinguish 2 kinds of min_sup's for MUG
			Node	node;
               		Num++;
			if( fUnion )	node.T = db[i];
                        else		node.T = db.complement[i];	// complement set
			node.I.insert(i);
			ns.push_back( node );
		}
		if( fNegative )
		if( (wholeSize - db[i].size()) >= min_sup && (wholeSize - db[i].size()) <= max_sup ){
			Node	node;
               		Num++;
			if( fUnion )	node.T = db.complement[i];	
                        else		node.T = db[i];	
			node.I.insert(-i);
			ns.push_back( node );
		}
	}

	if( Num == 0 ){
		cout << endl << "No t(I)'s is qualified for the range (min_sup -> max_sup)!" << endl;
		return;
	}

	//db.show(false);

	EXPAND( fUnion, C, 0, 0, ns );

        classes.fUnion = fUnion;

	//////////////////////////// seperate the isets having same signature function values
#if 1
	vector<Class>	CV, CVT;
	vector<Class>::iterator	cit;
	Tset		T, KeepT;
	SSIt 		sit;
	 

	for(myHashIt hit = classes.HT.begin(); hit != classes.HT.end(); hit++){
		if( hit->second.isets.size() == 1 )
			continue;

		sit = hit->second.isets.SS.begin(); 
		sit->getTset( fUnion, db.data, db.complement, KeepT );
		sit++;
		while( sit != hit->second.isets.SS.end() ){
			sit->getTset( fUnion, db.data, db.complement, T );

			if( T == KeepT ){
				sit++;
				continue;
			}

			bool fFound = false;
			for(cit=CV.begin(); cit!=CV.end(); cit++){
				if( T == cit->tset ){
					cit->isets.SS.push_back( *sit );
					fFound = true;		
					break;
				}
			}
			if( !fFound ){
				Class c;
				c.tset = T;
				c.isets.insert( *sit );
				CV.push_back( c );
			}
			sit = hit->second.isets.SS.erase(sit);
		}
		CopyEnd( CV, CVT );
		CV.clear();
	}

	for(cit=CVT.begin(); cit!=CVT.end(); cit++){
		int h = classes.HT.hash_funct()( cit->tset.sum() % 32768 ); // possibly need to change
		classes.HT.insert( myHTPair(h,*cit) );
	}

#endif
}



void EquivClasses::EXPAND( bool fUnion, const Sets &C, int sum, int sup, NodeSet &NS ) // fUnion, absolute parameter this time
{
        NSIT    nit1, nit2, tmpit;
        Node    node;
        NodeSet ns1, ns2;
        Sets    C1;
	Set	PT1;
        int     Total, PrintNum=0;
	bool	fPrint2 = false;
	int	sum1, sup1;
        
	if( fPrint1 ){
		fPrint1 = false;
		fPrint2 = true;
	}

	sort( NS.begin(), NS.end(), dec_order() );

        // sibling merging
        for(nit1=NS.begin(); nit1!=NS.end(); nit1++){
                if( nit1->merged )
                        continue;
                node.T.S.clear();
                node.I.S.clear();
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
		if( fPrint2 )	cout << "Processing(" << Total << "): " << ++PrintNum << endl;
		
                C1 = C;
                C1.insert( nit1->I );

		sup1 = sup + nit1->T.size();

		if( fUnion && sup1 > max_sup || 
		   !fUnion && sup1 > wholeSize-min_sup )
			continue;

		sum1 = sum + nit1->T.sum();
		
		if( !classes.add( C1, sum1, sup1 ))
			continue;

                if( C1.size() >= max_item )
                        continue;
                                                                           
                node.merged = false;
		for(nit2=ns2.begin(); nit2!=ns2.end(); nit2++) 
			nit2->clear();
		ns2.clear();
                
		for(nit2=nit1, nit2++; nit2!=ns1.end(); nit2++){
			if( fNeg && nit1->complement(*nit2) )
				continue;

			nit2->T.sub( nit1->T, node.T );
                        if(node.T.S.size() > 0 ){ // son is not subsumed by parents
	                        node.I = nit2->I;
                                ns2.push_back( node );
			}
                }
		if( ns2.size() > 0 )
			EXPAND( fUnion, C1, sum1, sup1, ns2 );
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
	bool	* algorithm, 
	bool	& fNegative
){
        if(argc < 2){
                cout << endl << "Usage: " << argv[0] << " [-fFile]|[-iItem -tTransaction -dDensity] -sMinSup -mMaxItem -aNU" << endl << endl;
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
                cout << "algorithm:           -aString /*U,N,D or/and C*/" << endl;
                cout << "Items Only w/o DB:   -o"             << endl;
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
                        case 'a': for(int j=2; argv[i][j]!='\0'; j++){
					switch(argv[i][j]){
						case 'U': algorithm[0] = true; break;
						case 'N': algorithm[1] = true; break;
						case 'D': algorithm[2] = true; break;
						case 'C': algorithm[3] = true; break;
                        			default: cout << "Error Input Options: " << argv[i] << endl; exit(0); break;
					}
				  }
			          break;
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
	bool	algorithm[4];
	bool	fNegative = false;

	EquivClasses	MUGs, MNGs, MDGs, MCGs; 
	DB 		db, db2C, db2D;

	for( int i=0; i<4; i++ )
		algorithm[i] = false; // MUG MNG MDG MCG 

	parseInput( argc, argv, inputf, itemNum, tranNum, density, min_sup, max_sup, max_item, 
		    synData, fOnlyItems, fLetters, fOutput, algorithm, fNegative );
	
	//srand( time(NULL) );
	srand( 130 );

	if(synData){
		db.synInit( itemNum, tranNum, density );
		cout << "done with synthetic data generation!" << endl << endl;
	}
	else{
		db.readsize( inputf );
		db.readfile( inputf );
	}

	if( fOutput ){
		if( !fOnlyItems )	
			db.show( fLetters );
		MUGs.fPrint1 = MNGs.fPrint1 = MDGs.fPrint1 = MCGs.fPrint1 = false;
	}

        //////////////////////////////////////////////////////////////////////////
	if( algorithm[0] || algorithm[3] ){
		MUGs.getClasses( db, min_sup, max_sup, max_item, true, fNegative ); //fUnion
		if( algorithm[3] ){ // MCG
			db2C.ClassInit( MUGs.classes ); 
			MCGs.getClasses( db2C, min_sup, 1000000, max_item, false, false );  // fUnion, fNegative
		}
	}

	if( algorithm[1] || algorithm[2] ){ 
		MNGs.getClasses( db, min_sup, max_sup, max_item, false, fNegative ); 
		if( algorithm[2] ){ // MDG
			db2D.ClassInit( MNGs.classes );
			MDGs.getClasses( db2D, 1, max_sup, max_item, true, false ); // fUnion, fNegative
		}
	}
        //////////////////////////////////////////////////////////////////////////

	if( algorithm[0] )  MUGs.classes.show( fOnlyItems, fLetters, fOutput );
	if( algorithm[1] )  MNGs.classes.show( fOnlyItems, fLetters, fOutput );
	if( algorithm[3] )  MCGs.classes.show2( db2C.isets, fOnlyItems, fLetters, false, fOutput ); // fUnionOut
	if( algorithm[2] )  MDGs.classes.show2( db2D.isets, fOnlyItems, fLetters, true, fOutput );  // fUnionOut

	#ifdef MEM_LEAK_CHECK
		showRegPointers();	
	#endif
	return 0;
}

