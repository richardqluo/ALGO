package com.bbg.test;

import java.util.*;

public class StkLstTree {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

	}
	//think straight part as cycle like loop part, faster and slower meet at the same diagonal spot of head
	//just like they both run in a cycle and will meet at where they began faster moved 2 cycle while slower moved 1 cycle
	//if one start from their meet spot and another start from head (diagonally parallel) with same speed, they meet at beginning of loop
    static boolean hasCycleList(Node head) {
        Node slow = head;
        Node fast = head.next;
        //more steps fast move each time > 2 , more the total  distance the two pointers would have to travel before meet.
        //fxp fast move 3 each time, fast over take slow in this case f(i)s(i+1)->f(i+3)s(i+2) where they will meet if fast move 2
        //if loop length is 2|L fast move 3|L+1 then f(2)s(1) -> f(1)s(2) -> f(2)s(1) will never meet because diff L btw them
        while(fast != null && fast.next != null){
            slow = slow.next;
            fast = fast.next.next;
 
            if(slow.value == fast.value){
                return true;
                
//              int len=0, loop=0;
//              while(slow.value != fast.value)//slow != fast compare address if dup value
//              {//fast stay at meet point while slow go thru whole loop to get length of the loop
//              	slow = slow.next;
//                	loop++;
//              }
                
//              slow = head;
//	            while (slow.next.value != fast.next.value) {//fast at same speed as slow, their.next meet at loop start  
//	            	slow = slow.next;
//	            	fast = fast.next;
//                  len++;
//	            }
//	            return len+loop;//length of whole list
            }
        }
 
        return false;
    }
    
    public Node reverseIterative(Node head) {
        if(head==null || head.next == null) 
            return head;
     
        Node cur = head.next;     
        head.next = null;//head to tail
        
        while(cur!= null){
            Node temp = cur.next;//temp shift to 3
            cur.next = head;//2 next point to 1
            head = cur;//1 shift to 2
            cur = temp;//2 shift to 3
        }
     
        return head;//the last cur will be null 
    }
    
    public Node reverseRecursive(Node head) {
        if(head==null || head.next == null)
            return head;
     
        Node cur = head.next;
        head.next = null;
     
        Node rest = reverseRecursive(cur);
        cur.next = head;//can not before recursion which need original cur state 
     
        return rest;
    }
    //swap every two adjacent nodes  1->2->3->4 as 2->1->4->3.
    public Node swapPairs(Node head) {
        if(head == null || head.next == null)   
            return head;
     
        Node h = new Node(0);
        h.next = head;
        Node p = h;//p(0)->1
     
        while(p.next != null && p.next.next != null){
            Node t1 = p;//node 0 before the pair
            p = p.next;
            t1.next = p.next; //0->2   
            //each loop change 3 pointers among 4 nodes 0->1->2->3 0->2->1->3
            Node t2 = p.next.next;//node 3 after the pair
            p.next.next = p;//2->1
            p.next = t2;//1->3
        }     
        return h.next;
    }
    //node contains an additional random pointer which could point to any node in the list or null.    
    public Node copyRandomList(Node head) {  
        if(head == null) 
        	return null;            
        Node i = head, n;           
        while (i != null) {  
            n = i.next;  
            Node copy = new Node(i.value);               
            i.next = copy;//old->new->oldnext, spare hashmap to store ref C++, space O(1)    
            copy.next = n;  
            i = n;  
        }  
      
        i = head;   
        while(i!=null){  
            if(i.random!=null)  
                i.next.random = i.random.next;  //copy random pointers new.random=old.randome.next(new)
            i = i.next.next; //jump to next old 
        }  
          
        i = head; n = i.next;  
        Node foo = n;  //head copy
        while(i!=null){  //decouple old and copy list 
            i.next = n.next;  
            i = i.next;  
            n.next = i==null?null:i.next;  
            n = n.next;  
        }  
        return foo;  
    }     
    //max length window|gap from given intervals with overlap
    //find common free time from friends time schedules [10:30-11] as interval(1030,1100)
    int mergeInterval(Interval[] intervalArray) {
    	int maxLen=0;//int maxGap=0;
    	Stack<Integer> ms = new Stack<Integer>();
    	for (Interval it : intervalArray)
    	{
    		if (ms.isEmpty()){
    			ms.push(it.Start);//no need for finding gap
    			ms.push(it.End);
    		}
    		else{//intervals are sorted by it.Start: ps <= ns
    			if (ms.peek() >= it.Start && ms.peek() <= it.End){//exclude next nest in prev ps ns ne pe
    				ms.pop();
   					ms.push(it.End); //overlap not nest: ps ns pe ne
    			}
    			else if(ms.peek() < it.Start){// ps pe gap ns ne
    				maxLen = Math.max(ms.pop()-ms.pop(), maxLen);
    				//maxGap=Math.max(it.Start-ms.pop(), maxGap);
    				ms.push(it.Start);//no need for gap
    				ms.push(it.End);
    			}
    		}
    	}
    	if (!ms.isEmpty()){//if the last item left. no need for finding gap
    		maxLen = Math.max(ms.pop()-ms.pop(), maxLen);
    	}
    	return maxLen;//return maxGap;
    }
    //minimum number of conference rooms required 2 for meetings [[0, 30],[5, 10],[15, 20]]
    //interval list of flights flying and landing time, How many airplanes are on the sky at most
    public int maxOverlap(Interval[] intervals) {
        if (null == intervals || intervals.length == 0) {
            return 0;
        }
        Arrays.sort(intervals, new Comparator<Interval>() {
            @Override
            public int compare(Interval o1, Interval o2) {
                return o1.Start - o2.Start;//sort the intervals by its start time
            }
        });
        PriorityQueue<Integer> queue = new PriorityQueue<Integer>();
        int count = 1;
        queue.offer(intervals[0].End);
        for (int i = 1; i < intervals.length; i++) {
            if (intervals[i].Start < queue.peek()) {
            	//count wont increment until all previous end < each current start are polled, which will be above existing count 
                count++;//if the current star time is less than the earliest end time
            } else {
                queue.poll();//keep the earliest end time until a gap, Greedy Algorithms 
            }
            queue.offer(intervals[i].End);
        }
        return count;//if >1 impossible serially complete 
    }
}

