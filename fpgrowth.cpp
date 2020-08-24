#include<bits/stdc++.h>
using namespace std;

#define trace(x) cerr<<#x<<": "<<x<<" "<<'\n';

#define ITEM_NAMES 26


// We need the order, hence the ordered map
map <char,int> fqMap;
vector<pair<char,int>> fqVec;
queue<char> trans;
// Class for recording a transaction
class Txn{

    public:
    int TID;
    vector <char> items;

    Txn(int, vector<char>);
};

Txn::Txn(int ID,vector<char> items): TID(ID),items(items){}

// FP Tree class
class FPT{

    public:
    char itemName;
    int count=0;
    FPT* link[ITEM_NAMES];

    FPT(char);
};

FPT::FPT(char ch):itemName(ch) {
    count = 1;
    for(int i=0;i<ITEM_NAMES;i++){
        link[i]=nullptr;
    }
}

// Header Table
unordered_map<char,FPT*>hdr,curr;

void insert_tree(queue<char> &trans, FPT* node){
    
    if(trans.empty()){
        return;
    }
    bool flag=0;

    for(int u=0;u<ITEM_NAMES;u++){
        // cerr<<"loop";
        if(node->link[u]&&node->link[u]->itemName == trans.front()){
            // trace(trans.front());
            node->link[u]->count++;
            
            trace(node->link[u]->itemName);
            trace(node->link[u]->count);
        
            trans.pop();
            insert_tree(trans,node->link[u]);
            flag=1;
            break;
        }
    }
    // else create node
    if(!flag){

        FPT* nn = new FPT(trans.front());
        // trace(trans.front());
        trans.pop();
        node->link[(int)(nn->itemName-'a')]=nn;
        
        trace(nn->itemName);
        trace(nn->count);
        if(hdr.find(nn->itemName)==hdr.end()){
            hdr.insert(make_pair(nn->itemName,nn));
            curr.insert(make_pair(nn->itemName,nn));
        }
        else {
            curr[nn->itemName]->link[(int)(nn->itemName-'a')] =  nn;
            curr[nn->itemName] = nn;
        }
        insert_tree(trans,node->link[(int)(nn->itemName-'a')]);
    
    }

    return;
}

bool comparator(const pair<int,int> &a, const pair<int,int> &b) 
{ 
    return (a.second > b.second); 
} 

// THE
// MAIN
// FUNCTION

int main(){
    //  minimum support value
    
    int minsup = 3;

    // Build transactions
    
    vector<Txn> txn;
    vector<vector<char>> vv={{'f','a','c','d','g','i','m','p'},
    {'a','b','c','f','l','m','o'},
    {'b','f','h','j','o'},{'b','c','k','s','p'},
    {'a','f','c','e','l','p','m','n'}};

    for(int i=0;i<5;i++){
        Txn T(100*(i+1),vv[i]);
        txn.push_back(T);
    }
    // Testing entries
        trace(txn[2].TID);
        trace(txn[2].items[0]);

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

    map<char,int>::iterator itr=fqMap.begin();
    for(itr;itr!=fqMap.end();itr++){
        if(itr->second >= minsup){
            fqVec.push_back(make_pair(itr->first,itr->second));
        }
    }
    sort(fqVec.begin(),fqVec.end(),comparator);

    // for(int i=0;i<fqVec.size();i++){
    //     cerr<<fqVec[i].first<<' '<<fqVec[i].second<<'\n';
    // }
    

    // make tree root
    
    FPT* root = new FPT('0');
    trace(root->itemName);

    // build tree

    // For now we are ignoring the possibility of duplicates
    // being present in a transaction.
    set<char> tempSet;

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
        cerr<<"\n\n";
        insert_tree(trans,root);
    }
    tempSet.clear();

    return 0;
}