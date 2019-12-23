#include <bits/stdc++.h>

using namespace std;

#define MAX 100007
#define f first
#define s second
// #define s.f second.first
// #define s.s second.second
#define fo(i,a,b) for(int i=a;i<b;i++)
#define fo2(i,a,b) for(int i=a;i<b;i*=2)



vector<int> buildSuffArr(char txt[MAX], int n);
int comp(pair<int,pair<int,int> > a, pair<int,pair<int,int> > b);
vector<pair<int,pair<int,int>>> radixSort(vector<pair<int,pair<int,int>>>& arr);
void sortByIndex(vector<pair<int,pair<int,int>>>& arr,int c);
vector<int> kasai(char txt[MAX], vector<int>& suffixArr);
int *constrSegmTree(vector<int>& lcp);//C++ doesn't allow returning array; hence, returning ptr to int instead
int _constrSegmTree(vector<int>& lcp, int l, int h, int *segmTree, int idx);
int rangeMinQuery(int *segmTree, int n, int qstart, int qend);
int _rangeMinQuery(int *segmTree, int l, int h, int qstart, int qend, int idx);
int lenOfLongestRepeatedSubstr(int *segmTree, int k, int n);

// void printArr(vector<int>& arr, int n) 
// { 
//     for (int i = 0; i < n; i++) 
//         cout << arr[i] << " "; 
//     cout << endl; 
// }  

// void printArr1(int* arr, int n) 
// { 

//     for (int i = 0; i < n; i++) 
//         cout << arr[i] << " "; 
//     cout << endl; 
// }
int main(){
	int n, k;
	cin>>n>>k;
	char txt[n];   
    cin >> txt ;
    // int n = strlen(txt);
    vector<int> suffixArr = buildSuffArr(txt, n); 
    int n1 = suffixArr.size(); 
  
    // cout << "Suffix Array : \n"; 
    // printArr(suffixArr, n1); 
    // cout<<endl;
  
    vector<int> lcp = kasai(txt, suffixArr); 
    // cout << buildSuffArr(txt, n)<< endl;

    // cout << "LCP Array : \n"; 
    // printArr(lcp, n1); 

    int *segmTree=constrSegmTree(lcp);

    // cout<<"Segment Tree constructed!"<<endl;
	int ht=(int)(ceil(log2(n1)));
	int segmTSize=2*(int)pow(2, ht) - 1;
    // printArr1(segmTree, segmTSize); 
    // cout<<endl;

    cout<<lenOfLongestRepeatedSubstr(segmTree, k, n1)<<endl;
  
    // Print minimum value in arr[qs..qe]  
    // cout<<"Minimum of values in range ["<<qs<<", "<<qe<<"] "<< 
    // "is = "<<rangeMinQuery(segmTree, n1, qs, qe)<<endl;
	return 0 ;
}

