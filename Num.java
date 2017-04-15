package com.bbg.test;

import java.io.IOException;
import java.util.*;

public class Num {
	private Map<Integer, Integer> cache = new HashMap<Integer, Integer>();
	private ArrayList<Integer> hist = new ArrayList<Integer>();
	private int coins[] = {1,5,10,25};
    public static String[] SCALE1 = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    public static String[] SCALE2 = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};//no need for 0 and 1 as ten
    public static String[] SCALE3 = {"", "Thousand", "Million", "Billion"};
    
    public static void main(String[] args) throws IOException {
    	boolean[] p=findPrimes(20);
    	System.out.println(amultiplyb(5,6));
    }

	public int Fibonacci(int n)// 0,1,1,2,3,5..
	{
		if (n < 2)
			return n;
		if (cache.containsKey(n)) {
			return cache.get(n);//memorization pattern
		} else // recursive: if initial n is too big then many recursive calls will cause callstack overflow
		{
			cache.put(n, Fibonacci(n - 1) + Fibonacci(n - 2)); // O(2^n), without cache overflow at n=100
			return cache.get(n);//with cache overflow at n=100k 
		}
	}
	//recursive is significantly slower than the iterative due to heavy push-pop of the registers in the ill level of each recursive call.
	//n=20	recursive 2315 ticks, recursive cache 61 ticks, iterative 10 ticks
	int fibo(int n) {
		if (n < 2) {
			return n;
		}
		int fibo = 1;
		int fiboPrev = 0;
		for (int i = 2; i <= n; ++i) {//O(n)
			int temp = fibo;
			fibo += fiboPrev;
			fiboPrev = temp;
		}
		return fibo;
	}

	public int reverseByMath(int x) {
		long r = 0;//avoid overflow, if x lower digit > higher, reversed x may overflow  even x are not
		while (x > 0) {
			r = r * 10 + x % 10;
			x /= 10;
		}
		return r>Integer.MAX_VALUE?0:(int)r;
	}

	public int reverseBinary(int num) {
		int res = 0;
		while (num > 0) {
			res = res << 1; // shift a bit left
			res = res | (num & 1); //the lowest bit
			num = num >> 1; // shift a bit right
		}
		return res;
	}
	//No need of extra space, better than i==reverseByMath(i) 
    public boolean isPalindrome(int x) {
    	//2332 div=1000
		int div = 1;
		while (x / div >= 10) {
			div *= 10;
		}
 
		while (x >=10) {
			int left = x / div;
			int right = x % 10;
 
			if (left != right)
				return false;
 
			x = (x % div) / 10;
			div /= 100;//2 digits because both head and tail move 1 digit 
		}
 
		return true;
    }
    //9: 1,2,..,9 = 9
    //99: 11,22,..,99 = 9
    //999: 101,111,..,191,..,999 = 9*10
    //9999: 1001,1111,..,1991,..,9999 = 9*10
    //99999: 10001,10101,..,10901,..,99999 = 9*10*10
    //999999: 100001,101101,..,109901,..,999999 = 9*10*10
    public int countPalindrome(int size){
    	if (size<=2)
    		return 9;
    	else if(size%2==1)//odd size
    		return countPalindrome(size-1)*10;
    	else //even size
    		return countPalindrome(size-2)*10;
    }
    //999999: 100001,101101,..,109901,..,999999 = 9*10*10
    public void outputPalindrome(){
	    for(int i = 1; i <= 9; i++) {//outmost from 1
	    	for(int j = 0; j <= 9; j++) {
	    		for(int k = 0; k <= 9; k++) {	    			
	    			System.out.println(i*100000+j*10000+k*1000+k*100+j*10+i);
	    		}
	    	}
	    }
    }
	// find prime num under n
	public void getPrime(int n) {
		hist.add(2);
		for (int i = 2; i < n; i++) {
			for (int item : hist) {
				if (i % item == 0)
					break;
				if (Math.pow(item, 2) > i) {//i=11 {2,3,5,7,11}only need to run by item=3
					hist.add(i);
					break;
				}
			}
		}
	}
	public static boolean[] findPrimes(int n) {
		boolean[] primes=new boolean[n+1];
		Arrays.fill(primes, true);
		for (int p = 2; p * p <= n; p++) {//no need to go beyond p*p
			if (primes[p]){
				for (int i=p*2; i<=n; i+=p)//start from p*2 so primes[p] wont be reset to false 
					primes[i] = false;
			}
		}
		return primes;
	}
	public boolean isPrime(int n) {
		for (int i = 2; i * i <= n; i++) {
			if (n % i == 0) {
				return false;
			}
		}
		return true;
	}
	//ugly number: only have factors 2 3 5, fxp 6 10  not 14
	public boolean isUgly(int n) {
		int[] primes={2,3,5};
		for(int i : primes){
			while(n%i==0){
				n /= i;
			}
		}
		return n==1;//no other factor left 
	}
	//http://www.geeksforgeeks.org/ugly-numbers/
	//large ugly numbers are smaller ugly numbers multiply 2, 3, 5
	public int NthUgly(int n){
		int[] us = new int[100];//1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15..
		us[0]=1;
		int i2=0,i3=0,i5=0;
		int m2=2,m3=3,m5=5;
		int u=0;
		for(int i=1;i<n;i++){
			u=Math.min(Math.min(m2, m3),m5); //[2 3 5] (0 0 0) [4(2x2) 3 5] (1 0 0) [4 6(2x3) 5] (1 1 0) [6(3x2) 6 5] (2 1 0) [6 6 10(2x5)] (2 1 1)
			us[i]=u;//keep increment one of factors that was min value
			if(u==m2){
				i2++; 
				m2=us[i2]*2;
			}
			if(u==m3){//not use 'else if' because u may = both m2 and m3, [6 6 10] (2 1 1) [8(4x2) 9(3x3) 10] (3,2,1), both i2 and i3 incremented
				i3++;
				m3=us[i3]*3;
			}
			if(u==m5){
				i5++;
				m5=us[i5]*5;				
			}
		}
		return u;
	}
	//stepping number if the adjacent digits are having diff of 1: 12 121 123 1234 5654
    public static ArrayList<Integer> stepnum(int n) {
        ArrayList<Integer> result = new ArrayList<Integer>();
        Queue<Integer> queue = new LinkedList<Integer>();

        for(int j = 1; j < 10; j++)
            queue.offer(j);//1-9
        int temp = queue.poll();
        while(temp <= n){         	
            if(temp>10)
                result.add(temp);
            
            int lastDigit = temp % 10;
            if(lastDigit == 0)//temp is stepnum (wont be 20) 10 -> 101 
                queue.offer(temp * 10 + lastDigit + 1);
            else if(lastDigit == 9)
                queue.offer(temp * 10 + lastDigit - 1);
            else{
                queue.offer(temp * 10 + lastDigit - 1);
                queue.offer(temp * 10 + lastDigit + 1);
            }
            temp = queue.poll();            
        }
        return result;
    }
    //1+ 2 + 4 + 7 + 14= 28=2x14=4x7
    void findPerectNum(int n){//btw 1 to n
     for(int pn = 2; pn <= n; pn+=2){//PerectNum always even
      int sum=0;
      for (int div = 1; div <= pn/2; ++div){//because 2X so divider <= half 
        if(pn%div==0)
           sum=sum+div;
      }
      if(sum==pn)
    	  System.out.println(pn);
     }
    }
	//(n-1)^2 = (n-1)(n-1) = n^2-2n+1,  n^2=(n-1)^2+2n-1
	public int square(int n){
        if(n == 0){
            return 0;
        }else{
            return ((square(n-1)+ (2*n)-1));
        }
    }
	//y^2 == x  
    int sqrt(int x) {
        if(x<=1) return x;//Corner case 0/1
        //if float x < 1 binary search start=x end=1 x<y<1:sqrt(0.25)=0.5>0.25
        int start=1, end=x;//0<y<=x, 
        while(start<=end) {//while(end-start>1e-9)
            int mid = start + (end-start)/2;
            if(x/mid==mid) //x/y==y, instead of x==mid*mid to avoid overflow
                return mid;
            else if(x/mid<mid)
                end = mid-1;
            else
                start = mid+1;
        }
        while(end-start>1e-9){//for double, never = due to precision 
            int mid = (start + end)/2;
            if(x/mid<mid)
                end = mid;
            else
                start = mid;
        }
        return end;//approx
        /*
        double x0 = 1;
        double x1;
        while (true){
            x1 = (x0+ x/x0)/2;
            if (Math.abs(x1-x0)<1)//Newton's method return float
            	return x1;
            x0=x1;
        }
        */
    }
	public static int powLogN(int a, int n)//O(logn)
	{	
		if (n < 0)
			return 0;
		if (n == 0)
			return 1;
		else if (n == 1)
			return a;
		else if(n%2==0)
			return power(a,n/2) * power (a,n/2);
		else //2^5=2^2*2^2*2
			return power(a,n/2) * power (a,n/2)*a;
	}
	//can not be used for pow(1/a, abs(n)) (n<0) 2^(-3) = (1/2)^3 due to precison lost by 1/a
	int pow(int a,int n)//O(n)
	{
		if(n==0)
			return 1;
		else
			return (a*pow(a,n-1));
	}
	public static int power(int a, int n)
	{	
			int r=1;
			while(n>0){
				r*=a;
				n--;
			}
			return r;
	}
	boolean isPower(int a, int n)
	{
		while(a>=n && a%n==0)
			a/=n;
		return a==1;
	}
	//n people in circle and remove each m hop on, find the last one left: cycledlinklist O(nm)
	//better way: f(n) = (f(n-1) + m) mod n, O(n)
	//n=5 m=3 [0 1 2 3 4] remove 2 and start from next 3 [3 4 0 1] 
	//n=4 m=3 [0 1 2 3] convert by above formula (x+3) mod 5 to [3 4 0 1] 
	public int Josephus(int n, int m)
	{
	  int s = 0;//only 1 people 
	  for (int i = 2; i <= n; i++)
	    s = (s + m) % i;
	  return s;
	}
	//similar to Josephus, from 1 2 3 4...remove by 2 then remove by 3 ..., find if n will be left
	public Boolean isLucky(int n,int step)//isLucky(n,2)
	{
	  if(step > n)//no longer fall on n 
	    return true;
	  if(n%step == 0)//fall on n 
	    return false; 
	  n -= n/step;//remove
	  step++;
	  return isLucky(n,step);
	}
	//1924: 192+4=196 19+6=25 2+5=7
	public static int digitRoot(int n) {
	    int root=n;
	    while ( (root=(root%10 + root/10))>9  );
	    return root;
	} 
	//1924: 1+9+2+4=16 1+6=7
	public static int digitRootRecursive(int n)
	{
	    if (n < 10)
	        return n;
	    int sum = 0;
	    int temp = n;
	    while (temp > 0){
	        sum += temp % 10;
	        temp = temp/ 10;
	    }
	    return digitRootRecursive(sum);
	} 
	//11 [9,5,6,1] = 2
	int minCoins(int n)	{//time complexity is exponential.
	   if (n == 0) return 0;
	   int min = Integer.MAX_VALUE;
	 
	   for (int i=0; i<coins.length; i++)
	   { //5 5 1=3, 6 5=2, min keep minimal count across i on each recursion level 
	     if (coins[i] <= n)
	    	 min=Math.min(min, minCoins(n%coins[i]) + n/coins[i]);//less recursion than minCoins(n-coins[i]) + 1
	   }
	   return min;
	}
	//min[] store min coins for each amount from 1 to n
	int DPMinCoins(int n) {//time complexity O(nm) m = coins size
		int[] min = new int[n + 1];//1D
		int i, j;
		Arrays.fill(min, Integer.MAX_VALUE);
		min[0] = 0;
		for (i = 1; i <= n; i++) {
			for (j = 0; j < coins.length; j++) {
				if (coins[j] <= i && min[i - coins[j]] + 1 < min[i]) {
					min[i] = min[i - coins[j]] + 1;//if use a coin j make count less
				}
			}
		}
		return min[n];
	}
	//find min steps from 1 to by only *2 or +1
	int minSteps(int n){
		int steps=0;
		while(n!=1){
			if(n%2==1){
				n=n-1;
			}else{//if even/2 instead of -1 because /2 faster than -1 
				n=n/2;
			}
			steps++;
		}
		return steps;
	}
	//a+b by bit operation
    public int aplusb(int a, int b) {
        while (b != 0) {//till there is no carry
            int carry = a & b;//& common bits of a b 
            a = a ^ b;//^ sum bits of a b where at least one of the bits is not set
            b = carry << 1;//Carry is shifted by one so that adding it to a gives the required sum
        }
        return a;
    }
    public static int amultiplyb(int a, int b) {
        int p = 0;  
        
        if ((a & 1) > 0) // If a is odd number (3)11&01=1, (2)10&01=0. 
            p = b; 
        // If 'a' contains any number larger than one than continue recursion.
        if (a > 1)
            p = p + amultiplyb(a >> 1, b << 1);//recursive
        
        while(b!=0){//iterative
        	if((b&1) > 0)
        		p=p+a;//p=aplusb(p,a)
        	a<<=1;//a=a<<1;
        	b>>=1;
        }
        
        return p;
    }
    //big number exceed numeric type size, but can be stored by string , list ref C++
    public String bigAdd(String a, String b){  
        int len1 = a.length();  
        int len2 = b.length();  
        int len = len1;  
        if(len1 > len2){  
            for(int i = 0; i < len1 - len2; i ++)  
                b = '0' + b; //append 0 to make same length 
            len = len1;  
        }else if(len1 < len2){  
            for(int i = 0; i < len2 - len1; i ++) 
                a = '0' + a;  
            len = len2;  
        }  
        int c[] = new int[len];  
        int a1, b1;  
        int carry = 0;  
        String s = "";  
        for(int i = len - 1; i >= 0; i --){  //from low to high  
        	a1 = a.charAt(i) - '0';  //char to int
        	b1 = b.charAt(i) - '0';                
            c[i] = (a1 + b1 + carry) % 10;  //carry from previous lower digits add
            carry = (a1 + b1 + carry) / 10; //a1*b1 for multiply   
            s = String.valueOf(c[i]) + s;  //prepend higher digit in front  
        }            
        return s;  
    } 
    //hashcode("abcd") = (ascii(a) * 33^3 + ascii(b) * 33^2 + ascii(c) *33^1 + ascii(d)*33^0) % HASH_SIZE with magic num 33
    public int hashCode(char[] key, int HASH_SIZE) {
        if (key.length == 0) {
            return 0;
        }
        long hashRst = 0;//hashRst * 33 may overflow int
        for (int i = 0; i < key.length ; i++) {
            hashRst = hashRst * 33 + (int)(key[i]);
            hashRst = hashRst % HASH_SIZE;//mod every loop since hashRst maybe big
        }
        return (int)hashRst;//revert to int after mod
    }
    //1234567899    OneBillionTwoHundredThirtyFourMillionFiveHundredSixtySevenThousandEightHundredNinetyNine
    public static String digitsToWords(int num) {
        if (num < 0) 
            return "";
        if (num == 0) 
            return "Zero";
        
        String rst = "";
        int i = 0;
        while(num>0 && i <= SCALE3.length) {
            int partial = num%1000;
            if (partial > 0) 
                rst = ThreeDigitsToWords(partial, i)+ rst;//from lower to higher digits
            
            num /= 1000;
            i++;
        }
        return rst;
    }

    public static String ThreeDigitsToWords(int num, int i) {
   	 StringBuilder str = new StringBuilder();
        if (num >= 100) {
            int hund = num / 100;
            str.append(SCALE1[hund]).append("Hundred");
            num = num % 100;
        }

        if (num < 20) {
       	 str.append(SCALE1[num]);
        } else {
            int numTens = num / 10;
            int numDigit = num % 10;
            str.append(SCALE2[numTens]).append(SCALE1[numDigit]);
        }
        str.append(SCALE3[i]);

        return str.toString();
    }
    //["2", "1", "+", "3", "*"] -> ((2 + 1) * 3) -> 9
	public static int evalReversePolishNotation(String[] tokens) {
		int returnValue = 0;
		String operators = "+-*/";
 
		Stack<Integer> stack = new Stack<Integer>();
 
		for (String t : tokens) {
			if (!operators.contains(t)) { //push to stack if it is a number
				stack.push(Integer.valueOf(t));
			} else {//pop numbers from stack if it is an operator
				int a = stack.pop();
				int b = stack.pop();
				switch (t) {
				case "+":
					stack.push(a + b);//push intermediate result 
					break;
				case "-":
					stack.push(b - a);
					break;
				case "*":
					stack.push(a * b);
					break;
				case "/":
					stack.push(b / a);
					break;
				}
			}
		}
		returnValue = stack.pop();
		return returnValue;
	}
	
    public int RomanToInt(String s) {
        if (s == null || s.length() == 0) {
            return 0;
        }
        int num = 0;
        for (int i = 0; i < s.length(); i++) {
            switch(s.charAt(i)) {
                case 'I': num += 1; break;
                case 'V': num += 5; break;
                case 'X': num += 10; break;
                case 'L': num += 50; break;
                case 'C': num += 100; break;
                case 'D': num += 500; break;
                case 'M': num += 1000; break;
                default:
                    return 0;
            }
        }
        //'IV, IX'-1, 'XL, XC'-10, 'CD, CM'-100 : letter append with following next 2 letters
        num -= countMatch(s, "IV") * 2;
        num -= countMatch(s, "IX") * 2;
        num -= countMatch(s, "XL") * 10 * 2;
        num -= countMatch(s, "XC") * 10 * 2;
        num -= countMatch(s, "CD") * 100 * 2;
        num -= countMatch(s, "CM") * 100 * 2;
        
        return num;
    }
    //number of matches
    public int countMatch(String s, String key) {
        int count = 0;
        while (s.indexOf(key) != -1) {
            count++;
            int i = s.indexOf(key);
            s = s.substring(i + 2);//continue on rest after like 'IV'
        }
        return count;
    }

}