class Interval
{
    public int Start, End;
    public Interval(int start, int end) {
        this.Start = start;
        this.End = end;
    }
}
//stack retrieving the minimum element in constant time.
class MinStack {
	Node head;

	public void push(int x) {
		if (head == null) {
			head = new Node(x);
		} else {
			Node temp = new Node(x);//temp.min=x
			//int min=Math.min(head.min, x);
			//Node temp = new Node(x-min); store diff instead of orig value to reduce mem
			//temp.min=min;
			temp.min = Math.min(head.min, x);//head.min keeps min of all items
			temp.next = head;//FIFO
			head = temp;
		}
	}
	public void pop() {
		if (head == null)
			return;
		head = head.next;
	}
	public int top() {
		if (head == null)
			return Integer.MAX_VALUE;
		return head.value;//return head.value+head.min;
	}
	public int getMin() {
		if (head == null)
			return Integer.MAX_VALUE;
		return head.min;
	}
}
//implement queue with stacks, use semaphore to ensure item available in queue to pop
class QueueByStack<E>
{
    private Stack<E> inbox = new Stack<E>();
    private Stack<E> outbox = new Stack<E>();
    public void queue(E item) {
        inbox.push(item);
        //semaphore.release();
    }
    public E dequeue() {
        if (outbox.isEmpty()) {//POP prev if exist
            while (!inbox.isEmpty()) {//semaphore.acquire()
               outbox.push(inbox.pop());//reverse order
            }
        }
        return outbox.pop();
    }
}

class QueueByList{
	Node last;
	Node first;
	public void Push(int o) {
		last.next = new Node(o);
		last=last.next;
	}
	public int Pop() {
		int temp = first.value;
		first = first.next;
		return temp;
	}
}
class Node {
	int value;
	int min;//only for MinStack
	Node next;
	Node random;//only for copyRandomList

	Node(int x) {
		value = x;
		next = null;
		min = x;
	}
}

class BinarySearchTree {
	public TreeNode root;
	
	//isValidBST(root, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY)
	public boolean isValidBST(TreeNode p, double min, double max){
	    if(p==null) 
	        return true;
	 
	    if(p.value <= min || p.value >= max)
	        return false;
	    //p.value as max|min for left|rigth subtree
	    return isValidBST(p.left, min, p.value) && isValidBST(p.right, p.value, max);
	}

	public int maxDepth(TreeNode root) {//minDepth same
		if (root == null)
			return 0;
		int l = maxDepth(root.left);
		int r = maxDepth(root.right);
		return (Math.max(l, r) + 1);
	}
	//multiple children
	public int maxDepths(TreeNode root)
	{
		if (root == null)// leaf
		{
			return 0;
		} else {
			int curmax = 0;// store max depth of nodes on a same level
			for (TreeNode nd : root.SubNodes) {
				int curdepth = maxDepths(nd);// recursive call
				if (curmax < curdepth) 
					curmax = curdepth;
			}
			return curmax+1;
		}
	}

