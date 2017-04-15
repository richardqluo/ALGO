package com.bbg.test;

import java.util.*;

public class Graph {

	public static void main(String[] args) {
//		GraphNode n1 = new GraphNode(1); 
//		GraphNode n2 = new GraphNode(2); 
//		GraphNode n3 = new GraphNode(3); 
// 
//		n1.neighbors = new GraphNode[]{n2,n3};
// 
//		breathFirstSearch(n1, 3);
		
		int[][] preq = {{0, 2},{1, 0},{1, 2}};
		int[] ret = findOrder(3,preq);
		return;
	}
	public static void breathFirstSearch(GraphNode root, int x){
		if(root.val == x)
			System.out.println("find in root");
 
		GraphQueue queue = new GraphQueue();
		root.visited = true;
		queue.enqueue(root);
 
		while(queue.first != null){//till all nodes within graph is visited
			GraphNode c = (GraphNode) queue.dequeue();
			for(GraphNode n: c.neighbors){ 
				if(!n.visited){
					n.visited = true;
					if(n.val == x)
						System.out.println("Find "+n);
					queue.enqueue(n);
				}
			}
		}
	}
	//Topological Sorting only for Directed Acyclic Graph (DAG: no cycle)
	//if directed A -> B, A must before B in the order list
	//multiple roots and more than one possible order 
	//similar given a list sorted word contain certain letters, to find alphabetical order of those letters 
	//{"caa", "aaa", "aab"} > c a b
	//compare each adjacent words find the first different letter, prevletter.neighbor.add(nextletter)
    public ArrayList<GraphNode> topSort(ArrayList<GraphNode> graph) {

        if (graph == null || graph.size() == 0) {
            return graph;
        }
        ArrayList<GraphNode> rst = new ArrayList<GraphNode>();
        HashMap<Integer, Integer> map = new HashMap<Integer, Integer>();
        for (GraphNode node : graph) {
            for (GraphNode neighbor : node.neighbors) {
                int key = neighbor.val;//not use up stream node.val
                if (map.containsKey(key)) {
                    map.put(key, map.get(key) + 1);//how many time it is down stream from other nodes
                } else {
                    map.put(key, 1);
                }
            }
        }
        Queue<GraphNode> queue = new LinkedList<GraphNode>();
        for (GraphNode node : graph) {
            if (!map.containsKey(node.val)) {//root is not down stream from any others
                queue.offer(node);
                rst.add(node);
            }
        }
        while (!queue.isEmpty()) {
            GraphNode node = queue.poll();  
            for (GraphNode n : node.neighbors) {
                int key = n.val;
                map.put(key, map.get(key) - 1);
                if (map.get(key) == 0) {//BFS: till all its up stream nodes are accounted before it
                    rst.add(n);
                    queue.offer(n);
                }
            }
        }
        return rst;
    }
	//BFS find possible order to complete courses at given prerequisites
	//similar multiple people each has a agenda consist sequential tasks, merge agendas into one without breaking sequential order
	//{a c d} {b c e}={a b c d e} or {b a c e d}  not {a e c b d}
	public static int[] findOrder(int numCourses, int[][] prerequisites) {
	    if(prerequisites == null){
	        throw new IllegalArgumentException("illegal prerequisites array");
	    }
	 
	    int len = prerequisites.length;
	 
	    //if there is no prerequisites, return a sequence of courses
	    if(len == 0){
	        int[] res = new int[numCourses];
	        for(int m=0; m<numCourses; m++){
	            res[m]=m;
	        }
	        return res;
	    }
	 
	    //records the number of prerequisites each course (0,...,numCourses-1) requires
	    //n rows 2 cols matrix (0,2) (1,0) (1,2) 0 requires 2 as prerequisites, 1 requires 0 2 as prerequisites, 
	    //use array pCounter instead of HashMap as next example, because course number is 0 based int can be represented by array index 
	    //prerequisites[1][0]=prerequisites[2][0]=1 pCounter[1]=2 prerequisites for 1
	    int[] pCounter = new int[numCourses];
	    for(int i=0; i<len; i++){
	        pCounter[prerequisites[i][0]]++;
	    }
	 
	    //stores courses that have no prerequisites
	    LinkedList<Integer> queue = new LinkedList<Integer>();
	    for(int i=0; i<numCourses; i++){
	        if(pCounter[i]==0){
	            queue.add(i);
	        }
	    }
	 
	    int numNoPre = queue.size();
	 
	    //initialize result 
	    int[] result = new int[numCourses];
	    int j=0;
	    //at least 1 course has no prerequisites (root), other wise circular dependent
	    while(!queue.isEmpty()){
	        int c = queue.remove();
	        result[j++]=c;
	 
	        for(int i=0; i<len; i++){
	        	//go thru root's sub courses, decrement the sub course preq count till 0, then add sub as root to queue
	            if(prerequisites[i][1]==c){//graph node c's down stream nodes 
	                pCounter[prerequisites[i][0]]--;
	                if(pCounter[prerequisites[i][0]]==0){
	                	//queue course whose preq are all in result already 
	                    queue.add(prerequisites[i][0]);
	                    numNoPre++;
	                }
	            }
	        }
	    }
	 
	    //numNoPre !=numCourses : circular dependency in which case involved courses will never be queued because pCounter never 0
	    if(numNoPre==numCourses){
	        return result;
	    }else{
	        return new int[0];
	    }
	}
}

