package edu.nyu.compfin14;

import java.util.*;

public class OrderBook {

	public static void main(String[] args) {
		OrdersIterator ordersIterator = new OrdersIterator();
		BuildOrderBook(ordersIterator.getIterator(),true);
	}

	private static void BuildOrderBook(Iterator<Message> messages,boolean showBest){
		//tree map to store sorted book by price per symbol
		HashMap<String, TreeMap<Double, Integer>> bidBook = new HashMap<String, TreeMap<Double,Integer>>();
		HashMap<String, TreeMap<Double, Integer>> askBook = new HashMap<String, TreeMap<Double,Integer>>();
		HashMap<String, NewOrder> prevOrders = new HashMap<String, NewOrder>();
		
		final double MAX_PRICE = 99999.99;
		final double MIN_PRICE = 0.00;
		
		while (messages.hasNext()){
			Message message = messages.next();
			String symbol = "";
			if (message instanceof NewOrder){
				NewOrder newOrder = (NewOrder)message;
				int size = newOrder.getSize();
				double limitPrice = newOrder.getLimitPrice();
				symbol = newOrder.getSymbol();
				String orderId = newOrder.getOrderId();

				prevOrders.put(orderId, newOrder);
				if(size>0){
						if (limitPrice!=limitPrice){
							//market buy order without matching ask book will be stored on top of bid book at max price to be filled by coming sell order 
							PutOrder(bidBook, symbol, MAX_PRICE, size);							
						}
						else{//limit buy order
							PutOrder(bidBook, symbol, limitPrice, size);
						}
				}
				else if (size<0){
						if (limitPrice!=limitPrice){//market sell order
							//market sell order without matching bid book will be stored on top of ask book at min price to be filled by coming buy order
							PutOrder(askBook, symbol, MIN_PRICE, size);
						}
						else{//limit sell order
							PutOrder(askBook, symbol, limitPrice, size);
						}
				}
			}
			else if(message instanceof OrderCxR) {
				OrderCxR orderCxR = (OrderCxR)message;
				int size = orderCxR.getSize();
				double limitPrice = orderCxR.getLimitPrice();
				if (limitPrice!=limitPrice){//market order
					limitPrice = size>0 ? MAX_PRICE : MIN_PRICE; 
				}
				String orderId = orderCxR.getOrderId();
				
				NewOrder prevOrder = (NewOrder)prevOrders.get(orderId);
				int prevSize = prevOrder.getSize();
				double prevLimitPrice = prevOrder.getLimitPrice();
				if (prevLimitPrice!=prevLimitPrice){//market order
					prevLimitPrice = prevSize>0 ? MAX_PRICE : MIN_PRICE; 
				}
				symbol = prevOrder.getSymbol();
				
				prevOrders.remove(orderId);
				if (size!=0){
					prevOrders.put(orderId, new OrdersIterator.NewOrderImpl(symbol,orderId,size,limitPrice));
				}
				if(prevSize>0){
					ResetOrder(bidBook, symbol, prevLimitPrice, prevSize, limitPrice, size);
				}
				else if (prevSize<0){
					ResetOrder(askBook, symbol, prevLimitPrice, prevSize, limitPrice, size);
				}
			}
			
			CrossOrder(askBook, bidBook, symbol);
			
			if (showBest){
				if(askBook.containsKey(symbol)){
					TreeMap<Double,Integer> askMap = (TreeMap<Double,Integer>)askBook.get(symbol);
					double bestAskPrice=0.00;
					if (askMap.size()>0){
						bestAskPrice = askMap.firstKey();
						if (bestAskPrice==MIN_PRICE){//ignore market order in book
							if (askMap.size()>1){
								bestAskPrice = askMap.higherKey(bestAskPrice);
							}
							else{
								continue;
							}
						}
					}
					else{
						continue;
					}
					int size = askMap.get(bestAskPrice);
					System.out.println(String.format("%s @ %f ( %d )", symbol,bestAskPrice,size));
				}
				if(bidBook.containsKey(symbol)){
					TreeMap<Double,Integer> bidMap = (TreeMap<Double,Integer>)bidBook.get(symbol);
					double bestBidPrice=0.00;
					if (bidMap.size()>0){
						bestBidPrice = bidMap.lastKey();
						if (bestBidPrice==MAX_PRICE){//ignore market order in book
							if (bidMap.size()>1){
								bestBidPrice = bidMap.lowerKey(bestBidPrice);
							}
							else{
								continue;
							}
						}
					}
					else{
						continue;
					}
					int size = bidMap.get(bestBidPrice);
					System.out.println(String.format("%s @ %f ( %d )", symbol,bestBidPrice,size));
				}
			}
		}
	}
	
