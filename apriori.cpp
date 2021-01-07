#include<bits/stdc++.h>
#include <chrono> 

using namespace std;
using namespace std::chrono;
typedef long long unsigned llu;
typedef long long ll;
typedef pair<ll, ll> ii;
typedef vector<ll> vl;

//use as vector<ii> v;
#define ff(a,b) for(ll i=a; i<b;i++)
#define ff2(a,b) for(ll j=a; j<b;j++)
#define MAX INT_MAX	
#define trace(x) cerr<<#x<<": "<<x<<" "<<'\n';
#define pb push_back
#define mp make_pair

/* Forming strings for hashing in C2 */
string formstr(int a, int b){
    string ret;
    ret+=to_string(a);
    ret+=' ';
    ret+=to_string(b);
    return ret;
}

/* Forming strings for hashing beyond C2 */
string formstr(vector<int> a, int skip){
    string ret;
    int n=a.size();
    ff(0,n){
    if(i!=skip){
        ret+=to_string(a[i]);
        ret+=' ';
    }
    }
    // prune extra space at the end
    string r2="";
    if(ret[ret.length()-1]==' '){
        ff(0,ret.length()-1){
            r2+=ret[i];
        }
    }
    return r2;
}

vector<int> formset(string a){
    vector<int>temp;
    stringstream ss(a);
    string word;
    while(ss>>word){
        temp.pb(stoi(word));
    }

    return temp;
}

vector<vector<int>> hashit(vector<vector<int>> C, int minsup, unordered_map<string,int> &hash, vector<vector<int>> txn){
    vector<vector<int>>ret;
    set<vector<int>>unique;
    hash.clear();

    // cerr<<"ci is fine\n";
    // ff(0,C.size()){
    //     ff2(0,C[i].size()){
    //         cout<<C[i][j]<<' ';
    //     }
    //     cout<<'\n';
    // }
    // cerr<<"ci\n";

    ff(0,C.size()){
        ff2(0,txn.size()){
        string temp;
        int ptr=0;
            for(int k=0;k<txn[j].size();k++){
                if(txn[j][k]==C[i][ptr]){
                    ptr++;
                }
                if(ptr==C[i].size()){
                     unique.insert(C[i]);
                    temp=formstr(C[i],-1);
                    // if(C[i].size()==3)
                    //     trace(temp);
                    if(hash.find(temp)==hash.end()){
                        hash.insert(mp(temp,1));
                    } else {
                        hash[temp]++;
                    }
                break;
                }
            }
        }   
    }
    vector<string>to_prune;
    for(auto i:hash){
        if (i.second<minsup){
            to_prune.pb(i.first);
        }
    }

    // trace(hash.size());
    ff(0,to_prune.size()){
        hash.erase(to_prune[i]);
    }
    trace(hash.size());

    for(auto ix:unique){
        ret.pb(ix);
    }

    // for(auto ix:hash){
    //     cerr<<ix.first<<':'<<ix.second<<'\n';
    // }
    return ret;
}


vector<vector<int>> aprioriGen(vector<vector<int>> F, int itr, unordered_map<string,int> hash){
    vector<vector<int>>ret;
    set<vector<int>>unique;
    
    ff(0,F.size()){
        for(int j=i+1;j<F.size();j++){
            vector<int> utd(F[i].size()*4);
            vector<int>::iterator it;
            sort(F[i].begin(),F[i].end());
            sort(F[j].begin(),F[j].end());
            it = set_union(F[i].begin(),F[i].end(),F[j].begin(),F[j].end(),utd.begin());
            // cerr<<"works";
            utd.resize(it-utd.begin());
            if(utd.size() == itr+1){

                bool flag=1;
                sort(utd.begin(),utd.end());

                for(int k=0;k<utd.size();k++){
                    string test = formstr(utd, k);
                    if(!hash[test]){
                        // cerr<<'['<<test<<']'<<'\n';
                        flag=0;
                    }
                }
                if(flag)
                    unique.insert(utd);
            }
        }
    }

    for(auto ix:unique){
        ret.pb(ix);
    }

    return ret;
}