class GraphNode{ 
	int val;
	GraphNode next;//populate when enqueue
	GraphNode[] neighbors;//only include downstream nodes for Directed Graph 
	boolean visited;
	GraphNode(int x) {
		val = x;
	}
	GraphNode(int x, GraphNode[] n){
		val = x;
		neighbors = n;
	}
}

class GraphQueue{
	GraphNode first, last;
 
	public void enqueue(GraphNode n){
		if(first == null){
			first = n;
			last = first;
		}else{
			last.next = n;
			last = n;
		}
	}
 
	public GraphNode dequeue(){
		if(first == null){
			return null;
		}else{
			GraphNode temp = new GraphNode(first.val, first.neighbors);
			first = first.next;
			return temp;
		}	
	}
}

//directed graph can be represented by list of Vertex(node) and Edge(distance), Vertex.adjacencies has only downstream vertex not upstream 
class Vertex implements Comparable<Vertex>
{
    public final String name;
    public Edge[] adjacencies;//Vertex A|B|C=new Vertex("A|B|C"); A.adjacencies = new Edge[]{ new Edge(B, 8),new Edge(C, 10) };
    //Initially the distance of each node to the source (specified in computePaths) is set to a very high value.
    public double minDistance = Double.POSITIVE_INFINITY;//
    public Vertex previous;
    public boolean isVisited;//only for Cyclic graph
    public Vertex(String argName) { name = argName; }
    public String toString() { return name; }
    public int compareTo(Vertex other)
    {	return Double.compare(minDistance, other.minDistance);    }
}

class Edge
{
    public final Vertex target;
    public final double weight;
    public Edge(Vertex argTarget, double argWeight)
    {	target = argTarget; weight = argWeight; }
}
//all nodes into Unsettled and settled set, A node is moved to the settled set if a shortest path from the source to it has been found.
class Dijkstra {//ref C++ findPaths
	public static void computePaths(Vertex source) {/*bool computePaths(Vertex source, Vertex dest, double max)*/
		source.minDistance = 0.;
		PriorityQueue<Vertex> vertexQueue = new PriorityQueue<Vertex>();//ordered by minDistance with the least value at the head
		vertexQueue.add(source);//add only start node initially 
		//or store a class{vertex; minDistance;} in PriorityQueue to avoid keeping state in node under concurrent  

		while (!vertexQueue.isEmpty()) {//BFS: till all is evaluated
			Vertex u = vertexQueue.poll();//get the head (least) first to minimize the update minDistance with less value on the same node
			//impossible that node in next layer will ahead of all nodes in previous layer, because it must be after one of them 
			/*if (vertexQueue.isEmpty() && u.minDistance>max)//pivot node (no other path from its upper nodes to lower nodes)  
					return false;*/
			for (Edge e : u.adjacencies) {
				Vertex v = e.target;
				double distanceThroughU = u.minDistance + e.weight;//source to target via u
				/*if(v==dest && distanceThroughU<max) //wrong if distanceThroughU>max return false; because cur distanceThroughU may not be least 
					return true;*/
				if (distanceThroughU < v.minDistance) {//find the adjacent node has the lowest distance, or use hashmap<String,Double> to store without keep minDistance in node    
					vertexQueue.remove(v);//remove if v already in queue, reset minDistance and reorder it back into queue

					v.minDistance = distanceThroughU; 
					v.previous = u;
					vertexQueue.add(v);
				}
			}
		}
	}

    public static List<Vertex> getShortestPathTo(Vertex target)
    {
        List<Vertex> path = new ArrayList<Vertex>();
        for (Vertex vertex = target; vertex != null; vertex = vertex.previous)
            path.add(vertex);//all vertex have been populated by computePaths()

        Collections.reverse(path);
        return path;
    }
    //detect cyclic :DFS, ref C++
	public boolean isCyclic(Vertex v) {//or use (Vertex v, HashSet<String> hs) to store visited to avoid keeping state in node under concurrent 
		if (v.isVisited) {//if (hs.contains(v.name);
			return true;
		}
		v.isVisited = true;//hs.add(v.name);
		for (Edge e : v.adjacencies) {
			Vertex t = e.target;
			if (t.isVisited)//if (hs.contains(v.name);
				return true;
			if (isCyclic(t))//isCyclic(t,hs)
				return true;
		}
		v.isVisited = false;//hs.remove(v.name);
		return false;
	}
}