	public boolean isBalanced(TreeNode root) {
		int lh;
		int rh;
		if (root == null)
			return true;
		lh = maxDepth(root.left);
		rh = maxDepth(root.right);
		//Parent left depth=right subtree may not
		if (Math.abs(lh - rh) <= 1 && isBalanced(root.left) && isBalanced(root.right))
			return true;

		return false;
	}
	//every node except leaves has two children
	public boolean isFull(TreeNode node)
    {
        if(node == null)
        	return true;
        if(node.left == null && node.right == null )//leaf node
            return true;
        if((node.left!=null) && (node.right!=null))
            return (isFull(node.left) && isFull(node.right));
        return false;
    }

	//if any root-to-leaf path has given sum
	public boolean hasPathSum(TreeNode root, int sum) {
		if (root == null)
			return false;
		if (root.value == sum && (root.left == null && root.right == null))
			return true;
	    //if(root.value > sum)//for all node.value are >0, no need to continue 
	    	//return false;
		return hasPathSum(root.left, sum - root.value)|| hasPathSum(root.right, sum - root.value);
	}
	//path[Sum](root, [sum-root.val,] result, list); similar to combSum()
	public void pathSum(TreeNode root, int sum, ArrayList<ArrayList<Integer>> result, ArrayList<Integer> list){
	//[public void pathMinSum(TreeNode root, int cursum, int minsum, ArrayList<Integer> result, ArrayList<Integer> list){]
		//if(sum<0)	return; //all node.value are >0
		//[if(cursum>minsum) return;] //no need to continue
	    if(root.left==null && root.right==null && root.value == sum){//[root.left==null && root.right==null && cursum<minsum ]
	        ArrayList<Integer> temp = new ArrayList<Integer>();
	        temp.addAll(list);//snapshot current items in list, which is constantly repopulated 
	        result.add(temp); //[result.clear();result.addAll(list);]	        
	    }
	 
	    if(root.left != null){
	        list.add(root.left.value);
	        pathSum(root.left, sum-root.value, result, list);
	        //[pathMinSum(root.left, cursum+root.value, result, list);]
	        list.remove(list.size()-1);
	    }
	 
	    if(root.right!=null){
	        list.add(root.right.value);
	        pathSum(root.right, sum-root.value, result, list);
	        //[pathMinSum(root.left, cursum+root.value, result, list);]
	        list.remove(list.size()-1);
	    }
	}
	//node.value may<0
    public int maxPathSum (TreeNode node, int sum) {
        if (node == null) 
            return sum;
        return Math.max(sum, Math.max(maxPathSum(node.left, sum+node.value), maxPathSum(node.right, sum+node.value)));
    }
    //dfs to sum int at each node, x10 per layer 
    int sumNum(TreeNode root, int sum){  
        if (root == null) return 0;  
        if (root.left == null && root.right == null)  
            return sum * 10 + root.value;  
 
        return sumNum(root.left, sum * 10 + root.value) + sumNum(root.right, sum * 10 + root.value);  
    } 
	public void insert(int value) {
		TreeNode node = new TreeNode(value);
		if (root == null) {
			root = node;
			return;
		}
		insertNode(root, node);
	}
	//always can be inserted as leaf
	private void insertNode(TreeNode root, TreeNode node) {
		//while(root!=null)  loop version
		if (root.value > node.value) {
			if (root.left == null) {
				root.left = node;
				return;
			} else {
				insertNode(root.left, node);//recursive version
				//root=root.left; loop version
			}
		} else {
			if (root.right == null) {
				root.right = node;
				return;
			} else {
				insertNode(root.right, node);
				//root=root.right;
			}
		}
	}
	//binary tree version
	public void findPrevNextTree(TreeNode root, TreeNode node){
		int prev=0,next=0;
		while(root!=null){//loop version
			if (root.value > node.value) {
				if (root.left.value < node.value) {
					prev = root.left.value;
					next = root.value;
					return;
				} else {
					root = root.left;//findPrevNext(root.left,node) recursive version
				}
			} else if (root.value < node.value) {
				if (root.right.value > node.value) {
					prev = root.value;
					next = root.right.value;
					return;
				} else {
					root = root.right;
				}
			} else {//root.value == node.value
				prev = root.left.value;
				next = root.right.value;
				return;
			}
		}
	}
	//sorted array version
	//similar: landing scheduler to insert a time if it does not conflict with existing time spots
	public void findPrevNextArray(int[] num, int val){
		int start = 0;
		int end = num.length-1;
		int prev=0,next=0;
		while(start+1<end){//leave 1 in middle
			int mid=(start+end)/2;
			if (num[mid]<val){
				start=mid;
			}else if(num[mid]>val){
				end=mid;
			}else{//num[mid]=val
				prev=num[mid-1];
				next=num[mid+1];
				return;
			}
		}
		prev=start;//if no existing item find, num[start+1]=val;
		next=end;
	}
	
