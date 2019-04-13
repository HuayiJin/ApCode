//
//  Header.h
//  Hicode
//
//  Created by HY J on 2019/3/6.
//  Copyright © 2019 HY J. All rights reserved.
//
#include <vector>
#include <iostream>
#include <time.h>
#include <iomanip>


#ifndef Header_h
#define Header_h
#define MAX 100
#define RATE 10 //损坏率(%)
#define TIMES 0
#define DISK_BREAK_NUM 6


using namespace std;
//This project is for Hi-code

/*磁盘存储形式
            k          m
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

int print_map(vector< vector<int> >);
int Comb(int n, int k);
void printRes(bool* index, int n);
bool hasDone(bool* index, int n, int k);
int index2map(vector<vector<int>> & diskmap, bool* index);
#endif /* Header_h */