	private static void PutOrder(HashMap<String, TreeMap<Double,Integer>> hashMap, String symbol, double price, int size){
		TreeMap<Double,Integer> treeMap = null;
		if(hashMap.containsKey(symbol)){
			treeMap = (TreeMap<Double,Integer>)hashMap.get(symbol);
		}
		else{
			treeMap = new TreeMap<Double,Integer>();
		}
		if (treeMap.containsKey(price)){
			treeMap.put(price, treeMap.get(price)+size);
		}
		else{
			treeMap.put(price, size);
		}
		hashMap.put(symbol, treeMap);
	}
	
	private static void ResetOrder(HashMap<String, TreeMap<Double,Integer>> hashMap, String symbol, double prevPrice, int prevSize, double price, int size){
		if(hashMap.containsKey(symbol)){
			TreeMap<Double,Integer> treeMap = (TreeMap<Double,Integer>)hashMap.get(symbol);
			int filledSize=0;
			int curSize=Math.abs(treeMap.get(prevPrice))-Math.abs(prevSize);
			if (curSize>0){//not filled
				treeMap.put(prevPrice, curSize);
			}
			else{//totally or partially filled
				treeMap.remove(prevPrice);
				filledSize=curSize;
			}
			int deductSize=Math.abs(size)+filledSize;//deduct partially filled size  
			if (deductSize>0){
				if(treeMap.containsKey(price)){
					treeMap.put(price, treeMap.get(price)+(size>0 ? deductSize : -deductSize));
				}
				else{
					treeMap.put(price,size>0 ? deductSize : -deductSize);
				}
			}
			if(treeMap.isEmpty()){
				hashMap.remove(symbol);
			}
		}
	}
	
	private static void CrossOrder(HashMap<String, TreeMap<Double,Integer>> askHashMap, HashMap<String, TreeMap<Double,Integer>> bidHashMap, String symbol){
		if(askHashMap.containsKey(symbol) && bidHashMap.containsKey(symbol)){
			TreeMap<Double,Integer> askTreeMap = (TreeMap<Double,Integer>)askHashMap.get(symbol);
			TreeMap<Double,Integer> bidTreeMap = (TreeMap<Double,Integer>)bidHashMap.get(symbol);
			double bestAskPrice=askTreeMap.firstKey();
			double bestBidPrice=bidTreeMap.lastKey();
			
			while(bestBidPrice>=bestAskPrice){
				int crossSize = askTreeMap.get(bestAskPrice) + bidTreeMap.get(bestBidPrice);
				
				if (crossSize==0){
					askTreeMap.remove(bestAskPrice);
					bidTreeMap.remove(bestBidPrice);
				}
				else if (crossSize<0){
					askTreeMap.put(bestAskPrice, crossSize);
					bidTreeMap.remove(bestBidPrice);
				}
				else if(crossSize>0){
					bidTreeMap.put(bestBidPrice, crossSize);
					askTreeMap.remove(bestAskPrice);
				}
				if(askTreeMap.isEmpty()){
					askHashMap.remove(symbol);
				}
				if(bidTreeMap.isEmpty()){
					bidHashMap.remove(symbol);
				}
				if(askTreeMap.isEmpty()||bidTreeMap.isEmpty()){
					break;
				}
				
				bestAskPrice=askTreeMap.firstKey();
				bestBidPrice=bidTreeMap.lastKey();
			}
		}
	}
}
