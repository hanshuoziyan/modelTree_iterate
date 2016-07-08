#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <utility>
using namespace std;

typedef struct treeNode{
   int isLeaf;//0-->leaf, 1-->other
   int property;//0-->degree, 1-->data
   int split;
   int left = -1;
   int right = -1;
   vector<double> coefficient;
   struct treeNode* tleft;
   struct treeNode* tright;
}treeNode;

void inMem(char* fileName, vector<string>& mem){
   std::ifstream fs;
   fs.open(fileName);
   char str[50];
   while(fs.getline(str, 50 )){
      string s;
      s = str;
      mem.push_back(s);
   }
}

void splitStr(string& str, int& isLeaf, int& property, int& split, int& left, int& right, vector<double>& coefficient){
  stringstream ss(str);
  double tmp;
  ss>>isLeaf;
  if(!isLeaf){
   //leaf node
     while(ss>>tmp){
        coefficient.push_back(tmp);
     }
  }else{
      ss>>property;
      ss>>split;
      ss>>left;
      ss>>right;
  }
}

treeNode* createTree(vector<string>& mem,int pos){
   if(mem.size() <= pos || pos == -1)
      return NULL;
   treeNode* tmpNode = new treeNode();
   splitStr(mem[pos], tmpNode->isLeaf, tmpNode->property, tmpNode->split, tmpNode->left, tmpNode->right, tmpNode->coefficient);
   tmpNode->tleft = createTree(mem, tmpNode->left-1);
   tmpNode->tright = createTree(mem, tmpNode->right-1);
   return tmpNode;

}

void deleteTree(treeNode* root){
   if(root == NULL)
      return;
   if(!root->isLeaf){
      delete root;
      return;
   }
   deleteTree(root->tleft);
   root->tleft = NULL;
   deleteTree(root->tright);
   root->tright = NULL;
   delete root;
   root = NULL;
}

void printTree(treeNode* root){
   if(root == NULL){
      cout<<"NULL"<<endl;
      return;
   }
   cout<<root->isLeaf<<" ";
   if(root->isLeaf){
      cout<<root->property<<" "<<root->split<<std::endl;
   }else{
      for(int i = 0; i < root->coefficient.size(); i++){
         cout<<root->coefficient[i]<<" ";
      }
      cout<<std::endl;
   }
   cout<<"left tree: ";
   printTree(root->tleft);
   cout<<"right tree: ";
   printTree(root->tright);
}

vector<double> getLeaf(int degree, int data, treeNode* root){
   vector<double> result;
   if(root == NULL)
      return result;
   int isLeaf = root->isLeaf;
   if(!isLeaf){
      return root->coefficient;
   }else{
      if((root->property == 0 && degree > root->split) || (root->property == 1 && data > root->split))
         return getLeaf(degree, data, root->tleft);
      else
         return getLeaf(degree, data, root->tright);
   }
}

void get_predictResult(char* fileName, treeNode* root){
   vector<string> mem;
   inMem(fileName, mem);
   int x1,x2;
   double real_y,predict_y;
   for(int i = 0; i < mem.size(); i++){
      stringstream ss(mem[i]);
      ss>>x1;
      ss>>x2;
      ss>>real_y;
      vector<double> result;
      result = getLeaf(x1, x2, root);
      predict_y = result[0] + result[1]*x1 + result[2]*x2;
      cout<<x1<<"\t"<<x2<<"\t"<<real_y<<"\t"<<predict_y<<endl;
   }

}

int main(int argc, char *argv[]){
   vector<string> mem;
   inMem(argv[1],mem);
   treeNode* root = NULL;
   if(mem.size() > 0){
      root = createTree(mem, 0);
      /*vector<double> result;
      result = getLeaf(16, 100000, root);
      for(auto i : result){
         cout<<i<<endl;
      }*/
      get_predictResult(argv[2], root);
      deleteTree(root);
     // printTree(root);
   }
   return 0;
}
