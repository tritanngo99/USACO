/* Use the slash-star style comments or the system won't see your
   identification information */
/*
ID: tritann1
TASK: sprime
LANG: C++                 
*/
/* LANG can be C++11 or C++14 for those more recent releases */
#include<bits/stdc++.h>
using namespace std;
bool isprime(long n){
  long i ;
  if(n==2) return true;
  if(n%2==0) return false;
  for(i=3;i*i<=n;i+=2){
    if(n%i==0) return false;
  }
  return true;
}
vector<int> solve(int N){
   int i,j;
   if(N==1){
     vector<int> r;
     r.push_back(2);
     r.push_back(3);
     r.push_back(5);
     r.push_back(7);
     return r;
   }
   vector<int> pre = solve(N-1);
   vector<int> now;
   for(i=0;i<pre.size();i++){
     for(j=1;j<=9;j+=2){
        int num = pre[i]*10+j;
        if(isprime(num)) now.push_back(num);
     }
   }
   return now;
}
int main(){
  freopen("sprime.in","r",stdin);
  freopen("sprime.out","w",stdout);
  int N,i,j;
  cin>>N;
  vector<int> result = solve(N);
  for(i=0; i<result.size();i++){
    cout<<result[i]<<'\n';
  }
  return 0;
}