vector<int> buildSuffArr(char txt[MAX], int n){
	/*Each elem of suff vector has 3 things(in the foll order:
	1- index (of substring in the text string)
	2- rank
	3- next rank

	suff stores the sorted suffixes of given string
	*/
	vector<pair<int,pair<int,int> > > suff(n);

	// suff[0].f=1;
	// suff[0].s.f=2;
	// suff[0].s.s=3;

	// cout<<suff[0].f<<endl;
	// cout<<suff[0].s.f<<endl;

	int idx[n];

	/*For a string of length n, n suffixes are possible*/

	fo(i,0,n-1){
		suff[i].f=i;
		suff[i].s.f=(int)txt[i];
		suff[i].s.s=(int)txt[i+1];
		// suff[i].s.f=txt[i]-'a';
		// suff[i].s.s=txt[i+1]-'a';
	}

	suff[n-1].f=n-1;
	suff[n-1].s.f=(int)txt[n-1];
	// suff[n-1].s.f=txt[n-1]-'a';
	suff[n-1].s.s=-1;

	/*The above computes rank and nextRank as per the 1st and the 2nd characters (2^0 and 2^1) in each of the suffixes*/

	// stable_sort(suff.begin(), suff.end(), comp);
	suff=radixSort(suff);

	int lim=2*n;
	fo2(k,4,lim){
		int temprank=suff[0].s.f;
		suff[0].s.f=0;//Need to store the rank coz it'll get updated with a new rank; no need to store nextRank, as it doesn't get overwritten

		//updating ranks of each suffix
		fo(i,1,n){
			if(suff[i].s.f == temprank && suff[i].s.s == suff[i-1].s.s){
				suff[i].s.f=suff[i-1].s.f;//assigning i the same rank as that of i-1, coz both rank and nextRank are same for ith and i-1th suffix
			}
			else{
				temprank=suff[i].s.f;
				suff[i].s.f=suff[i-1].s.f+1;//suffix at i greater than suffix at i-1, hence adding 1 to the rank of i-1th suffix
			}
		}

		/*This stores where each "index" of a suffix can be found in the suff vector*/
		fo(i,0,n){
			idx[suff[i].f]=i;
		}

		//updating nextRanks of each suffix
		fo(i,0,n){
			int nextidx=suff[i].f+k/2;//giving rank as per 2^2, 2^4 ann so on characters
			int id;
			if(nextidx<n){
				suff[i].s.s=suff[idx[nextidx]].s.f;
			}
			else{
				suff[i].s.s=-1;
				// suff[i].s.s=suff[idx[nextidx%n]].s.f;// Imp step for lexicographic "rotation"
			}
		}

		// stable_sort(suff.begin(), suff.end(), comp);
		suff=radixSort(suff);
	}

	// cout<<"Here before suffixArr"<<endl;

	//Indexes of all sorted suffixes in the suffix array stored in suffixArr
	vector<int> suffixArr;
	fo(i,0,n){
		suffixArr.push_back(suff[i].f); 
	} 
    
    return suffixArr; 
}

vector<int> kasai(char txt[MAX], vector<int>& suffixArr){
	vector<int> lcp(suffixArr.size(), 0);
	int n=suffixArr.size(), lenLCP=0;

	//Maps the "index" of each suffix to their pos in the sorted suffix array SA
	vector<int> idxToArrPos(n,0);

	fo(i,0,n){
		idxToArrPos[suffixArr[i] ]=i;
	}

	fo(i,0,n){
		if(idxToArrPos[i]==n-1){
			lenLCP=0;
			continue;
		}

		//j will have the next consecutive suffix to ith suffix (as per the sorted suffix array)
		int j=suffixArr[idxToArrPos[i]+1];

		while(i+lenLCP<n && j+lenLCP<n && txt[i+lenLCP]==txt[j+lenLCP]){ 
            lenLCP++; 
        }

        lcp[idxToArrPos[i]]=lenLCP;

        if(lenLCP>0){
        	lenLCP--;
        }
	}

	return lcp;
}

int *constrSegmTree(vector<int>& lcp){
	// cout<<"Inside segment tree constr"<<endl;
	int n=lcp.size();
	int ht=(int)(ceil(log2(n)));

	// int numOfNodesInSegmTree=2*n-1;
	int numOfNodesInSegmTree=2*(int)pow(2, ht) - 1;

	int *segmTree=new int[numOfNodesInSegmTree];

	_constrSegmTree(lcp, 0, n-1, segmTree, 0);

	return segmTree;
}

int _constrSegmTree(vector<int>& lcp, int l, int h, int *segmTree, int idx){
     
    // cout<<"inside segment tree constr util"<<endl;
    //If only 1 elem in lcp                          
	if(l==h){
		segmTree[idx]=lcp[l];
		return lcp[l];
	}

	int mid=l+(h-l)/2;
	//recurring for left and right childs(subtrees) and storing min of 2 leafs in this node idx
	segmTree[idx]=min(_constrSegmTree(lcp,l,mid,segmTree,idx*2+1),
					_constrSegmTree(lcp,mid+1,h,segmTree,idx*2+2));
	return segmTree[idx];

}

int rangeMinQuery(int *segmTree, int n, int qstart, int qend){
	// cout<<"inside rangeMinQuery"<<endl;

	if(qstart<0 || qend >=n || qstart>qend){
		cout<<"Invalid range : Out of bound"<<endl;
		return -1;
	}

	int x=_rangeMinQuery(segmTree, 0, n-1, qstart, qend, 0);
	return x;
}

