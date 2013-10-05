/*
	ID: zdzapple
	LANG: C++
	TASK: packrec
*/
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <climits>
#include <queue>
#include <deque>
 
using namespace std;
 
struct Rect{
	int x;
	int y;
 
	bool operator<(const Rect&b) const{ return x < b.x;}
	void rotate(){ swap(x, y);}
 
}r[4], Record[100];
 
int X = 0, Y = 0, n = 1, minArea = INT_MAX;
 
void record(){
	if (X * Y <= minArea){
		if (X * Y < minArea){
			n = 0, minArea = X * Y;
		}
		else if (X * Y == minArea){
			for (int i = 0 ; i < n ; i++)
				if (X == Record[i].x || X == Record[i].y)
					return;
		}
		if (X < Y)
			Record[n].x = X, Record[n].y = Y;
		else
			Record[n].x = Y, Record[n].y = X;
		n++;
	}
}
 
void check(Rect r[]){
	//case 1
	X = r[0].x + r[1].x + r[2].x + r[3].x;
	Y = max(r[0].y, max(r[1].y, max(r[2].y, r[3].y)));
	record();
	//case 2
	X = max(r[0].x + r[1].x + r[2].x, r[3].x);
	Y = max(r[0].y, max(r[1].y, r[2].y)) + r[3].y;
	record();
	//case 3
	X = max(r[0].x + r[1].x, r[2].x) + r[3].x;
	Y = max(r[3].y, r[2].y + max(r[0].y, r[1].y));
	record();
	//case 4,5
	X = r[0].x + max(r[1].x, r[2].x) + r[3].x;
	Y = max(r[0].y, max(r[1].y + r[2].y, r[3].y));
	record();
	//case 6
	if (r[3].y >= r[1].y){
		if (r[0].x <= r[1].x)
			if (!(r[0].y + r[1].y > r[3].y && r[0].x + r[2].x > r[1].x + r[3].x)){
				X = max(r[2].x, r[1].x + r[3].x);
				Y = max(r[0].y + r[1].y, r[2].y + r[3].y);
				record();
			}
	}
}
 
void makeCombination(queue<Rect>& Q, int depth = 0){
	if (depth == 4){
		check(r);
		return;
	}
	for (int i = depth ; i < 4 ; i++){
		r[depth] = Q.front();
		Q.pop();
		makeCombination(Q, depth + 1);
		r[depth].rotate();
		makeCombination(Q, depth + 1);
		r[depth].rotate();
		Q.push(r[depth]);
	}
}
 
 
int main() {
    ofstream fout ("packrec.out");
    ifstream fin ("packrec.in");
 
    queue<Rect> Q;
    for (int i = 0 ; i < 4 ; i++){
    	int x, y;
    	fin >> x >> y;
    	Q.push((Rect){x, y});
    }
 
    makeCombination(Q);
    sort(Record, Record + n);
 
    fout << minArea << endl;
    for (int i = 0 ; i < n ; i++)
    	fout << Record[i].x << ' ' << Record[i].y << endl;
 
    return 0;
}