	public TreeNode lowestCommonAncestor(TreeNode root, TreeNode p, TreeNode q) {
		//assume p.value < q.value
		if (root.value > p.value && root.value > q.value) {
			return lowestCommonAncestor(root.left, p, q);
		} else if (root.value < p.value && root.value < q.value) {
			return lowestCommonAncestor(root.right, p, q);
		}
		//root.value > p.value && root.value < q.value
		return root;
	}
	//sortedArrayToBST(num, 0, num.length - 1);
	public TreeNode sortedArrayToBST(int[] num, int start, int end) {
		if (start > end)
			return null;
 
		int mid = (start + end) / 2;
		TreeNode root = new TreeNode(num[mid]);
		root.left = sortedArrayToBST(num, start, mid - 1);
		root.right = sortedArrayToBST(num, mid + 1, end);
 
		return root;
	}
	
    public TreeNode sortedListToBST(Node head) {  
        if (head == null) {
            return null;
        } else if (head.next == null) {
            return new TreeNode(head.value);
        }
        
        Node mid = findMid(head);
        TreeNode root = new TreeNode(mid.next.value);
        root.right = sortedListToBST(mid.next.next);
        mid.next = null;//split: head~mid mid.next mid.next.next~tail
        root.left = sortedListToBST(head);
       
        return root;
    }
    public Node findMid(Node head) {
        Node slow = head;
        Node fast = head.next;//hop 2 nodes 
        while (fast.next != null && fast.next.next != null) {
            slow = slow.next;
            fast = fast.next.next;
        }
        return slow;//slow at middle when fast reach the end
    }
    //Sort a linked list in O(n log n) time using constant space complexity
    public Node SortList(Node head) {  
        if (head == null || head.next == null) {
            return head;
        }
        Node mid = findMid(head);
        Node right = SortList(mid.next);
        mid.next = null;//split 2 half : head~mid and mid.next~tail
        Node left = SortList(head);
        return Merge(left, right);
    }
    //merge 2 sorted list 
    public Node Merge(Node left, Node right) {
        Node temp = new Node(0);
        Node head = temp;
        while (left != null && right != null) {
            if (left.value < right.value) {//head.next=min(left,right)
                head.next = left;
                left = left.next;
            } else {
                head.next = right;
                right = right.next;
            }
            head = head.next;
        }
        //either left or right = null (reached the end), point head.next to the other remaining list
        if (left != null) {
            head.next = left;
        } else if (right != null){
            head.next = right;
        }
        return temp.next;//initial head
    }
    //merge many sorted list log(k) * n
    //similar masterHeap manage over multiple minHeaps to behave like a single minHeap
    //each time pop from every minHeap and push to a PriorityQueue and poll min from pq
    public Node mergeKLists(Node[] lists) {
        if(lists==null||lists.length==0)
            return null;     
        PriorityQueue<Node> queue = new PriorityQueue<Node>(lists.length,new Comparator<Node>(){
            public int compare(Node l1, Node l2){
                return l1.value - l2.value;
            }
        });     
        Node head = new Node(0);
        Node p = head;     
        for(Node list: lists){
            if(list!=null)
                queue.offer(list);
        }         
        while(!queue.isEmpty()){
            Node n = queue.poll();
            p.next = n;
            p=p.next;
     
            if(n.next!=null)
                queue.offer(n.next);
        }         
        return head.next;     
    }
	public static int[] mergeKSortedArray(int[][] arr) {
		PriorityQueue<ArrayContainer> queue = new PriorityQueue<ArrayContainer>();
		int total=0;
		for (int i = 0; i < arr.length; i++) {
			queue.add(new ArrayContainer(arr[i], 0));//reference to a arr
			total = total + arr[i].length;
		} 
		int m=0;
		int result[] = new int[total];
		while(!queue.isEmpty()){
			ArrayContainer ac = queue.poll();
			result[m++]=ac.arr[ac.index]; 
			if(ac.index < ac.arr.length-1)
				queue.add(new ArrayContainer(ac.arr, ac.index+1));//add back the same arr with next index
		} 
		return result;
	}
    //merge two binary search trees into one in place
    public TreeNode mergeTree(TreeNode head1, TreeNode head2) {
		if (head1 == null)
			return head2;
		if (head2 == null)
			return head1;

		if (head1.value > head2.value) {
			TreeNode temp = head2.right;
			head2.right = null;
			head1.left = mergeTree(head1.left, head2);
			head1 = mergeTree(head1, temp);
			return head1;
		} else if (head1.value < head2.value) {
			TreeNode temp = head2.left;
			head2.left = null;
			head1.right = mergeTree(head1.right, head2);
			head1 = mergeTree(head1, temp);
			return head1;
		} else {
			head1.left = mergeTree(head1.left, head2.left);
			head1.right = mergeTree(head1.right, head2.right);

			return head1;
		}
	}
	//output from least to most http://www.javabeat.net/binary-search-tree-traversal-java/
	public void inOrderRecursive(TreeNode currRoot) {//(TreeNode currRoot,int k) //output kth smallest
		if (currRoot == null) {
			return;
		}
		inOrderRecursive(currRoot.left);
		//if(k==0)
		//	System.out.print(currRoot.value);
		//else
		//	k--;
		System.out.print(currRoot.value + ", ");//after return from the left most leaf
		inOrderRecursive(currRoot.right);
	}
    public ArrayList<Integer> traversePreOrder(TreeNode root) {
    	ArrayList<Integer> returnList = new ArrayList<>();
    	returnList.add(root.value);
    	returnList.addAll(traversePreOrder(root.left));//C++ vector a.insert(a.end(), b.begin(), b.end());
    	returnList.addAll(traversePreOrder(root.right));
    	return returnList;
    	}
	public void inOrderIterative(TreeNode currRoot) {
		if (currRoot == null) {
			return;
		}
        Stack<TreeNode> stack = new Stack<TreeNode>();
        TreeNode p = root;
 
        while(!stack.empty() || p != null){//p != null for root.right!= null 
            if(p != null){
                stack.push(p);
                p = p.left;
            //push to left most leaf-> leaf.left = null -> pop leaf -> leaf.right  = null -> pop leaf parent -> parent.right
            }else{
                TreeNode t = stack.pop();
                System.out.print(t.value + ", ");//or flatten into a sorted array or list
	            /*k--; 
	            if(k==0) result = t.value; //return kth smallest inorder from left < right*/
                p = t.right;
            }
        }
	}
    //preorder|postorder :parent node is processed before|after its children
    public ArrayList<Integer> preorderTraversal(TreeNode root) {
        ArrayList<Integer> returnList = new ArrayList<Integer>(); 
        if(root == null)
            return returnList; 
        Stack<TreeNode> stack = new Stack<TreeNode>();
        stack.push(root); //DFS, exhaust left subtree before right
        while(!stack.empty()){
            TreeNode n = stack.pop();
            returnList.add(n.value); 
            if(n.right != null){
                stack.push(n.right);
            }
            if(n.left != null){//left push after right and pop before right
                stack.push(n.left);
            } 
        }
        return returnList;
    }
	//recursive build Tree from pre|post and inorder, preStart=0; preEnd = preorder.length-1;
    //find root from pre-order and left/right subtree from in-order
    //in-order:   2 3 4 (5) 6 7 8
    //pre-order: (5) 3 2 4 7 6 8
    public TreeNode buildHelper(int[] preorder, int preStart, int preEnd, int[] inorder, int inStart, int inEnd) {
    //public TreeNode buildHelper(int[] inorder, int inStart, int inEnd, int[] postorder, int postStart, int postEnd) {	
        if (inEnd < inStart || preEnd < preStart) {
            return null;
        }

        TreeNode root = new TreeNode(preorder[preStart]);
        int pos = 0;
        for (int i = 0; i < inorder.length; i++) {
            if (inorder[i] == preorder[preStart]) {//find root element index from inorder
            //if (inorder[i] == postorder[postEnd]) {	
                pos = i;
                break;
            }
        }
        //preStart + 1: after root, pos - inStart:length of left subtree in preorder, pos - 1: left inorder subtree before root
        TreeNode left = buildHelper(preorder, preStart + 1, preStart + (pos - inStart), inorder, inStart, pos - 1);
        //TreeNode left = buildHelper(inorder, inStart, pos - 1, postorder, postStart, postStart + pos - inStart - 1);
        root.left = left;
        TreeNode right = buildHelper(preorder, preStart + (pos - inStart) + 1, preEnd, inorder, pos + 1, inEnd);
        //TreeNode right = buildHelper(inorder, pos + 1, inEnd, postorder, postStart + pos - inStart, postEnd - 1);
        root.right = right;
        return root;
    }

