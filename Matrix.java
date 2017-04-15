package com.hbk.test;

import java.awt.Point;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Queue;

import com.bbg.test.ArrStr;

public class Matrix {
	private static int [] [] grid;

	public static void main(String[] args) {
		/*
		ArrayList<Integer> list = new ArrayList<Integer>(); 
		ArrayList<ArrayList<Integer>> path = new ArrayList<ArrayList<Integer>>();
		grid = new int[][]{	{0, 0, 1, 0, 0, 1, 0},
				            {1, 0, 1, 1, 0, 0, 0},
				            {0, 0, 0, 0, 1, 0, 1},
				            {1, 0, 1, 0, 0, 0, 0},
				            {1, 0, 1, 1, 0, 1, 0},
				            {1, 0, 0, 0, 0, 1, 0},				            
				            {1, 1, 1, 1, 0, 0, 0}};
		int count =traverse(0,0,0, list,path);
		System.out.println(count);
		*/
		
		//int ret = backPackVal(10, new int[]{2,3,5,7}, new int[]{1, 5, 2, 4});
		//int ret = numDecodings("1234");
		//int ret = minEditDistance("mart","karma");
		
		int[] arr = {0,1, 4, 7, 9,15, 17,18,20}; 
		int max = maxProfit(arr); 
		System.out.println(max);

		return;
	}	
	//no need way list path if just count
	//similar find min cost, each grid element store cost{up,down,left,right} to neighbors, MAX_VALUE for border 
	//int traverse (int row, int col, int cost)
	//if out of boundary or visited (grid[i][j]==null), return MAX_VALUE; if reach dest, return cost
	//result = min(traverse(row+|-,col+|-1,cost+grid[row][col].up|down|left|right)
	
