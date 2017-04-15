package com.citi.test;

import java.util.*;

//Basic Test Cases: No Exit, No backtracking needed, On edge, Not on edge
public class LabyrinthSolverImpl implements LabyrinthSolver {
	//backtracking visited locations
	private ArrayList<Location> visited = new ArrayList<Location>(); 
	//backtracking previous moves
	private Stack<Integer> moves = new Stack<Integer>();
	@Override
	public boolean exitExists(Labyrinth l) {
		//coordinates relative to the start point for backtracking visited locations
		int offsetX=0;
		int offsetY=0;
		return traverse(l, offsetX, offsetY);
	}

	private boolean traverse(Labyrinth l, int offsetX, int offsetY) {
		int prevX = offsetX;
		int prevY = offsetY;
		if (l.isOutside()){
			return true;
		}
		else {
			boolean[] around = l.wallsAround();
			for (int i=0; i<=3; i++){
				if (prevX != offsetX || prevY != offsetY){
					//revert to previous recursion 
					offsetX = prevX;
					offsetY = prevY;
					int prevM = moves.pop();
					int revertM = 0;
					switch (prevM) {
					case 0:
						revertM=2; break;
					case 1:
						revertM=3; break;
					case 2:
						revertM=0; break;
					case 3:
						revertM=1; break;
					default:
						throw new RuntimeException("Wrong direction: " + i);
					}
					l.tryMove(revertM);
				}
				int x = offsetX;
				int y = offsetY;
				if (!around[i]){
					switch (i) {
					case 0:
						y=offsetY-1; break;
					case 1:
						x=offsetX+1; break;
					case 2:
						y=offsetY+1; break;
					case 3:
						x=offsetX-1; break;
					default:
						throw new RuntimeException("Wrong direction: " + i);
					}
					//if visited return false
					Location loc = new Location(x,y);
					if (visited.contains(loc)){
						//reset x y if not move
						y=offsetY;
						x=offsetX;
						continue;
					}
					else{
						if(l.tryMove(i)){
							moves.push(i);
							visited.add(loc);
							//reset offsetX offsetY if move
							offsetY = y;
							offsetX = x;
							//recursive call
							if(traverse(l,offsetX, offsetY)){
								return true;
							}
							else{
								continue;
							}
								
						}						
					}
				}
			}
			return false;
		}
	}
	
	class Location {
		int x;
		int y;
		Location(int X, int Y) {
			this.x = X;
			this.y = Y;
		}
		
		@Override
		public boolean  equals (Object object) {
	    if (!(object instanceof Location))
	        return false;
	      if (object == this)
	        return true;
	      return  this.x == ((Location) object).x && this.y == ((Location) object).y;
		}
	}
	
	public static void main(String[] args) {
		LabyrinthSolverImpl labyrinthSolverImpl = new LabyrinthSolverImpl();
		Labyrinth l = new LabyrinthImpl();
		System.out.println(labyrinthSolverImpl.exitExists(l));
	}

}
