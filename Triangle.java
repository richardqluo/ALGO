package com.liqnet.test;
import java.io.*;
import java.util.*;

public class Triangle {
    private static List<List<Integer>> triangle = new ArrayList<List<Integer>>();

	public static void main(String[] args) {
		try {
			loadData();
		} catch (IOException e) {
			e.printStackTrace();
		}
        int res = maxSum(triangle);
        System.out.println(res);
	}
	//dynamic programming to obtain the maximum value from bottom to top
	//http://stackoverflow.com/questions/8002252/euler-project-18-approach/8002423#8002423
	//http://codegolf.stackexchange.com/questions/6634/triangle-puzzle
	static int maxSum(List<List<Integer>> values)	{
		int curVal,nextVal;
		for(int row = values.size()-2; row >= 0; row--){//start from the second last row
			List<Integer> curRow = values.get(row);
			for(int col = 0; col < curRow.size(); col++){//set val in current row based on next row
				 curVal = curRow.get(col)+values.get(row+1).get(col);
				 nextVal = curRow.get(col)+values.get(row+1).get(col+1);
	             curRow.set(col,curVal > nextVal ? curVal : nextVal);//pick the bigger sum with 2 neighbors from next row 
			}
		}

		return values.get(0).get(0);
	}

	static void loadData() throws IOException{
		BufferedReader in = new BufferedReader(new FileReader("C:\\Depo\\Data\\triangle_test_100rows.txt"));
		try {
			String line;
			while ((line = in.readLine()) != null) {
				List<Integer> lines = new ArrayList<Integer>();
				for (String item : line.split(" "))	{
					lines.add(Integer.parseInt(item));
				}
				triangle.add(lines);
			}
		} 
	    finally {
	    	in.close();
	    }
	}
	//1
	//11
	//121
	//1331
	//14641
	void pascalTriangle() {//O(n^2)
		int[][] a = new int[10][10];
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j <= i; j++) {
				if(j==0||j==i){
					a[i][j] =1;//first and last of each row =1
				}
				else{
					a[i][j] = a[i - 1][j - 1] + a[i - 1][j];
				}
			}
		}
	}
	//return value on given row/col 
	public int pascal(int i, int j) {
		if (j==0 || i == j) {
			return 1;
		} 
		else if(j == 1 || j == i-1) {//avoid recursion on item next to sides, 4 of 14641
			return i;
		}
		else {
			return pascal(i - 1, j - 1) + pascal(i - 1, j);
		}
	}
}
