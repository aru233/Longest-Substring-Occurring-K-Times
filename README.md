# Longest-Substring-Occurring-K-Times

Aim: Given an integer K, find the longest substring that appears in the text at least K times in O(nlogn), using Suffix Array.

Approach to the solution:
1. Construct SuffixArray (SA) -->O(nlogn)

2.Find LCP(Longest Common Prefix) of every 2 consecutive suffixes (i and i+1) as per the SA built in previous step.
Kasai Algo used for LCP construction--> O(n) for Kasai and O(nlogn) for SuffixArray
The algorithm constructs LCP array from suffix array and input text in O(n) time.

3. Build segment tree of LCP --> preprocessing time is O(n) and time to for range minimum query is O(Logn). 
The extra space required is O(n) to store the segment tree.

4. Range Min query : i to i+k-2 (where i=0 to n-k).  
Observe that lcp(x, y) = minimum { lcp(x, x + 1), lcp(x + 1, x + 2),… lcp(y – 1, y) }. 
So let x=i and y=i+k-2; We need to find the minimum between lcp of x,x+1; x+1,x+2 so on; thus using segment tree for it so 
as to get each range query in logn time.

5. Find the max out of all the above queries’ result..i.e max of all the lcps obtained above.

6. That's the length of the longest subtring repeated k times