/*
    idx --> Index of current node in the  
           segment tree. Initially 0 is  
           passed as root is always at index 0  
    l & h --> Starting and ending indexes  
                of the segment represented  
                by current node, i.e., st[index]  
    qstart & qend --> Starting and ending indexes of query range 
*/
int _rangeMinQuery(int *segmTree, int l, int h, int qstart, int qend, int idx){
	
	// cout<<"inside _rangeMinQuery util: l "<<l<<" h: "<<h<<" idx: "<<idx<<endl;
	//segment is in range
	if(l>=qstart && h<=qend){
		// cout<<"TRUE"<<endl;
		return segmTree[idx];//return root of that segment's subtree
	}

	//segment outside the range
	if(l>qend || h<qstart){
		return INT_MAX;
	}

	//part of segment overlaps with the range
	int mid=l+(h-l)/2;
	return min(_rangeMinQuery(segmTree,l, mid, qstart, qend, 2*idx+1),
		_rangeMinQuery(segmTree, mid+1,h, qstart, qend, 2*idx+2));

}

int lenOfLongestRepeatedSubstr(int *segmTree, int k, int n){
	int maxLen=-1;
	fo(i,0, n-k+1){
		int qstart=i;
		int qend=i+k-2;
		/*-2 done coz as per kasai algo, lcp[i]=longest common prefix btw suffix i and i+1. So, let's say k=3;if I had to query 
		min btw suffixes 0,1,2 I'd just have to access lcp[0](for suffix 0-1), lcp[1](for suffix 1-2) i.e till i+k-2(when i=0)
		*/
		int x=rangeMinQuery(segmTree, n, qstart, qend);

		// cout<<"qstart: "<<qstart<<", qend: "<<qend<<" lcs in range: "<<x<<endl;
		maxLen=max(maxLen,x);
		// cout<<"maxLen "<<maxLen<<endl;
	}

	if(maxLen==0){//when no substring occurs k times
		return -1;
	}

	return maxLen;
}

int comp(pair<int,pair<int,int> > a, pair<int,pair<int,int> > b){
	if(a.s.f == b.s.f){
		return a.s.s<b.s.s;
	}
	else{
		return a.s.f<b.s.f;
	}
}

void sortByIndex(vector<pair<int,pair<int,int>>>& arr,int c){
	int count[MAX]={0};
	
	for(int j=0; j<arr.size(); j++){
		switch(c){
			case 1:
				count[arr[j].second.first+1]++;
				break;
			case 2:
				count[arr[j].second.second+1]++;
				break;
		}
		
	}
	for(int j=1; j<=MAX; j++)
		count[j] += count[j-1];
	vector<pair<int,pair<int,int>>> sorted(arr.size());
	for(int j=(arr.size()-1); j>=0; j--){
		switch(c){
			case 1:
				sorted[count[arr[j].second.first+1]-1]=arr[j];
				count[arr[j].second.first+1]--;
				break;
			case 2:
				sorted[count[arr[j].second.second+1]-1]=arr[j];
				count[arr[j].second.second+1]--;
				break;
		}
	}
	arr=sorted;
}



vector<pair<int,pair<int,int>>> radixSort(vector<pair<int,pair<int,int>>>& arr){
	int c=2;
	while(c)
	{	sortByIndex(arr,c);
		c--;
	}
	return arr;
}

/* 
Steps followed:
1- Construct SuffixArray (SA) -->O(nlogn)
2- Find LCP of 2 consecutive suffixes (i and i+1) as per the SA, using Kasai Algo --> O(n) for kasai and O(nlogn) for SuffixArray
The algorithm constructs LCP array from suffix array and input text in O(n) time.
3- Build segment tree of LCP --> preprocessing time is O(n) and time to for range minimum query is O(Logn). 
The extra space required is O(n) to store the segment tree.
4- Range Min query : i to i+k-2 (where i=0 to n-k).  
Observe that lcp(x, y) = minimum { lcp(x, x + 1), lcp(x + 1, x + 2),… lcp(y – 1, y) }. 
So let x=i and y=i+k-2… need to find min between lcp of x,x+1; x+1,x+2 so on; thus using segment tree for the same, 
so as to get each range query in logn time.
5- Find the max out of all the above queries’ result..i.e max of all the lcps obtained above.
6- That's the length of the longest subtring repeated k times
*/


