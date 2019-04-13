//
//  main.cpp
//  Hicode
//
//  Created by HY J on 2019/3/6.
//  Copyright © 2019 HY J. All rights reserved.
//
#include "main.h"


int print_map(vector< vector<int> > diskmap){
    for(int i = 0 ; i < diskmap.size() ; ++i){
        for(int j= 0 ; j < diskmap[i].size() ; ++j){
            cout<<diskmap[i][j]<<"\t";
        }
        cout<<endl;
    }
    cout<<endl;
    return 0;
}

int diskclean(vector< vector<int> > & diskmap){
    for(int i = 0 ; i < diskmap.size() ; ++i){
        for(int j = 0 ; j < diskmap[0].size() ; ++j){
            diskmap[i][j]=0;
        }
    }
    diskmap[diskmap.size()-1][diskmap[0].size()-1]=-2;
    diskmap[diskmap.size()-1][diskmap[0].size()-2]=-2;
    return 0;
}

int index2map(vector<vector<int>> & diskmap, bool* index){
    int a = diskmap.size();
    int b = diskmap[0].size();
    
    for(int i = 0 ; i < a ; ++i){
        for(int j = 0 ; j < b ; ++j){
            if(i == a-1 && j >= b-2){
                continue;
            }
            diskmap[i][j]=index[i * b + j];
        }
    }
    return 0;
}

int disk_break_RS(vector< vector<int> > &RSmap, bool* index, int & comb_num){
    int length;
    length = RSmap.size() * RSmap[0].size();
    
    if (!hasDone(index, length, DISK_BREAK_NUM))
    {
        //从左到右扫描数组
        for (int i = 0; i < length - 1; i++)
        {
            //找到第一个“10”组合将其变成"01"组合
            if (index[i] && !index[i + 1])
            {
                index[i] = false;
                index[i + 1] = true;
                
                //将"01"组合左边的1移到最左边
                int count = 0;
                for (int j = 0; j < i; j++)
                {
                    if (index[j])
                    {
                        index[j] = false;
                        index[count++] = true;
                    }
                }
                index2map(RSmap, index);
                //print_map(RSmap);
                comb_num ++;
                break;
            }
        }
        return 0;
    }
    else return -1;
}

int disk_break2(vector< vector<int> > &diskmap, bool* index, int & comb_num){
    int length;
    length = diskmap.size() * diskmap[0].size() - 2;
    
    if (!hasDone(index, length, DISK_BREAK_NUM))
    {
        //从左到右扫描数组
        for (int i = 0; i < length - 1; i++)
        {
            //找到第一个“10”组合将其变成"01"组合
            if (index[i] && !index[i + 1])
            {
                index[i] = false;
                index[i + 1] = true;
                
                //将"01"组合左边的1移到最左边
                int count = 0;
                for (int j = 0; j < i; j++)
                {
                    if (index[j])
                    {
                        index[j] = false;
                        index[count++] = true;
                    }
                }
                index2map(diskmap, index);
                //print_map(diskmap);
                comb_num ++;
                break;
            }
        }
        return 0;
    }
    else return -1;
}

int disk_break(vector< vector<int> > &diskmap, int k){
    for (int i=0; i < diskmap.size()-1 ; i++){
        for (int j=0; j<diskmap[0].size(); j++) {
            srand( (unsigned)time( NULL ) + i*j);//srand()函数产生一个以当前时间开始的随机种子
            if (RATE > rand()%MAX) {
                diskmap[i][j] = 1;
            }
        }
    }
    for (int i=0; i < k ; i++){
        if (RATE > rand()%MAX) {
            diskmap[diskmap.size()-1][i] = 1;
        }
    }
    return 0;
}

//行校验计算 如果可恢复，则返回0; 如果不可恢复，返回1; 其他情况返回-1;
int row_recover(vector<int>& row, int m){
    int broken=0;
    for (int i=0; i<row.size(); i++) {
        if (row[i] == -1) {
            return -1;
        }
        if (row[i] == 1) {
            broken ++;
        }
    }
    if (broken <= m) {
        for (int i=0; i<row.size(); i++) {
            row[i] = 0;
        }
        return 0;
    }
    else{
        return 1;
    }
    return -1;
}

//列校验计算 如有错误返回-1; 如有不可恢复的盘，返回1；正常恢复返回0；
int col_recover(vector< vector<int> > &diskmap, int k){
    int broken=0;
    int bad_col=0;
    for (int i=0; i<k ; i++) {
        for (int j=0; j < diskmap.size(); j++) {
            if (diskmap[j][i] == -1) {
                cout<<"error";
                return -1;
            }
            if (diskmap[j][i] == 1) {
                broken ++;
            }
        }
        if(broken <= 1){//列损失小于1，整列完全恢复。此处可调整
            for (int j=0; j < diskmap.size(); j++) {
                diskmap[j][i] = 0;
            }
        }
        else {
            bad_col++;
        }
        broken = 0;
    }
    if (bad_col > 0) {
        return 1;
    }
    return 0;
}

