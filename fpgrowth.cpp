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

#define ITEM_NAMES 2500

// vector to store the frequent itemsets
vector<pair<vector<int>,int>> ans;

// We need the order, hence the ordered map
map <int,int> fqMap;
vector<pair<int,int>> fqVec;
queue<int> trans;
// Class for recording a transaction
class Txn{

    public:
    vector <int> items;

    Txn( vector<int>);
};

Txn::Txn(vector<int> items): items(items){}

// FP Tree class
class FPT{

    public:
    int itemName;
    int count=0;
    FPT* parent;
    FPT* link[ITEM_NAMES];

    FPT(int);
};

class HT{
    public: 
    int itemName;
    FPT* target;
    HT* next;

    HT(int);
};

FPT::FPT(int ch):itemName(ch) {
    count = 1;
    parent=nullptr;
    for(int i=0;i<ITEM_NAMES;i++){
        link[i]=nullptr;
    }
}

HT::HT(int cc):itemName(cc) {
    target=nullptr;
    next=nullptr;
}

string formstr(string a, int b){
    string ret;
    ret+=a;
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

vector<int> formset(string a, int item){
    vector<int>temp;
    stringstream ss(a);
    string word;
    while(ss>>word){
        temp.pb(stoi(word));
    }
    temp.pb(item);

    return temp;
}

// Header Table
map<int,pair<HT*,int>>hdr,curr,cc2;

void insert_tree(queue<int> &trans, FPT* node){
    
    if(trans.empty()){
        return;
    }
    bool flag=0;

    for(int u=0;u<ITEM_NAMES;u++){
        if(node->link[u]&&node->link[u]->itemName == trans.front()){
            // trace(trans.front());
            node->link[u]->count++;
            
            // trace(node->link[u]->itemName);
            // trace(node->link[u]->count);
        
            trans.pop();
            insert_tree(trans,node->link[u]);
            flag=1;
            hdr[node->link[u]->itemName].second++;
            break;
        }
    }
    // else create node
    if(!flag){

        FPT* nn = new FPT(trans.front());
        
        nn->parent = node;
        // trace(trans.front());
        trans.pop();
        node->link[(nn->itemName)]=nn;
        
        // trace(nn->itemName);
        // trace(nn->count);
        if(hdr.find(nn->itemName)==hdr.end()){
            HT* hh = new HT(nn->itemName);
            hh->target = nn;
            hdr.insert(make_pair(nn->itemName,mp(hh,nn->count)));
            curr.insert(make_pair(nn->itemName,mp(hh,nn->count)));
        }
        else {
            HT* hh = new HT(nn->itemName);
            hh->target = nn;
            curr[nn->itemName].first->next =  hh;

            curr[nn->itemName] = mp(hh,nn->count);
            hdr[nn->itemName].second++;
            // trace(curr[nn->itemName].first->itemName);
            // trace(hdr[nn->itemName].second);
        }
            
        insert_tree(trans,node->link[(nn->itemName)]);
    
    }
    return;
}


void buildSubtable(int I, map<int,pair<HT*,int>> &ret, map<int,pair<HT*,int>> cm){
    HT* curr = cm[I].first;
    map<int,int>badmap;
    badmap.clear();
   cc2.clear();
   ret.clear();
    while(curr){

        FPT* walk = curr->target->parent;

        while(walk->itemName!=0){

            if(badmap.find(walk->itemName)==badmap.end()){
                walk->count=0;
                badmap.insert(mp(walk->itemName,1));
            }
            if(ret.find(walk->itemName)==ret.end()){
                // walk->count+=curr->target->count;
                HT* hh = new HT(walk->itemName);
                hh->target = walk;

                ret.insert(make_pair(walk->itemName,mp(hh,curr->target->count)));
                cc2.insert(make_pair(walk->itemName,mp(hh,curr->target->count)));
            } 
            else { 
                // H[walk->itemName].first.target.second+=curr->target->count;
                // walk->count+=curr->target->count;

                HT* hh = new HT(walk->itemName);
                hh->target = walk;
                cc2[walk->itemName].first->next =  hh;
                cc2[walk->itemName] = mp(hh,walk->count);
                ret[walk->itemName].second+=curr->target->count;
            }
            // cerr<<"---\n";
            // trace(walk->count);
            walk=walk->parent;
        }

        // cerr<<"_______";
        // trace(curr->itemName);
        curr=curr->next;

    }
}

void mineTree(string pattern, int minsup, map<int,pair<HT*,int>> H){

    // cerr<<"Htable\n";
    // for(auto ix:H){
    //     cout<<ix.first<<':'<<ix.second.second<<'\n';
    // }

    for(auto ix:H){
        if(ix.second.second >= minsup){
            ans.pb(mp(formset(pattern,ix.first),ix.second.second));
            // trace(pattern);
            // trace(ix.first);
            map<int,pair<HT*,int>>tmap;
            tmap.clear();
            buildSubtable(ix.first, tmap, H);
            mineTree(formstr(pattern, ix.first), minsup, tmap);
        }
    }
}


// -------------------------------------------------------------
bool comparator(const pair<int,int> &a, const pair<int,int> &b) 
{ 
    return (a.second > b.second); 
} 

// THE
// MAIN
// FUNCTION

int main(int argc, char *argv[]){

    if(argc<3){
        cout<<"please enter arguments properly as <minsup> <input_datafile> without the <>\n";
        return 0;
    }
    //  minimum support value
    ans.clear();
    int minsup = stoi(argv[1]);

    // Build transactions
    
    vector<Txn> txn;

    string str,word;
    ifstream file(argv[2]);
    vector<vector<int>> vv;

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
        vv.pb(v1);
    }

    ff(0,vv.size()){
        Txn T(vv[i]);
        txn.push_back(T);
    }
    // Testing entries
        // trace(txn[2].items[0]);

    // list frequent items, build header table
    
    for(int i=0;i<txn.size();i++){
        for(int j=0;j<txn[i].items.size();j++){
            if(fqMap.find(txn[i].items[j])==fqMap.end()){
                fqMap.insert(make_pair(txn[i].items[j],1));
            }
            else{
                fqMap[txn[i].items[j]]++;
            }
        }
    }

    map<int,int>::iterator itr=fqMap.begin();
    for(itr;itr!=fqMap.end();itr++){
        if(itr->second >= minsup){
            fqVec.push_back(make_pair(itr->first,itr->second));
        }
    }
    // sort(fqVec.begin(),fqVec.end(),comparator);

    // for(int i=0;i<fqVec.size();i++){
    //     cerr<<fqVec[i].first<<' '<<fqVec[i].second<<'\n';
    // }
    

    // Starting time measurement---------------
    auto start = high_resolution_clock::now();
    
    // make tree root
    
    FPT* root = new FPT(0);
    root->parent = nullptr;

    trace(root->itemName);

    // build tree


    set<int> tempSet;

    for(int i=0;i<txn.size();i++){

        while(!trans.empty()){
            trans.pop();
        }
        tempSet.clear();

        for(int j=0;j<txn[i].items.size();j++){
            tempSet.insert(txn[i].items[j]);
        }
        for(int j=0;j<fqVec.size();j++){
            if(tempSet.find(fqVec[j].first)!=tempSet.end()){
                trans.push(fqVec[j].first);
            }
        }
        // cerr<<"\n\n";
        insert_tree(trans,root);
    }
    tempSet.clear();
    
    // cout<<"Header tree\n";
    // for(auto ix:hdr){
    //     cout<<ix.first<<':'<<ix.second.second<<'\n';
    // }

    
    mineTree("",minsup,hdr);

        // Stopping time measurement
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    auto durationmin = duration_cast<milliseconds>(stop - start); 

    ofstream fout;
    fout.open("frequent_fpgrowth.txt");
    fout<<"Finally, frequent itemsets and their frequencies are:\n";
    ff(0,ans.size()){
        ff2(0,ans[i].first.size()){
            fout<<ans[i].first[j]<<' ';
        }
        fout<<':'<<ans[i].second<<'\n';
    }

    cout <<"Time taken in seconds:"<< duration.count() << endl;
    cout <<"Time taken in millis:"<< durationmin.count() << endl;  
    fout <<"Time taken in seconds:"<< duration.count() << endl;
    fout <<"Time taken in millis:"<< durationmin.count() << endl;  
    fout.close();

    return 0;
}