/**
Packing Rectangles
Russ Cox 
This program is straightforward, but a bit long due to the geometry involved. 

There are 24 permutations of the 4 rectangles, and for each permutation, 16 different ways to orient them. We generate all such orientations of permutations, and put the blocks together in each of the 6 different ways, recording the smallest rectangles we find. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct Rect Rect;
struct Rect {
    int wid;
    int ht;
};

Rect
rotate(Rect r)
{
    Rect nr;

    nr.wid = r.ht;
    nr.ht = r.wid;
    return nr;
}

int
max(int a, int b)
{
    return a > b ? a : b;
}

int
min(int a, int b)
{
    return a < b ? a : b;
}

int tot;
int bestarea;
int bestht[101];

void
record(Rect r)
{
    int i;

    if(r.wid*r.ht < tot)
        *(long*)0=0;

    if(r.wid*r.ht < bestarea || bestarea == 0) {
        bestarea = r.wid*r.ht;
        for(i=0; i<=100; i++)
            bestht[i] = 0;
    }
    if(r.wid*r.ht == bestarea)
        bestht[min(r.wid, r.ht)] = 1;
}

void
check(Rect *r)
{
    Rect big;
    int i;

    /* schema 1: all lined up next to each other 
    big.wid = 0;
    big.ht = 0;
    for(i=0; i<4; i++) {
        big.wid += r[i].wid;
        big.ht = max(big.ht, r[i].ht);
    }
    record(big);

    /* schema 2: first three lined up, fourth on bottom 
    big.wid = 0;
    big.ht = 0;
    for(i=0; i<3; i++) {
        big.wid += r[i].wid;
        big.ht = max(big.ht, r[i].ht);
    }
    big.ht += r[3].ht;
    big.wid = max(big.wid, r[3].wid);
    record(big);

    /* schema 3: first two lined up, third under them, fourth to side 
    big.wid = r[0].wid + r[1].wid;
    big.ht = max(r[0].ht, r[1].ht);
    big.ht += r[2].ht;
    big.wid = max(big.wid, r[2].wid);
    big.wid += r[3].wid;
    big.ht = max(big.ht, r[3].ht);
    record(big);

    /* schema 4, 5: first two rectangles lined up, next two stacked 
    big.wid = r[0].wid + r[1].wid;
    big.ht = max(r[0].ht, r[1].ht);
    big.wid += max(r[2].wid, r[3].wid);
    big.ht = max(big.ht, r[2].ht+r[3].ht);
    record(big);

    /*
     * schema 6: first two pressed next to each other, next two on top, like: 
     * 2 3
     * 0 1
     
    big.ht = max(r[0].ht+r[2].ht, r[1].ht+r[3].ht);
    big.wid = r[0].wid + r[1].wid;

    /* do 2 and 1 touch? 
    if(r[0].ht < r[1].ht)
        big.wid = max(big.wid, r[2].wid+r[1].wid);
    /* do 2 and 3 touch? 
    if(r[0].ht+r[2].ht > r[1].ht)
        big.wid = max(big.wid, r[2].wid+r[3].wid);
    /* do 0 and 3 touch? 
    if(r[1].ht < r[0].ht)
        big.wid = max(big.wid, r[0].wid+r[3].wid);

    /* maybe 2 or 3 sits by itself 
    big.wid = max(big.wid, r[2].wid);
    big.wid = max(big.wid, r[3].wid);
    record(big);    
}

void
checkrotate(Rect *r, int n)
{
    if(n == 4) {
        check(r);
        return;
    }

    checkrotate(r, n+1);
    r[n] = rotate(r[n]);
    checkrotate(r, n+1);
    r[n] = rotate(r[n]);
}

void
checkpermute(Rect *r, int n)
{
    Rect t;
    int i;

    if(n == 4)
        checkrotate(r, 0);

    for(i=n; i<4; i++) {
        t = r[n], r[n] = r[i], r[i] = t;    /* swap r[i], r[n] 
        checkpermute(r, n+1);
        t = r[n], r[n] = r[i], r[i] = t;    /* swap r[i], r[n] 
    }
}

void
main(void)
{
    FILE *fin, *fout;
    Rect r[4];
    int i;

    fin = fopen("packrec.in", "r");
    fout = fopen("packrec.out", "w");
    assert(fin != NULL && fout != NULL);

    for(i=0; i<4; i++)
        fscanf(fin, "%d %d", &r[i].wid, &r[i].ht);

    tot=(r[0].wid*r[0].ht+r[1].wid*r[1].ht+r[2].wid*r[2].ht+r[3].wid*r[3].ht);

    checkpermute(r, 0);
    fprintf(fout, "%d\n", bestarea);
    for(i=0; i<=100; i++)
        if(bestht[i])
            fprintf(fout, "%d %d\n", i, bestarea/i);
    exit(0);
}



**/