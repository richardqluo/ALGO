package com.bbg.test;

import java.util.ArrayList;
//placing n queens on an n×n chess board such that they can not attack each other (on same row/col or diagonal)
public class NQueen {
    public ArrayList<ArrayList<Integer>> solveNQueens(int n) {
        ArrayList<ArrayList<Integer>> rst = new ArrayList<ArrayList<Integer>>();
        if (n <= 0) 
          return rst;
        ArrayList<Integer> cols = new ArrayList<Integer>(); 
        Search(rst, cols, n);

        return rst;
      }

      public boolean IsValid(ArrayList<Integer> cols, int rowNum) {
        int colNum = cols.size(); //the next col and rowNum is where new queen  
        for (int col = 0; col < colNum; col++) {
          if (cols.get(col) == rowNum) //if any existing queen at the same row
            return false;
          //if any existing queen at diagonal |c2-c1|==|r2-r1|
          //oldQ col - newQ col == oldQ row - newQ row
          if (Math.abs(col - colNum) == Math.abs(cols.get(col) - rowNum))
            return false;
        }
        return true;
      }
       
      public void Search(ArrayList<ArrayList<Integer>> rst, ArrayList<Integer> cols, int n){
        if (cols.size() == n) {//when the valid row position in last col is found 
          rst.add(cols);
          return;
        }
        //1D array store queen position on chess board because only 1 queen per row/col: index=col and value=row
        //generate all comb, fxp {1 3 0 2} means queens at [1,0] [3,1] [0,2] [2,3] 
        for (int row = 0; row < n; row++) {
          if (IsValid(cols, row)) {//validate existing queens in previous cols against current row position 
            cols.add(row);
            Search(rst, cols, n);
            cols.remove(cols.size() - 1);
          }
        }
      }
}