	public TreeNode buildTreePreInorder(int[] preorder, int[] inorder) {
		TreeNode root = new TreeNode(preorder[0]);
		Stack<TreeNode> myStack = new Stack<TreeNode>();
		myStack.push(root);
		int j = 0;
		for (int i = 1; i < preorder.length; i++) {
			TreeNode curr = myStack.peek();
			if (curr.value != inorder[j]) {
				curr.left = new TreeNode(preorder[i]);
				myStack.push(curr.left);
			} else {
				while (!myStack.empty() && myStack.peek().value == inorder[j]) {
					curr = myStack.peek();
					myStack.pop();
					j++;
				}
				if (j < inorder.length) {
					curr.right = new TreeNode(preorder[i]);
					myStack.push(curr.right);
				}
			}
		}
		return root;
	}
	//find pair of elements sum up to given num 
	//similar in sorted array, if(a[l]+a[r]<|=|>num){l++|return|r--;  
    public boolean twoSumTree(TreeNode A, int B) {
        Stack<TreeNode> stack1 = new Stack<TreeNode>();
        Stack<TreeNode> stack2 = new Stack<TreeNode>();
        TreeNode cur1 = A;
        TreeNode cur2 = A;
        
        while (!stack1.isEmpty() || !stack2.isEmpty() || cur1 != null || cur2 != null) {
            if (cur1 != null || cur2 != null) {
                if (cur1 != null) {
                    stack1.push(cur1);//inorder
                    cur1 = cur1.left;
                }                
                if (cur2 != null) {
                    stack2.push(cur2);//reverse - inorder
                    cur2 = cur2.right;
                }
            } else {//till reach the end of left|right
                int val1 = stack1.peek().value;
                int val2 = stack2.peek().value;                
                if (stack1.peek() == stack2.peek()) //same node
                	break;      
                if (val1 +  val2 == B) 
                	return true;                
                if (val1 + val2 < B) {//increase smaller node
                    cur1 = stack1.pop();
                    cur1 = cur1.right;
                } else {//decrease larger node
                    cur2 = stack2.pop();
                    cur2 = cur2.left;
                }
            }
        }        
        return false;
    }
	//if T2 is a subtree of T1
    public boolean isSubtree(TreeNode T1, TreeNode T2) {
        if (T2 == null) {
            return true;
        } else if (T1 == null) {
            return false;
        } else {
            return compare(T1, T2) || isSubtree(T1.left, T2) || isSubtree(T1.right, T2);
        }
    }
    //recursive compare node and sub
    //return compare(root.left, root.right); is a symmetric binary tree
    public boolean compare(TreeNode T1, TreeNode T2) {
        if (T1 == null && T2 == null) {
            return true;
        } else if (T1 == null || T2 == null) {
            return false;
        }
        //return T1.value == T2.value && compare(T1.left, T2.right) && compare(T1.right, T2.left);
        if (T1.value != T2.value) 
            return false;
            
        return compare(T1.left, T2.left) && compare(T1.right, T2.right);
    }    
    //mirror a tree, Iterative ref C++
	TreeNode createMirror(TreeNode root) {
		if (root == null)
			return null;
		TreeNode tmp = new TreeNode(root.value);
		tmp.left = createMirror(root.right);
		tmp.right = createMirror(root.left);
		return tmp;
	}
    public void invertRecursive(TreeNode root) {
        if (root == null) {
            return;
        }
        TreeNode temp = root.left;
        root.left = root.right;
        root.right = temp;
        
        invertRecursive(root.left);
        invertRecursive(root.right);
    }
    //BFS, ref mirror C++
    public void invertIterative(TreeNode root) {
        LinkedList<TreeNode> queue = new LinkedList<TreeNode>();
     
        if(root!=null){
            queue.add(root);
        }
     
        while(!queue.isEmpty()){
            TreeNode p = queue.poll();
            if(p.left!=null)
                queue.add(p.left);
            if(p.right!=null)
                queue.add(p.right);
     
            TreeNode temp = p.left;
            p.left = p.right;
            p.right = temp;
        }
    }
    //BFS:output tree nodes by layer
    //validate if even/odd asc/desc by layer
	public void treeByLayer(TreeNode root) {
		Queue<TreeNode> queue = new LinkedList<TreeNode>();
		queue.add(root);
		queue.add(null);//indicator for change line
		int level=0;
		while (!queue.isEmpty()) {
			TreeNode p = queue.poll();
			if (p != null) {//delimited by null
				//if(level==k) //print all nodes at k level
				System.out.print(p.value);
				if (p.left != null)
					queue.add(p.left);
				if (p.right != null)
					queue.add(p.right);
			}else if(!queue.isEmpty()){//only need next null indicator when queue has items 
				queue.add(null); //get previous null indicator mean all nodes of current layer are queued
				level++;
				System.out.println();//new layer, change even/odd asc/desc condition
			}
		}
	}
	//return only righ-most nodes 
	public List<Integer> rightSideView(TreeNode root) {
	    ArrayList<Integer> result = new ArrayList<Integer>();	 
	    if(root == null) return result;	 
	    LinkedList<TreeNode> queue = new LinkedList<TreeNode>();
	    queue.add(root);	 
	    while(queue.size() > 0){
	        int size = queue.size();	 
	        for(int i=0; i<size; i++){//size of previous layer
	            TreeNode top = queue.remove();
	            if(i==0)//the first element in the queue (right-most of the layer)
	                result.add(top.value);            
	            if(top.right != null)//add right first
	                queue.add(top.right);
	            if(top.left != null)
	                queue.add(top.left);
	        }
	    }	 
	    return result;
	}
    //Populating Next to Right Pointers in Each Node, recursive for space O(1)
	//    1 -> NULL
	//  /  \
	// 2 -> 3 -> NULL
	/// \  / \
	//4->5->6->7 -> NULL
	public void nextPtrRight(TreeNode root) {
		if (root == null)
			return;

		TreeNode left = root.left;
		TreeNode right = root.right;
		if (left != null) {
			left.next = right;//2 -> 3 
			if (root.next != null)
				right.next = root.next.left;//5->6
		}
		nextPtrRight(root.left);
		nextPtrRight(root.right);
	}
    //BFS:transform sorted array to a min binary tree 0 12 3456, queue each layer and populate children from top to bottom 
	public TreeNode arrayToMinHeap(int[] a) {
		if (a.length == 0)
			return null;
		// Queue interface implements: ArrayBlockingQueue, ConcurrentLinkedQueue, DelayQueue, LinkedBlockingQueue, LinkedList,
		// PriorityBlockingQueue, PriorityQueue, or SynchronousQueue
		Queue<TreeNode> queue = new LinkedList<TreeNode>();

		TreeNode root = new TreeNode(a[0]);
		queue.add(new TreeNode(a[0]));

		int i = 0;
		while (!queue.isEmpty()) {
			if (i >= a.length)
				break;
			TreeNode p = queue.poll();
			// children layer are add the same order as parent
			if (++i < a.length) {
				p.left = new TreeNode(a[i]);
				queue.add(p.left);
			}
			if (++i < a.length) {
				p.right = new TreeNode(a[i]);
				queue.add(p.right);
			}
		}
		return root;
	}
	//transform unsorted array with no duplicates to a max tree 
	//[2, 5, 6, 0, 3, 1] (left and right subtree also max trees of the subarray divided by the root 6)
	//     6
	//    / \
	//   5   3
	//  /   / \
	// 2   0   1
    public TreeNode arrayToMaxTree(int[] A) {
    	if (A == null || A.length == 0) {
    		return null;
    	}
    	Stack<TreeNode> stack = new Stack<TreeNode>();
    	for (int i = 0; i < A.length; i++) {
    		TreeNode node = new TreeNode(A[i]);
    		while (!stack.isEmpty() && node.value >= stack.peek().value) {
    			node.left = stack.pop();//pop all nodes < cur, set the final(largest) to cur.left, upper<lower in stack 
    		}
    		if (!stack.isEmpty()) {//stack node > cur and it has left already, only its right can be cur
    			stack.peek().right = node;
    		}
    		stack.push(node);
    	}

    	TreeNode rst = stack.pop();
    	while(!stack.isEmpty()) {
    		rst = stack.pop();//the largest value at bottom is root, which never popped
    	}
    	return rst;
    }
    //2*6-(23+7)/(1+2)
    //             -
    //            / \
    //           *   /
    //          / \ / \
    //         2  6 +  + 
    //             /\  /\
    //            23 7 1 2 
    //remove all () even redundant with blow recursive inorder     
    public TreeNode expressionToTree(String[] expression) {
        if (expression == null || expression.length == 0) {
            return null;
        }
        Stack<TreeNode> stack = new Stack<TreeNode>();
        int base = 0;
        int val = 0;

        for (int i = 0; i < expression.length; i++) {
            if (expression[i].equals("(")) {
                base += 10;//up level weight upon enter ( 
                continue;//pass thru next till )
            }
            if (expression[i].equals(")")) {
                base -= 10;
                continue;
            }
            val = getWeight(base, expression[i]);
            TreeNode node = new TreeNode(val, expression[i]);
            //min tree based on weight
            while (!stack.isEmpty() && node.value <= stack.peek().value) {//23+7+1+2 2nd + at further depth by = 
                node.left = stack.pop();
            }
            if (!stack.isEmpty()) {
                stack.peek().right = node;
            }
            stack.push(node);
        }
        if (stack.isEmpty()) {
            return null;
        }
        TreeNode rst = stack.pop();
        while (!stack.isEmpty()) {
            rst = stack.pop();//the least value at bottom of stack but top of tree
        }
        return rst;//if the tree is not full (every node except leaves has two children) means the expression is not valid
    }
    int getWeight(int base, String s) {
        if (s.equals("+") || s.equals("-")) {
            return base + 1;//Gradual weight to symbol based on current level 
        }
        if (s.equals("*") || s.equals("/")) {
            return base + 2;
        }
        return Integer.MAX_VALUE;//max weight to numbers at leaf 
    }
    public String treeToExpression(TreeNode t) {
        String cur = "";
        String l = "";
        String r= "";
        if (t != null) {
        	switch(t.value){
        	case '*':
        	case '/':
                if(t.left.value=='+' || t.left.value=='-')//(a+b)*...
                    l="("+treeToExpression(t.left)+")";    
                else
                	l=treeToExpression(t.left);
                cur=t.value+"";
                if(t.right.value=='+' || t.right.value=='-')//...*(a+b)
                    r="("+treeToExpression(t.right)+")";
                else
                	r=treeToExpression(t.right);	
                break;
        	default: //+|-|num, a+b+c+d a*b+c*d    			
                l=treeToExpression(t.left);
                cur=t.value+"";//+"" to string()
                r=treeToExpression(t.right);
                break;
                }
        	}
        return l+cur+r;
    }

}