int break_and_recover_RS(vector< vector<int> > RSmap, int k, int m, int l){
    int failed_num =0;
    int length;
    length = RSmap.size() * RSmap[0].size();
    
    int comb_num = 1;
    bool *index = new bool[length]();
    //选中前k个位置
    for (int i = 0; i < DISK_BREAK_NUM; i++)
    {
        index[i] = true;
    }
    index2map(RSmap, index);
    print_map(RSmap);
    
    while(disk_break_RS(RSmap, index, comb_num) == 0){

        vector< vector<int>> originmap(RSmap);
        
        int result = 0;
        //row recover
        for (int i=0; i<RSmap.size(); i++) {
            result += row_recover(RSmap[i], m);
        }
        
        if (result >= 1) {
            /*
             cout<<"recover failed! origin map:"<<endl;
             print_map(originmap);
             cout<<"recover result"<< endl;
             print_map(RSmap);
             */
            failed_num ++;
        }
    }
    cout<<comb_num<<endl;
    cout<<failed_num<<endl;
    double failed_rate = 100 * (double)failed_num/ (double)comb_num;
    cout<<"failed rate is: "<< failed_rate<<'%'<<endl<<endl;
    return failed_num;
}

int break_and_recover2(vector< vector<int> > diskmap, int k, int m, int l){
    int failed_num =0;
    int length;
    length = diskmap.size() * diskmap[0].size() - 2;
    
    int comb_num = 1;
    bool *index = new bool[length]();
    //选中前k个位置
    for (int i = 0; i < DISK_BREAK_NUM; i++)
    {
        index[i] = true;
    }
    index2map(diskmap, index);
    print_map(diskmap);
    
    while(disk_break2(diskmap, index, comb_num) == 0){
        //col recover
        vector< vector<int>> originmap(diskmap);

        col_recover(diskmap, k);
        //row recover
        for (int i=0; i<diskmap.size(); i++) {
            row_recover(diskmap[i], m);
        }
        //col recover again
        int result = 0;
        result = col_recover(diskmap, k);
        
        if (result == 1) {
            /*
            cout<<"recover failed! origin map:"<<endl;
            print_map(originmap);
            cout<<"recover result"<< endl;
            print_map(diskmap);
             */
            failed_num ++;
        }
    }
    cout<<comb_num<<endl;
    cout<<failed_num<<endl;
    double failed_rate = 100 * (double)failed_num/ (double)comb_num;
    cout<<"failed rate is: "<< failed_rate<<'%'<<endl<<endl;
    return failed_num;
}

int break_and_recover(vector< vector<int> > diskmap, int k, int m, int l){
    //break disks!
    disk_break(diskmap, k);
    vector< vector<int>> originmap(diskmap);
    
    //col recover
    col_recover(diskmap, k);
    //row recover
    for (int i=0; i<=l; i++) {
        row_recover(diskmap[i], m);
    }
    //col recover again
    int result = 0;
    result = col_recover(diskmap, k);
    
    if (result == 1) {
        cout<<"recover failed! origin map:"<<endl;
        print_map(originmap);
        cout<<"recover result"<< endl;
        print_map(diskmap);
    }
    return result;
}


//输出结果
void printRes(bool* index, int n)
{
    for (int i=0;i<n;i++)
    {
        cout << index[i] << " ";
    }
    cout << endl;
}

//检查最后k个位置是否已全变成0
bool hasDone(bool* index, int n, int k)
{
    for (int i=n-1;i>=n-k;i--)
    {
        if (!index[i])
        {
            return false;
        }
    }
    return true;
}

int Comb(int n, int k)
{
    int comb_num = 1;
    bool *index = new bool[n]();
    //选中前k个位置
    for (int i = 0; i < k; i++)
    {
        index[i] = true;
    }
    printRes(index, n);
    
    while (!hasDone(index, n, k))
    {
        //从左到右扫描数组
        for (int i = 0; i < n - 1; i++)
        {
            //找到第一个“10”组合将其变成"01"组合
            if (index[i] && !index[i + 1])
            {
                index[i] = false;
                index[i + 1] = true;
                
                //将"01"组合左边的1移到最左边
                int count = 0;
                for (int j = 0; j < i; j++)
                {
                    if (index[j])
                    {
                        index[j] = false;
                        index[count++] = true;
                    }
                }
                printRes(index, n);
                comb_num ++;
                break;
            }
        }
    }
    delete[] index;
    return comb_num;
}

/*磁盘存储形式
     k           m
     -------------  -------
     |  |  |  |  |  |  |  |
     -------------  -------
l    |  |  |  |  |  |  |  |
     -------------  -------
     |  |  |  |  |  |  |  |
     -------------  -------
 
     -------------
 1   |  |  |  |  |
     -------------
 0表示数据正常，1表示数据损坏，-1表示无此盘。
 */

int main(int argc, const char * argv[]) {
    int k,l,m;
    //cout<<"input k l m"<<endl;
    //cin>>k>>l>>m;
    k=4;
    l=3;
    m=2;
    cout<<"k="<<k<<"\tl="<<l<<"\tm="<<m<<endl;
    cout<<"Disk number is: "<<((l+1)*k+m*l)<<endl;
    cout<<"Disk break number is: " << DISK_BREAK_NUM<<endl;
    vector< vector<int> > diskmap(l+1);
    vector< vector<int> > RSmap(l);

    for(int i = 0 ; i < diskmap.size() ; ++i){
        for(int j= 0 ; j < k+m ; ++j){
            diskmap[i].push_back(0);
        }
    }
    for(int i = 0 ; i < RSmap.size() ; ++i){
        for(int j= 0 ; j < k+m ; ++j){
            RSmap[i].push_back(0);
        }
    }
    
    diskmap[l][k]=-1;
    diskmap[l][k+1]=-1;

    int APresult = 0, RSresult =0;
    
    APresult = break_and_recover2(diskmap, k, m, l);
    RSresult = break_and_recover_RS(RSmap, k, m, l);
    
    //disk_break2(diskmap);
    return APresult;
}