	//similar sliding puzzle, init grid one empty slot at most rightdown can swap with up|right|left slot, result grid to find path
	//traverse from result back to init 3 ways, use way to swap empty slot with target it moving to till reach rightdown, 
	//compare grid with given init, if same return path otherwise continue to find next path 
	public static int traverse (int row, int col, int way,  ArrayList<Integer> list, ArrayList<ArrayList<Integer>> path) {
		if ((row < 0) || (row >= grid.length) || (col < 0) || (col >= grid[0].length)){
			return 0;
		} else {
			int pos = grid[row][col];
			if (pos  == 1 || pos  == -1 ) {
				// block or visited
				return 0;
			} else if (row == grid.length-1 && col == grid[0].length-1) {
				//reach the dest, count it as a one
				ArrayList<Integer> temp = new ArrayList<Integer>(list);
				temp.add(way);//last step
				path.add(temp);
				return 1;
			} else {
				grid[row][col] = -1; // record this position so don't revisit.
				list.add(way);
				// Count all possible path with one less digit starting
				int result=0;
				result = traverse(row,col+1,1,list,path)//right
						 + traverse(row+1,col,2,list,path)//down
						 + traverse(row,col-1,3,list,path)//left
					 	 + traverse(row-1,col,4,list,path);//up
				grid[row][col] = pos; // Remove record from position.
				list.remove(list.size()-1);
				return result; 
			}
		}
	}
	//m x n grid filled with non-negative numbers, path (down or right) from top left to bottom right which minimizes the sum of all numbers along its path
	//similar find number of possible unique paths 
	//similar probability of passing given (x,y) if corner 1 side 2 mid 4 /total, because 1|2|4 way to pass it  
	static int minPathSum(int[][] grid) {//O(MN)time
	    if(grid == null || grid.length==0)
	        return 0;
	 
	    int m = grid.length;
	    int n = grid[0].length;
	    
	    int[][] dp = new int[m][n];
	    dp[0][0] = grid[0][0];    
	 
	    // initialize top row with sum of path
	    for(int i=1; i<n; i++){
	        dp[0][i] = dp[0][i-1] + grid[0][i];
	        //if(grid[0][i]!=1) 1 as block
	        //dp[0][i] = 1;//find number of possible unique paths: only move right without direction change as one path 
	    }
	 
	    // initialize left column with sum of path
	    for(int j=1; j<m; j++){
	        dp[j][0] = dp[j-1][0] + grid[j][0];
	        //if(grid[j][0]!=1) 1 as block
	        //dp[j][0] = 1;//only move down without direction change as one path
	    }
	 
	    //dp min path value to each pos : dp(1,1)=min(dp(0,1),dp(1,0)) + grid(1,1)  
	    for(int i=1; i<m; i++){
	        for(int j=1; j<n; j++){
	        	dp[i][j] = Math.min(dp[i][j-1],dp[i-1][j]) + grid[i][j];
	        	//if(grid[i][j]!=1) 1 as block dp[i][j] stay 0
	            //dp[i][j] = dp[i-1][j] + dp[i][j-1];  dp[j] += dp[j - 1];//1D
	        }
	    }
	 
	    return dp[m-1][n-1]; //return dp[n-1];
	}
	//depth-first search: reiterate of same subpath can be avoided by Dynamic Programming O(mn)
	//[if grid[i][j] is char, given a dictionary (hashset), find longest valid word by path down or right]
	//[?load dict words into trie, start from from [m,n] go thru every [i,j] to [0,0], move up or left because dfs reverse 
	static int dfs(int i, int j){//[string dfs(int i, int j)]
		//if(grid[i][j]==1)	return 0; 1 as block
	    if(i==grid.length-1 && j==grid[0].length-1){
	        return grid[i][j];
	        //return 1; //find possible unique paths
	    }
	 
	    if(i<grid.length-1 && j<grid[0].length-1){
	        int r1 = grid[i][j] + dfs(i+1, j);//[string r1 = dfs(i-1, j)+grid[i][j];
	        int r2 = grid[i][j] + dfs(i, j+1);//[string r2 = dfs(i, j-1)+grid[i][j];
	        return Math.min(r1,r2);//if trie.prefix(r1|r2) return string r=longer(r1,r2)
	        //return dfs(i+1,j) + dfs(i,j+1);
	    }
	    //j=grid[0].length-1 right col 
	    if(i<grid.length-1){
	        return grid[i][j] + dfs(i+1, j);
	        //return dfs(i+1,j);
	    }
	    //i=grid.length-1 bottom row
	    if(j<grid[0].length-1){
	        return grid[i][j] + dfs(i, j+1);
	        //return dfs(i,j+1);
	    }	 
	    return 0;//out of boundary
	}
	//determine the knight's minimum initial health to reach the princess with at least 1 health point
	//-2 (K)-3	 3
	//-5	-10	 1
	//10	 30	-5 (P)
	static int calculateMinimumHP(int[][] dungeon) {
		int m = dungeon.length;
		int n = dungeon[0].length;
	 
		int[][] h = new int[m][n];
		//dungeon[m - 1][n - 1]<0 ?  1 - dungeon[m - 1][n - 1] : 1, min HP required at P
		//dungeon -5 then need hp 6 to get 1 left, 5 then only need 1 to continue
		h[m - 1][n - 1] = Math.max(1 - dungeon[m - 1][n - 1], 1);
	 
		//init last row, can only move right
		for (int i = m - 2; i >= 0; i--) {//reverse set based on h[m - 1][n - 1]
			h[i][n - 1] = Math.max(h[i + 1][n - 1] - dungeon[i][n - 1], 1);
		}
	 
		//init last column, can only move down
		for (int j = n - 2; j >= 0; j--) {
			h[m - 1][j] = Math.max(h[m - 1][j + 1] - dungeon[m - 1][j], 1);
		}
	    //h[i][j] is the minimum health value before he enters (i,j)
		for (int i = m - 2; i >= 0; i--) {
			for (int j = n - 2; j >= 0; j--) {
				int down = Math.max(h[i + 1][j] - dungeon[i][j], 1);
				int right = Math.max(h[i][j + 1] - dungeon[i][j], 1);
				h[i][j] = Math.min(right, down);
			}
		}
	 
		return h[0][0];
	}
	//'1's (land) and '0's (water), count the number of islands
	//0 empty seat,1 healthy kid, 2 sick kid, healthy kid sit next to sick kid will be affected, find num of final healthy kids  
	static int numIslands() {//O(m*n)
	    if (grid==null || grid.length==0 || grid[0].length==0) return 0;
	    int count = 0;
	 
	    for (int i=0; i<grid.length; i++) {
	        for (int j=0; j<grid[0].length; j++) {
	            if(grid[i][j] == '1'){//if(grid[i][j] != '0') diff land 1, 2, 3...
	                count++;
	                merge(i, j);//set all adjacent land to water so each land only count 1, merge(i, j, grid[i][j]); 
	            }
	        }
	    }
	    return count;
	}
	//each side expose to water or other land count as 1 for perimeter, 1 perimeter 4, 11 perimeter 6
	static void merge(int i, int j){//[int] merge(int i, int j, land[, perimeter])
		//boundary check can be spared by adding 0 around the matrix
	    if(i<0 || j<0 || i>grid.length-1 || j>grid[0].length-1)
	        return; //[return 1; increment perimeter by 1 if reach the end]
	 
	    //if current cell is water or visited
	    if(grid[i][j] == '0')// || grid[i][j]!=land) only merge if same kind land
	    	return; //[return 1;]
	    //set visited cell to '0'	    
	    grid[i][j] = '0'; //[return 0; if same land or revisit need not increment perimeter]
	 
	    merge(i-1, j);//merge(i-1, j,land);
	    merge(i+1, j);
	    merge(i, j-1);
	    merge(i, j+1);
	    //[return merge(i-1, j)+merge(i+1, j)+merge(i, j-1)+merge(i, j+1);]
	}
	//000 
	//010 
	//001
	//merge all 1 at border and its adjacent 1 to #, only isolated 1 will remain, convert them to 0 
	//000 
	//000 
	//001
	public void solve() {
	    int m = grid.length;
	    int n = grid[0].length;	 	    
	    for(int i=0;i<m;i++){//merge 1 to # on left & right boarder
	        if(grid[i][0] == '1')
	        	bfs(i, 0);	 
	        if(grid[i][n-1] == '1')
	        	bfs(i,n-1);
	    }	 	    
	    for(int j=0; j<n; j++){//merge 1 to # on top & bottom boarder
	         if(grid[0][j] == '1')
	        	 bfs(0,j);
	        if(grid[m-1][j] == '1')
	        	bfs(m-1,j);
	    }	 	    
	    for(int i=0;i<m;i++){//process the board, 1 to 0 and revert #
	        for(int j=0; j<n; j++){
	            if(grid[i][j] == '1'){
	            	grid[i][j] = '0';
	            }else if(grid[i][j] == '#'){
	            	grid[i][j] = '1';
	            }
	        }
	    }
	}
	private Queue<Integer> queue = new LinkedList<Integer>();
	private void bfs(int i, int j) {//BFS avoid overflow caused by DFS merge 
		int n = grid[0].length;
		fillCell(i, j); 
		while (!queue.isEmpty()) {
			int cur = queue.poll();
			int x = cur / n;
			int y = cur % n; 
			fillCell(x - 1, y);
			fillCell(x + 1, y);
			fillCell(x, y - 1);
			fillCell(x, y + 1);
		}
	} 
	private void fillCell(int i, int j) {
		int m = grid.length;
		int n = grid[0].length;
		if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] != '1')
			return; 
		// add current cell is queue & then process its neighbors in bfs
		queue.offer(i * n + j);
		grid[i][j] = '#';
	}
	//find the largest square containing all 1's
    public int maxSquare(int[][] matrix) {
    	if (matrix == null || matrix.length == 0 || matrix[0].length == 0) {
    		return 0;
    	}
    	int n = matrix.length;
    	int m = matrix[0].length;
    	int[][] dp = new int[n][m];
    	int maxLen = 0;
    	//Init right and bottom
    	for (int i = 0; i < m; i++) {
    		dp[n - 1][i] = matrix[n - 1][i];
    		maxLen = Math.max(maxLen, dp[n - 1][i]);
    	}
    	for (int i = 0; i < n; i++) {
    		dp[i][m - 1] = matrix[i][m - 1];
    		maxLen = Math.max(maxLen, dp[i][m - 1]);
    	}
    	//if start from rigt-bottom: right and bottom dont have prev to increment on, but left and top can be incremented above prev   
    	for (int i = n - 2; i >= 0; i--) {
    		for (int j = m - 2; j >= 0; j--) {
    			if (matrix[i][j] == 1) {
    				//if min=1, all adjacent 3 point in current 2X2 square are 1s, then new length dp[i][j]=2
    				//if min=2, all adjacent 3 2X2 squares are 1s, then new length dp[i][j]=3
    				dp[i][j] = Math.min(Math.min(dp[i][j + 1], dp[i + 1][j]), dp[i + 1][j + 1]) + 1;
    				maxLen = Math.max(maxLen, dp[i][j]);
    			}
    		}
    	}
    	return maxLen * maxLen;
    }
	
	static int maximalRectangle(char[][] matrix) {
		int m = matrix.length;
		int n = m == 0 ? 0 : matrix[0].length;
		int[][] dp = new int[m][n + 1];
		//dp[i][j] represents the number of consecutive 1's (without 0 in between) in column j which end in row i (including).
		int maxArea = 0;
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				if (matrix[i][j] == '0') {
					dp[i][j] = 0;
				} else {
					dp[i][j] = i == 0 ? 1 : dp[i - 1][j] + 1;
				}
			}
		}
		//call "Largest Rectangle in Histogram" on each row level
		for (int i = 0; i < m; i++) {
			int area = ArrStr.largestRectangle(dp[i]);
			if (area > maxArea) {
				maxArea = area;
			}
		}
	 
		return maxArea;
	}

	//Given n items with size A[i], return the max size a backpack of size m can be filled
	//time/space complexity O(mn)
	static int backPack2D(int m, int[] A) {
	        if (A == null || A.length == 0 || m <= 0) {
	            return 0;
	        }
	        //dp[i][j] means if some of items A[0-i] can fulfill size j 
	        boolean[][] dp = new boolean[A.length + 1][m + 1];
	        //for simplicity, add A[0]=0 so j-A[i] instead of j-A[i-1]
	        dp[0][0] = true;//dp[2][0]=dp[1][0]=dp[0][0] (A[2|1]>j=0)
	        //dp[i][j] = either dp[i-1][j-A[i]] (add item i) or dp[i-1][j] (not add item i) is true
	        //j=0<i dp[i][0]=true=dp[i][2-2]=dp[i][4-2]=dp[i][3-3]
	        //  0  1  2  3  4
	        //0[T, F, F, F, F]2 
	        //1[T, F, T, F, F]3(2) 
	        //2[T, F, T, T, F] (3)
	        //dp[1][2]=dp[0][2]||dp[0][2-2]=T
	        //dp[1][3]=dp[0][3]||dp[0][3-2]=F
	        //dp[2][2]=dp[1][2] j=2<A[2]=3
	        //dp[2][3]=dp[1][3]||dp[1][3-3]=T

	        for (int i = 1; i <= A.length; i++) {
	            for (int j = 0; j <= m; j++) {
	                if (j - A[i - 1] >= 0) {
	                    //A[i - 1] represent A[i] due to an extra help row, calculate row i-1 value save in row i 
	                    dp[i][j] = dp[i - 1][j] || dp[i - 1][j - A[i - 1]];
	                } else {// j is smaller than A[i-1], not adding A[i-1]                   
	                    dp[i][j] = dp[i - 1][j];
	                }      
	            }
	        }
	        for (int j = m; j >= 0; j--) {
	            if (dp[A.length][j]) {
	                return j;//Return max j with dp[A.length][j] true.
	            }
	        }
	        return 0;
	    }
	//time complexity O(mn),space complexity O(m)
    static int backPack1D(int m, int[] A) {
        if (A == null || m == 0) {
            return 0;
        }

        boolean[] DP = new boolean[m + 1];
        DP[0] = true;//other init false
        //DP[i][j]= DP[i-1][j-A[i]] | DP[i-1][j], result of row i only depend on previous row i-1
        //and only last DP row[i=A.length] matters, can reuse one DP row so i recalc and override DP result from previous row i-1
        //1[T, F, T, F, F]2 
        //2[T, F, T, T, F]3 DP[3]=DP[3-3]=DP[0] which still has result from previous i 
        for (int i = 1; i <= A.length; i++) {//DP row i only depend on row i-1, so i still from 1 up
            for (int j = m; j >= 0; j--) {//from m to 0 to use lower DP[j] result from previous row
                if (j - A[i - 1] >= 0 && DP[j - A[i - 1]]) {
                	//DP[0]=DP[2]=DP[3]=true
                    DP[j] = true;
                }
                //else: dp[i][j] = dp[i - 1][j]; no need to recalc for i, keep result from i-1 
            }
        }

        for (int j = m; j >= 0; j--) {
            if (DP[j]) {
                return j;
            }
        }
        return 0;
    }
    //Given n items with size Ai and value Vi, and a backpack with size m. What's the maximum value can you put into the backpack
    //A[2, 3, 5, 7] V[1, 5, 2, 4] m=10 return 9
    static int backPackVal(int m, int[] A, int V[]) {
        if (A == null || m == 0) {
            return 0;
        }

        int[] DP = new int[m + 1];
        DP[0] = 0;//save max value instead of T/F

        for (int i = 1; i <= A.length; i++) {
            for (int j = m; j >= 0; j--) {
                if (j - A[i - 1] >= 0 ) {
                    DP[j] = Math.max(DP[j], DP[j-A[i-1]] + V[i-1]);//DP[j] from previous i (not add) or DP[j-A[i-1]]+V[i-1] (add) 
                }
            }
        }
        int max=0;
        for (int j = m; j >= 0; j--) {
            max=Math.max(DP[j], max);
        }
        return max;
    }
	//similar sell stock qty(index) 1, 2, 3, 4, 5, 6, 7, 8 profit 1, 4, 7, 9,15, 17,18,20, max profit sell 8 by any way 3:7+5:15=22
	static int maxProfit(int[] arr){ 
		if(arr == null || arr.length == 0) 
			return 0; 
		int[] dp = new int[arr.length]; 
		for(int i = 1; i < arr.length; i++){ 
			for(int j = 0; j <= i; j++) 
				dp[i] = Math.max(dp[i], dp[i - j] + arr[j]); //btw use arr[j]: [i - j]+[j] or keep existing [i]
		} 
		return dp[arr.length-1]; 
	}
    //Min times of char operation (replace, delete, insert) required to transform one string to another
    //given a word, find closest word from a list 
    static int minEditDistance(String word1, String word2) {
    	int len1 = word1.length();
    	int len2 = word2.length();
     
    	// len1+1, len2+1, because finally return dp[len1][len2]
    	int[][] dp = new int[len1 + 1][len2 + 1];
    	//each right or down as a insert or delete, rightdown as a replace
    	for (int i = 0; i <= len1; i++) {
    		dp[i][0] = i;//dp[1,0]=1 as insert word1[0], till dp[len,0]=word[len-1] 
    	}
    	for (int j = 0; j <= len2; j++) {
    		dp[0][j] = j;
    	}
     
    	for (int i = 0; i < len1; i++) {
    		char c1 = word1.charAt(i);
    		for (int j = 0; j < len2; j++) {
    			char c2 = word2.charAt(j);
    			//update dp[i + 1][j + 1] value for char at word1[i],word2[j]
    			//Replace:dp[i + 1][j + 1] = word1[i] == word2[j] ? DP[i][j] : DP[i][j] + 1;
    			//Insert: dp[i + 1][j + 1] = dp[i][j+1] + 1; // missing 1 char in word1
    			//Delete: dp[i + 1][j + 1] = dp[i+1][j] + 1; // extra char in word1
    			if (c1 == c2) {
    				//no additional op required above previous
    				dp[i + 1][j + 1] = dp[i][j];
    			} else {
    				int replace = dp[i][j] + 1;
    				//dp[i][j + 1] already has 1 as delete|insert from dp[i][j], +1 as insert|delete as second step, delete and insert equivalent to replace
    				int insert = dp[i][j + 1] + 1;
    				int delete = dp[i + 1][j] + 1;
    				//min(replace,insert,delete)
    				int min = replace > insert ? insert : replace;
    				min = delete > min ? min : delete;
    				dp[i + 1][j + 1] = min;
    			}
    		}
    	}
     
    	return dp[len1][len2];
    }
    //number of distinct subsequences of T= "rabbit" in S = "rabbbit", 3:bb_,b_b,_bb 
    public int numSubsequences(String S, String T) {
    	int[][] dp = new int[S.length() + 1][T.length() + 1];
     
    	for (int i = 0; i < S.length(); i++)
    		dp[i][0] = 1;//when T=="", whatever S will have 1 subsequence: ""
    	for (int j = 1; j < T.length(); j++)
    		dp[0][j] = 0;//when S=="", whatever T will not be subsequence of S == "", but keep dp[0][0]=1
    	for (int i = 1; i <= S.length(); i++) {
    		for (int j = 1; j <= T.length(); j++) {
    			dp[i][j] = dp[i - 1][j];//when T exist in S[a~b], it will surely exist in S[a~b+1], so DP[i][j] = DP[i-1][j] + something
    			if (S.charAt(i - 1) == T.charAt(j - 1)) 
    				//if S[i] and S[i - 1] both equals to T[j - 1], count dp[i - 1][j - 1]'s previous records in
    				dp[i][j] += dp[i - 1][j - 1];//T(ap) S(app) count of 2nd p also need add count of 1st p, otherwise last result as 1 not 2
    		}
    	}
     
    	return dp[S.length()][T.length()];
    }
    //longest common substring 
    //  b a b
    //c　0　0　0
    //a　0　1　0
    //b　1　0　2
    //a　0　2　0
    //if a[i]=b[j] then dp[i,j]=dp[i-1,j-1]+1, return max(dp[i,j]) 
    //use 1D c[] to keep only last dp row and override it
    static void getLCString(char[] a, char[] b) {
        int i, j;
        int len1, len2;
        len1 = a.length;
        len2 = b.length;
        int maxLen = len1 > len2 ? len1 : len2;
        int max = 0;
        int[] maxIndex = new int[maxLen];
        int[] c = new int[maxLen]; 
 
        for (i = 0; i < len2; i++) {
            for (j = len1 - 1; j >= 0; j--) {//from len to 0 to use lower c[j] result from previous row
                if (b[i] == a[j]) {//i and j no need to be in same ascend order based on previous dp 
                    if ((i == 0) || (j == 0))
                        c[j] = 1;//initial c[0], no previous c
                    else
                        c[j] = c[j - 1] + 1;
                } else {
                    c[j] = 0;
                }
 
                if (c[j] > max) { 
                    max = c[j]; 
                    maxIndex[0] = j; 
                    //if a bigger new max, save it at index 0 and clean the rest that equal to previous max 	
                    for (int k = 1; k < maxLen; k++) {
                        maxIndex[k] = 0;
                    }
                } else if (c[j] == max) { //multiple substrings at the same length as current max 
                    for (int k = 1; k < maxLen; k++) {
                        if (maxIndex[k] == 0) {
                            maxIndex[k] = j;//end index of multiple substrings at the same length = c[j] = max[0]
                            break; //append to the last element of maxIndex[]
                        }
                    }
                }
            }
        }
 
        for (j = 0; j < maxLen; j++) {
            if (maxIndex[j] > 0) {
                for (i = maxIndex[j] - max + 1; i <= maxIndex[j]; i++)
                    System.out.print(a[i]);
            }
        }
    }
    //divide b[m] books among k scribes, each book has page b[i] and can be assign to one scriber 
    //scriber start at the same time at same writing speed 1page per minute, min time to finish (k<m, other wise max(b[i]) is the answer)
    //f[i][j] min time for first i books are given to j scribes to finish 
    //f[i][j] = min{max(f[x][j-1], sum(b[x+1 ~ i])), j<x<i} jth scribe start from xth book
    static int minCopyTime(int[] b,int k) {
    	int[][] dp = new int[b.length+1][k+1];
    	for(int j=0;j <k;j++)
    		dp[0][j]=b[0];//1st book given to one of j scribes always take same pages time
    	for(int i=1;i<b.length;i++){
    		dp[i][0]=dp[i-1][0]+b[i];//first i books are given to 1 scribe to finish
    	}
    	for(int i=1;i<=b.length;i++){
    		for(int j=1;j<=k;j++){
    			int min=Integer.MAX_VALUE;
                int max = 0;
                int sumPgs = 0;
                for (int l = j; l <= i; l++)//j-1 =< l < i because each scribe has at lease 1 book
                	sumPgs = sumPgs + b[l];
                for(int m=j-1;m<i;m++){
                	//dp[m][j-1] sum[m+1 ~ i] : loop thru all m that book[j-1~m] assign to j-1 and book[m+1~i] assign to jth scribes
                	max = Math.max(dp[m][j-1],sumPgs);   
                	if (min>max)
                		min=max;
                	sumPgs -= b[m+1];//keep sum [m+1 ~ i] for the next loop
                }
    			dp[i][j]=min;
    		}
    	}
    	return dp[b.length][k];
    }
    //Given n balloons, Each painted with a number[i], 
    //burst balloon i will get nums[left] * nums[i] * nums[right] coins, and the left and right then becomes adjacent
    //dp[i][j]: max coins for burst all balloons between i and j
    static int maxBalloonCoins(int[] iNums) {
        int n = iNums.length;//iNums[0,n-1]
        int[] nums = new int[n + 2];
        for (int i = 0; i < n; i++) nums[i + 1] = iNums[i];//shift 1 index to handle iNums[-1] as nums[0]
        //burst last balloon get iNums[-1]*iNums[i]*iNums[n] coins, add nums[0](iNums[-1]) and nums[n+1](iNums[n]) =1 
        nums[0] = nums[n + 1] = 1;
        int[][] dp = new int[n + 2][n + 2];
        return DP(1, n, nums, dp);//start from 1
    }
    static int DP(int i, int j, int[] nums, int[][] dp) {
        if (dp[i][j] > 0) return dp[i][j];//memo previous result
        for (int x = i; x <= j; x++) {
        	//    nums = [3,1,5,8] --> [3,5,8] -->   [3,8]   -->  [8]  --> []
        	//   coins =  3*1*5      +  3*5*8    +  1*3*8      + 1*8*1   = 167
            dp[i][j] = Math.max(dp[i][j], DP(i, x - 1, nums, dp) + nums[i - 1] * nums[x] * nums[j + 1] + DP(x + 1, j, nums, dp));
        }
        return dp[i][j];
    }
    //each step can merge two adjacent stone piles to a new pile, the sum of stone in the new pile is accumulated as cost
    //if can merge any two piles instead of adjacent, then push all into heap, each time pull 2 min merge and push back
    //similar: minimum cost of concat adjacent strings a+b=ab c+d=cd ab+cd=abcd 2+2+4=8 vs a+b=ab ab+c=abc abc+d=abcd 2+3+4=9 
    //dp[i][j]=min(dp[i][k]+dp[k+1][j]+sum[i,j]) for all k from i to j, dp[i][j] = 0; (i=j)
    //min cost for merge stone between i and j = merge stone between i and k + merge stone between k+1 and j , 
    public int minCostMergeStone(int[] S) {  
        int n = S.length;  
        int[][] dp = new int[n][n];  
        int[] sum = new int[n]; //no need for concat string all length=1, j-i+1 is sum[i,j] 
        for(int i=0; i<n; i++) {  
            sum[i] = (i>0?sum[i-1]:0) + S[i];  //sum up S[0-i]
        }  
        for(int i=0; i<n; i++) {  
            Arrays.fill(dp[i], Integer.MAX_VALUE); //init all dp[i][j] as max value for later min
            dp[i][i] = 0;  
        }  
        for(int w=1; w<n; w++) {//window=1 merge every adjacent piles, then =2 merge every adjacent piles from prev merge           
            for(int i=0; i<n-w; i++) { 
                int j = i+w;          //end=start+window(sliding)  
                int sumij = sum[j] - (i>0?sum[i-1]:0); // S[0-j]-S[0-i], can only merge adjacent  
                for(int k=i; k<j; k++) {//find min dp among all possible k between i,j  
                    dp[i][j] = Math.min(dp[i][j], dp[i][k] + dp[k+1][j] + sumij);//compare with dp[i][j] from prev k  
                }  
            }  
        }  
        return dp[0][n-1];  
    }
    

    
    
    //total number of ways to decode a digits message based on map 'A'=1 'B'=2  ... 'Z'=26
    static int numDecodings(String s) {
        if(s==null||s.length()==0||s.equals("0"))
            return 0;
     
        int[] t = new int[s.length()+1];
        t[0] = 1; //to work with t[1] as below
     
        if(isValid(s.substring(0,1)))
            t[1]=1;//1st digit in (1,9) can surely be decoded to a char 1 0 X : 1 1 2
        else
            t[1]=0;//1st digit =0  0 1 X : 1 0 1
     
        for(int i=2; i<=s.length(); i++){
            if(isValid(s.substring(i-1,i))){
                t[i]+=t[i-1];
            }
            //either previous 1 or previous 2 digits are in (1,26) will increment t[i]
            //1234 t[2]+=t[1] also t[2]+=t[0] 1<23<26, t[2]=2
            if(isValid(s.substring(i-2,i))){
                t[i]+=t[i-2];
            }
        }
     
        return t[s.length()];
    }
    //verify if char 1-26 
    static boolean isValid(String s){
        if(s.charAt(0)=='0')
            return false;//if 0 does not count or 0X already count as X 
        int value = Integer.parseInt(s);
        return value>=1&&value<=26;
    }
    //searches for a value in an m x n sorted matrix:end = m * n - 1; while(start<end){  mid = (end + start)/2;  
    //int num = matrix[mid/n][mid%n];//[row][col]  if (num < target) { start =    mid;}...O(logN+M)   
    public boolean searchMatrix(int[][] matrix, int target) { //O(m + n)
        int m=matrix.length-1;
        int n=matrix[0].length-1;     
        int i=m; //sort on row
        int j=0;     
        while(i>=0 && j<=n){
            if(target < matrix[i][j]){
                i--;//[m][0] at 1st col of row, if smaller must in the prev row
            }else if(target > matrix[i][j]){
                j++;//[m][0] at last row, if bigger must in the same row
            }else{
                return true;
            }
        }     
        return false;
    }
    
    //find largest submatrix (nxn)  DP solution based on Kadane's algorithm - O(n^3) time and O(n^2) space
    //Kandane's algorithm is a way to find a contiguous subsequence with maximum sum
    public static void kadane2D(int[][] a, int n) {
        int[][] s = new int[n + 1][n]; // [ending row][sum from row zero to ending row] (rows 1-indexed!)
        for (int r = 0; r < n + 1; r++) {
            for (int c = 0; c < n; c++) {
                s[r][c] = 0;
            }
        }
        for (int r = 1; r < n + 1; r++) {
            for (int c = 0; c < n; c++) {
                s[r][c] = s[r - 1][c] + a[r - 1][c];
            }
        }
        int maxSum = Integer.MIN_VALUE;
        int maxRowStart = -1;
        int maxColStart = -1;
        int maxRowEnd = -1;
        int maxColEnd = -1;
        for (int r1 = 1; r1 < n + 1; r1++) { // rows 1-indexed!
            for (int r2 = r1; r2 < n + 1; r2++) { // rows 1-indexed!
                int[] s1 = new int[n];
                for (int c = 0; c < n; c++) {
                    s1[c] = s[r2][c] - s[r1 - 1][c];
                }
                int max = 0;
                int c1 = 0;
                for (int c = 0; c < n; c++) {
                    max = s1[c] + max;
                    if (max <= 0) {
                        max = 0;
                        c1 = c + 1;
                    }
                    if (max > maxSum) {
                        maxSum = max;
                        maxRowStart = r1 - 1;
                        maxColStart = c1;
                        maxRowEnd = r2 - 1;
                        maxColEnd = c;
                    }
                }
            }
        }
    }

    //maximum number of points that lie on the same straight line: slopes are equal from the same base points
    public int maxPointsOnLine(Point[] points) {
        if(points == null || points.length == 0) return 0;
     
        HashMap<Double, Integer> result = new HashMap<Double, Integer>();
        int max=0;
     
        for(int i=0; i<points.length; i++){
            int duplicate = 1;
            int vertical = 0;
            for(int j=i+1; j<points.length; j++){
                //go thru rest point off the current base point 
                if(points[i].x == points[j].x){
                    if(points[i].y == points[j].y){
                        duplicate++;
                    }else{
                        vertical++;//case divided by 0
                    }
                }else{//1.0 * cast int to double
                    double slope = points[j].y == points[i].y ? 0.0 : (1.0 * (points[j].y - points[i].y))/ (points[j].x - points[i].x);
                    //0 is horizontal
                    if(result.get(slope) != null){
                        result.put(slope, result.get(slope) + 1);
                    }else{
                        result.put(slope, 2);//at least 2 points each line 
                    }
                }
            }
     
            for(Integer count: result.values()){
                if(count+duplicate > max){//duplicates always included
                    max = count+duplicate;
                }
            }
     
            max = Math.max(vertical + duplicate, max);
            result.clear();//clear before go next i base point 
        }
     
        return max;
    }
}