class TreeNode {
	int value;
	String symbol;//only for expression 
	TreeNode left;
	TreeNode right;
	TreeNode next;//only for connect

	TreeNode(int x) {
		value = x;
	}
	
	TreeNode(int x, String sym) {
		value = x;
		symbol = sym;
	}

	public List<TreeNode> SubNodes; // multiple children version
}

//single producer multiple consumers
class ArrayBlockingQueue<E> {
	ArrayList<E> arr = new ArrayList<E>(10);
	synchronized E dequeue() throws InterruptedException {
		//can not use if: consumer thread 1 wait and thread 2 come in after producer notify, 2 check if and consume
		//meanwhile 1 wake up and try to consume as well, result in race condition
		while (arr.isEmpty()) 
			wait();
		return arr.remove(0);
	}
	synchronized void enqueue(E item) throws InterruptedException {
		if (arr.size() < 10) {
			arr.add(item);
			notify();
		}
	}
}
class ArrayContainer implements Comparable<ArrayContainer> {
	int[] arr;
	int index; 
	public ArrayContainer(int[] arr, int index) {
		this.arr = arr;
		this.index = index;
	} 
	@Override
	public int compareTo(ArrayContainer o) {
		return this.arr[this.index] - o.arr[o.index];
	}
}
//Heap is implemented as an array,  arrayHeap[0] has min, right>left, parent<children, 
//given element index [i] its parent[i-1/2] left[2i+1] right[2i+2]
//0123456
//    0
//  1   2
// 3 4 5 6
class MinHeap
{
	private int[] arrayHeap; 
	private int heapSize;
	private int capacity;
	MinHeap(int Capacity){
		arrayHeap = new int[Capacity];
		this.capacity = Capacity;
	}
	private	int LeftChild(int id) { return 2*id + 1; }
	private int RightChild(int id) { return 2*id + 2; }
	private int Parent(int id) {
		if ( id == 0 ) return -1;
		else return (id-1) / 2;
	}
	private void Swap(int a, int b){
		int temp = a;
		a = b;
		b = temp;
	}
	private void RemoveMin(){
		int p = 0;
		arrayHeap[p] = arrayHeap[heapSize-1];//set root the smallest value to be removed to the biggest value in heap 
		while(p < heapSize) {
			int left = LeftChild(p);
			if (left >= heapSize)
				break;
			int min = left;
			int right = RightChild(p);
			if (right >= heapSize)
				break;
			if (arrayHeap[right] < arrayHeap[left])
				min = right;
			if (arrayHeap[min] < arrayHeap[p]) {
				Swap(arrayHeap[min], arrayHeap[p]);
				p = min;
			} else {
				break;
			}
		}
	}
	public void Insert (int Item){
		if (heapSize == capacity) {
			if (Item > arrayHeap[0]) {
				RemoveMin();
				heapSize--;
			} else {
				return;
			}
		}
		int p = heapSize;
		arrayHeap[heapSize++] = Item;//arrayHeap[p] is new item to be inserted because 0 base index
		while(p != 0 && arrayHeap[Parent(p)] > arrayHeap[p]){
			int q = Parent(p);
			Swap(arrayHeap[p],arrayHeap[q]);
			p = q;
		}
	}
}
//implement Iterator support peek() as well as next() hasNext() of Iterator<Integer> 
class PeekingIterator implements Iterator<Integer> {
	private int cache;
	private Iterator<Integer> itt;
	private boolean notEnd;
	public PeekingIterator(Iterator<Integer> iterator) {
	    itt = iterator;
	    cache = itt.next();//store next value from internal iterator beforehand
	    notEnd = iterator.hasNext();
	}
    // Returns the next element in the iteration without advancing the iterator.
	public Integer peek() {
        return cache;
	}
	@Override
	public Integer next() {
		int curr = cache;
		notEnd = itt.hasNext();
		if (itt.hasNext()) {
			cache = itt.next();//keep cache updated after assign previous value to cur to return
		}
	    return curr;
	}
	@Override
	public boolean hasNext() {
	    return notEnd;
	}
	@Override
	public void remove() {
		// TODO Auto-generated method stub
	}
}