int main(int argc, char *argv[]){

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    if(argc<3){
        cout<<"please enter arguments properly as <minsup> <input_datafile> without the <>\n";
        return 0;
    }
    /* Minimum support value  */
    int minsup = stoi(argv[1]); //30

    // Build transactions

    unordered_map<int,int>rep;
    rep.clear();
    string str,word;
    ifstream file(argv[2]);
    vector<vector<int>> txn;

    while(getline(file, str, '\n')){
        // cout<<str;
        // cout<<'\n';
        stringstream ss(str);
        set<int>s1;
        while(ss >> word){
            if(word == "-1"||word == "-2"){
                continue;
            }
            s1.insert(stoi(word));
        }
        vector<int>v1;
        for(auto i:s1){
            v1.pb(i);
        }
        txn.pb(v1);
    }

    // TESTING the Transaction VECTOR

    // ff(0,txn.size()){
    //     for(int j=0;j<txn[i].size();j++){
    //         cout<<txn[i][j]<<' ';
    //     }
    //     cout<<'\n';
    // }


    // Starting time measurement
    auto start = high_resolution_clock::now();


    // Hashing for C1,C2
    unordered_map<int,int>c1;
    unordered_map<string,int>c2;
    // for subsequent maps
    unordered_map<string,int> odd,even;

    ff(0,txn.size()){
        for(int j=0;j<txn[i].size();j++){
            if(c1.find(txn[i][j])==c1.end()){
                c1.insert(mp(txn[i][j],1));
            } else {
                c1[txn[i][j]]++;
            }
        }
    }


    // Transaction reduction code - pruning C1
    vector<int>to_prune1;
    for(auto i:c1){
        if (i.second<minsup){
            to_prune1.pb(i.first);
        }
    }

    trace(c1.size());
    ff(0,to_prune1.size()){
        c1.erase(to_prune1[i]);
    }
    trace(c1.size());

    // Generating C2 --correct
    ff(0,txn.size()){
        string temp;
        for(int j=0;j<txn[i].size();j++){
            for(int k=j+1;k<txn[i].size();k++){
                if(c1[txn[i][j]]&&c1[txn[i][k]]){
                    temp=formstr(txn[i][j],txn[i][k]);
                    // trace(temp);
                    if(c2.find(temp)==c2.end()){
                        c2.insert(mp(temp,1));
                    } else {
                        c2[temp]++;
                    }
                }
            }
        }
    }
    // Transaction reduction code - pruning C2
    vector<string>to_prune2;
    for(auto i:c2){
        if (i.second<minsup){
            to_prune2.pb(i.first);
        }
    }

    ff(0,to_prune2.size()){
        c2.erase(to_prune2[i]);
    }


    // Running apriori beyond C2
    vector<vector<int>>C,F;
    for(auto i:c2){
        F.pb(formset(i.first));
    }
    trace(F.size());
    // iteration
    int itr=2;

    ofstream fout;
    fout.open("frequent_apriori.txt");
    fout<<"Finally, frequent itemsets and their frequencies are:\n";

    C.pb({1});
    while(!C.empty()){

        if(itr!=2){               
            if(itr&1){
                odd.clear();
                F = hashit(C,minsup,odd,txn);
                C.clear();
                C = aprioriGen(F,itr,odd);
                for(auto ix:odd){
                    fout<<ix.first<<':'<<ix.second;
                    fout<<'\n';
                }
            } else {
                even.clear();
                F = hashit(C,minsup,even,txn);
                C.clear();
                C = aprioriGen(F,itr,even);
                for(auto ix:even){
                    fout<<ix.first<<':'<<ix.second;
                    fout<<'\n';
                }
            }
        } else {
            C = aprioriGen(F,itr,c2);
            trace(C.size());
            for(auto ix:c2){
                fout<<ix.first<<':'<<ix.second;
                fout<<'\n';
            }
        }
        // trace(itr);
        // trace(F.size());
        itr++;
    }

    // Stopping time measurement
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    auto durationmin = duration_cast<milliseconds>(stop - start); 

    
    // if(itr&1){
    //     for(auto ix:even){
    //         fout<<ix.first<<':'<<ix.second;
    //         fout<<'\n';
    //     }
    // } else {
    //     for(auto ix:odd){
    //         fout<<ix.first<<':'<<ix.second;
    //         fout<<'\n';
    //     }
    // }

    cout <<"Time taken in seconds:"<< duration.count() << endl;
    cout <<"Time taken in millis:"<< durationmin.count() << endl;  
    fout <<"Time taken in seconds:"<< duration.count() << endl;
    fout <<"Time taken in millis:"<< durationmin.count() << endl;  
    fout.close();

    return 0;
}