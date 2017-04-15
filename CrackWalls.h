using namespace std;
#include <vector>
#include <unordered_map>
class CrackWalls
{
public:
	CrackWalls();
	~CrackWalls();
	void findWays(int);
	long long findPoses(int);
private:
	vector<string>  ways;
	void getWays(int, char *, int);
	unordered_map<int, vector<int>> poses;
	unordered_map<string, long long> cache;
	bool(*interPoses)[3500] = new bool[3500][3500];
	long long getLayers(int, int);
};

