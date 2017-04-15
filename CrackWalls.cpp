#include "CrackWalls.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

CrackWalls::CrackWalls()
{
}


CrackWalls::~CrackWalls()
{
	delete[] interPoses;
}

void CrackWalls::findWays(int n)
{
	char lst[20];
	getWays(n, lst, 0);
}

//find all possible bricks layout at given width n Fibonacci f(m)=f(m-3)+f(m-2), for example 333 is one of layout for width 9
void CrackWalls::getWays(int n, char *lst, int stp)
{
	if (n == 0) {
		*lst = '\0';
		ways.push_back(lst - stp);
		return;
	}
	if (n >= 2) {
		*lst = '2';
		getWays(n - 2, lst + 1, stp + 1);
	}
	if (n >= 3) {
		*lst = '3';
		getWays(n - 3, lst + 1, stp + 1);
	}
}

long long CrackWalls::findPoses(int layers)
{
	for (unsigned i = 0; i<ways.size(); i++)
	{
		string way = ways[i];
		int prevPos = 0;
		int curPos = 0;
		vector<int> pose;
		//pose stores the position of inner seam, for example bricks layout 2322 has seam 257
		for (unsigned j = 0; j < way.length() - 1; j++)
		{
			char temp = way.at(j);
			curPos = temp - '0' + prevPos;
			pose.push_back(curPos);
			prevPos = curPos;
		}
		poses.insert({ i, pose });
	}

	//matrix interPoses set false if two layouts have any same seam position 
	for (auto it = poses.begin(); it != poses.end(); ++it)
	{
		int curIdx = it->first;
		vector<int> curPose = it->second;
		vector<int> safePose;
		while (++curIdx < poses.size())
		{
			vector<int> nextPose = poses.at(curIdx);
			unsigned i = 0, j = 0;

			while (i < curPose.size() && j < nextPose.size())
			{
				//compare elements of vectors both in ascend order
				int curSeam = curPose.at(i);
				int nextSeam = nextPose.at(j);
				if (curSeam == nextSeam)
				{
					interPoses[it->first][curIdx] = false;
					interPoses[curIdx][it->first] = false;
					break;
				}
				else if (curSeam > nextSeam)
				{
					j++;
				}
				else
				{
					i++;
				}
			}
		}
	}

	long long sum = 0;
	for (unsigned i = 0; i < poses.size(); i++)
	{
		sum = sum + getLayers(layers, i);
		printf("%d : %lld\n", i, sum);
	}

	return sum;
}

//recusively find all possible ways of crackless wall at a given layers
long long  CrackWalls::getLayers(int n, int pos)
{	
	if (n == 1)
	{
		return 1;
	}
	//cache to reuse previous result
	string cur = to_string(n) + to_string(pos);
	unordered_map<string, long long>::const_iterator got = cache.find(cur);
	if (got != cache.end())
	{
		return got->second;
	}
	else
	{
		long long result = 0;
		for (unsigned i = 0; i < poses.size(); i++)
		{
			if (pos != i && interPoses[pos][i])
			{
				//use a safe layout as the next layer 
				result = result + getLayers(n - 1, i);
			}
		}
		cache.insert({ cur, result });
		return result;
	}
}

int main()
{
	CrackWalls cw;
	cw.findWays(32);
	cout << cw.findPoses(10);//(9,3)=8 (18,5)=7958 
	cin.ignore(1);
	return 0;
}