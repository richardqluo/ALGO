package com.bbg.test;

import java.util.*;

/*     root
      /    \
     c      d
    /         \
   a           o
  /  \          \
 r    t          g  */

public class Trie {
    private TrieNode root;
    
    public Trie() {
        root = new TrieNode();
    }
 
    public void insert(String word) {
    	//children nodes char as key 
        HashMap<Character, TrieNode> children = root.children;
 
        for(int i=0; i<word.length(); i++){
            char c = word.charAt(i);
 
            TrieNode t;
            if(children.containsKey(c)){
                    t = children.get(c);
            }else{
                t = new TrieNode(c);
                children.put(c, t);
            }
 
            children = t.children;
 
            //set leaf node
            if(i==word.length()-1)
                t.isLeaf = true;    
        }
    }
    
    public TrieNode searchNode(String word){
        Map<Character, TrieNode> children = root.children; 
        TrieNode t = null;
        //drill down to the node/leaf match the whole string
        for(int i=0; i<word.length(); i++){
            char c = word.charAt(i);
            if(children.containsKey(c)){
                t = children.get(c);
                children = t.children;
            }else{
                return null;
            }
        }
 
        return t;
    }

    //auto-complete list of words starts with the given prefix.
	public List<String> startsWith(String prefix) {
		List<String> result = new ArrayList<String>();
		TrieNode t = searchNode(prefix);

		if (t != null) {
			dfs(t, prefix,result);
		}
		return result;
	}
	//recursive deep first search 
    void dfs(TrieNode node, String prefix, List<String> words){
    	if(node.isLeaf){
    		words.add(prefix);
    		return;
    	}else{
    		for(TrieNode n:node.children.values()){
    			StringBuilder sb = new StringBuilder(prefix);
    			sb.append(n.c);
    			dfs(n, sb.toString(),words);
    		}
    	}
    }
}
class TrieNode {
    char c;
    HashMap<Character, TrieNode> children = new HashMap<Character, TrieNode>();
    boolean isLeaf;
 
    public TrieNode() {}
 
    public TrieNode(char c){
        this.c = c;
    }
}
class Dictionary{    
	//dial key map btw char and digit : a-2, b-2, d-3 ...
    private char[] dials = new char[26]; //dials['a']=2;dials['b']=2;dials['d']=2;
    //same digits to all strings mapped to it   
    private HashMap<String,ArrayList<String>> dict = new HashMap<String,ArrayList<String>>();
    //store seq char from digits
    private Trie  trie = new Trie();
    public void buildDictionary(ArrayList<String> words) { 
    	for (String s : words){
    		StringBuilder digits = new StringBuilder();
    		for(int i=0; i<s.length();i++){
    			char c=s.charAt(i);
    			char d=dials[c];
    			digits.append(d);    			
    		}
    		trie.insert(digits.toString());
    	}
    } 
    //return all strings mapped to the given digit 
    public List<String> lookup(int input) { 
    	String prefix= Integer.toString(input);
    	List<String> digits=trie.startsWith(prefix);
    	List<String> words=new ArrayList<String>();
    	for(String d : digits){
    		ArrayList<String> word=dict.get(d);
    		words.addAll(word);    		
    	}
    	return words;
    } 
}
