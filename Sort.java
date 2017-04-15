package com.bbg.test;

import java.util.*;

public class Sort {

	public static void main(String[] args) {
		int[] nums = {3,2,1,5,6,4}; int k=2;
		//kth largest = length - k +1 smallest
		//int ret = getKthSmallest(nums.length - k +1, nums, 0, nums.length - 1);
		//int[] ret = bucketSort(nums,k);
		int[] ret = countSort(new int[]{3,3,4,2,2,0,1,1},5);
		return;

	}
	//kth ((n+1)/2 for median) smallest (Largest  reverse <>) element in an unsorted array by quickselect Average  O(n), worst O(n^2).
	//similar: Top K or K-most frequent words, use hashmap(word, freq), then extract to array freq[] to pass to getKthLargest() return kth freq, go thru hashmap to find words having freq>=kth freq 
	//quickselect only recurses into one side O(n) (quicksort recursing into both sides) 
	static int getKthSmallest(int k, int[] nums, int start, int end) {
		int pivot = nums[end];
		int left = start;
		int right = end-1;
		while (left < right) {
			while (nums[left] < pivot && left < right) {
				left++;//keep checking left < right because left and right changing
			}
			while (nums[right] > pivot && left < right) {
				right--;
			}
			//after above loop, current nums[left|right] >|< pivot, swap them to right|left
			swap(nums, left, right);
		}
		//swap value at index left=right with pivot to recurse into either (< or >) side of pivot
		swap(nums, left, end);
		//nums on left < pivot, recursive till has k nums on left  
		if (k == left + 1) {//left is 0 based need +1
			return pivot;
		} else if (k < left + 1) {
			return getKthSmallest(k, nums, start, left - 1);
		} else {
			return getKthSmallest(k-left - 1, nums, left + 1, end);//discount left + 1 on first half
		}
	}
	static void swap(int[] nums, int n1, int n2) {
		int tmp = nums[n1];
		nums[n1] = nums[n2];
		nums[n2] = tmp;
	}
	//return the median (A[(n - 1) / 2] for sorted array)of numbers at every time a new number added
	//[4, 5, 1, 3, 2, 6, 0] return [4, 4, 4, 3, 3, 3, 3]
	//return n/10 by keep max heap size /9 >= min heap, O(nlogn)
	//or use count sort if knowing data range [0,k]
    public int[] streamMedian(int[] nums) {
        int[] rst = new int[nums.length];
        if (nums == null || nums.length == 0) {
            return rst;
        }
        
        PriorityQueue<Integer> minHeap = new PriorityQueue<Integer>();//default min at head
        PriorityQueue<Integer> maxHeap = new PriorityQueue<Integer>(10, new Comparator<Integer>() {
            public int compare(Integer x, Integer y) {//C++ priority_queue<int, std::greater<int>> maxHeap;
                return y - x;//max at head
            }
        });
        
        rst[0] = nums[0];
        maxHeap.offer(rst[0]);
        
        for (int i = 1; i < rst.length; i++){
            int preMedian = maxHeap.peek();
            if (nums[i] > preMedian) {//divide to 2 groups > or < head of maxHeap go to min or maxheap
                minHeap.offer(nums[i]);
            } else {
                maxHeap.offer(nums[i]);
            }
            //make 2 groups length equal so head at (n - 1) / 2
            if (maxHeap.size() > minHeap.size() + 1) {
                minHeap.offer(maxHeap.poll());
            } else if (maxHeap.size() < minHeap.size()) {
                maxHeap.offer(minHeap.poll());
            }
            rst[i] = maxHeap.peek();
        }
        return rst;
    }
	//integers well distributed over an interval (i, j). divide this interval in N equal sub-intervals (or buckets). 
	//put each number in its corresponding bucket and Sort elements in each O(n+N)
	public static int[] bucketSort(int[] array, int bucketCount) {
	    int high = array[0];
	    int low = array[0];
	    for (int i = 1; i < array.length; i++) { //find the range of input elements
	        if (array[i] > high) high = array[i];
	        if (array[i] < low) low = array[i];
	    }
	    double interval = ((double)(high - low + 1))/bucketCount; //range of one bucket

	    ArrayList<Integer> buckets[] = new ArrayList[bucketCount];
	    for (int i = 0; i < bucketCount; i++) { //initialize buckets
	        buckets[i] = new ArrayList<Integer>();
	    }

	    for (int i = 0; i < array.length; i++) { //partition the input array
	        buckets[(int)((array[i] - low)/interval)].add(array[i]);
	    }

	    int pointer = 0;
	    //only need sort some of buckets to find kth largest or median
	    for (int i = 0; i < buckets.length; i++) {
	        Collections.sort(buckets[i]); //mergeSort elements in each bucket 
	        for (int j = 0; j < buckets[i].size(); j++) { //merge the buckets
	            array[pointer] = buckets[i].get(j);
	            pointer++;
	        }
	    }
	    return array;
	}
	//h(3) of N(5) papers have >= h citations each, and rest N-h papers have <= h citations each
	//find max index that there are no more than i items >= array[i] 
	//naive:sort array and iterate desc if (i + 1 > citations[i]) {return i;} 6 5 3 1 0 return 3
	//O(N) by  bucket sort, O(logn) ref c++
    public int hIndex(int[] citations) {
        if (citations == null || citations.length == 0) 
        	return 0;
    
        int n = citations.length;
        int[] buckets = new int[n + 1];//extra last item
        for (int i = 0; i < n; i++) {
            if (citations[i] <= n) {
                buckets[citations[i]]++;//each bucket holds the number of papers of which the citation is i
            } else {
                buckets[n]++;//Occurrence of number > n are added together to the extra last item 
            }
        }
        int numPapers = 0;
        for (int i = n; i >= 0; i--) {//buckets[1,1,0,1,0,2]
            numPapers += buckets[i];
            if (numPapers >= i) {//i desc
                return i;
            }
        }
        return 0;
    }
	//n positive integer within 0-k with dups O(n+k), applicable to 
	//sort an array of n objects with (0 to k) different colors 
	//similiar put each item from unsorted array to array count[item]++ per occurrence
	//output thru its index (sorted), 0 no output, dup output -- till 0  
	static int[] countSort(int[] in, int k) {//[3,3,0,2,2] 3
	    if(in==null||in.length<2){
	        return null;
	    }
	 
	    int[] ca = new int[k+1];//number of unique elements
	    for(int i=0; i<in.length; i++){
	        ca[in[i]]++;//counts occurrence of each int, [1,0,2,2]
	    }
	 
	    for(int i=1; i<ca.length; i++){
	        ca[i]=ca[i-1]+ca[i];//increment all prev counts, ca[last]=in.length, [1,1,3,5] 
	    }
	 
	    int[] out = new int[in.length];
	    for(int i=0;i<in.length; i++){
	    	out[ca[in[i]]-1]=in[i];//put num of same value at a section, out[5-1]=in[0]=3 
	    	ca[in[i]]--;//[1,1,3,4]
	    }
	    //median= out[ca[k]/2];
	    return out;//[0,2,2,3,3]
	}
	//moving window(size k) over an array returns list of max value for each slide
    public ArrayList<Integer> maxSlidingWindow(int[] nums, int k) {
    	ArrayList<Integer> rst = new ArrayList<Integer>();
    	if (nums == null || nums.length == 0 || k < 0) {
    		return rst;
    	}
    	Deque<Integer> deque = new LinkedList<Integer>();//Maintain index of potential max values in decreasing order for current window
    	for (int i = 0; i < nums.length; i++) {
    		if(i>=k){ //start sliding the window by i++
	    		rst.add(nums[deque.peekFirst()]);//add the max from previous window
	    		if (!deque.isEmpty() && deque.peekFirst() <= i - k) //peekFirst() index is out of the current window
	    			deque.pollFirst();
    		}
    		while (!deque.isEmpty() && nums[deque.peekLast()] <= nums[i]) //remove all index of smaller values 
    			deque.pollLast();//no need to keep all index as long as have the max at first with descending order  
    		deque.offerLast(i);
    	}
    	rst.add(nums[deque.peekFirst()]);//Last move's result needs to be recorded
    	return rst;
    }
    //reorder unsorted array in-place such that nums[0] <= nums[1] >= nums[2] <= nums[3]
    //[3, 2, 4], [2, 3, 4] [2, 4, 3] since 3 > 2 at first round, it is not possible 4 will be < 2 at second round 
    public void wiggleSort(int[] nums) {
    	if (nums == null || nums.length <= 1) {
    		return;
    	}
    	int flag = 1;
    	for (int i = 1; i < nums.length; i++) {
    		if (flag * nums[i] < flag * nums[i - 1]) {
    			swap(nums, i, i - 1);
    		}
    		flag = -1 * flag;//switch < > value
    	}
    }
    //sort the given array of pancake size by only flip: Reverse array from top 0 to i, not i to bottom n 
    public void pancakeSort(int arr[])
    {
        for (int cur = arr.length-1; cur >= 1; cur--)//last compare between [0][1]
        {
            int mi = 0;// Find index of the maximum element in [0,cur]
            for (int i = 0; i <= cur; ++i){
                if (arr[i] > arr[mi])
                    mi = i;
            }
            // Move the maximum to end of current array if it's not already at the end
            if (mi != cur)
            {	// first flip max to [0] then flip to [n] because can not directly flip to n
                flip(arr, mi);//if mi=0 then no change
                flip(arr, cur);
            }
        }
    }
    static void flip(int arr[], int i)
    {
        int temp, start = 0;
        while (start < i)
        {
            temp = arr[start];
            arr[start] = arr[i];
            arr[i] = temp;
            start++;
            i--;
        }
    }
    
	static void sortMapValue(Map<Integer,String> map) {
		List list = new ArrayList(map.entrySet());
		Collections.sort(list, new Comparator() {
			public int compare(Object obj1, Object obj2) {
				return ((Comparable) ((Map.Entry) (obj1)).getValue()).compareTo(((Map.Entry) (obj2)).getValue());
			}
		});
	}
}


