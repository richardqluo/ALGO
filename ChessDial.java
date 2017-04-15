package com.hbk.test;
import java.io.*;

public class ChessDial {
	private static int numLen;
	private static char [] [] buttons;
	private static int numRows;
	private static int numCols;
	private static String stepString;
	
	public static void main(String[] args) {

		stepString = Reader.readLine();

		String lenString = Reader.readLine();
		numLen = Integer.parseInt(lenString);
		
		String startString = Reader.readLine().replaceAll("\\s+","");//remove space
		
		//rows
		String numRowsString = Reader.readLine();
		numRows = Integer.parseInt(numRowsString);
		//cols
		String numColsString = Reader.readLine();
		numCols = Integer.parseInt(numColsString);

		String [] rows = new String [numRows];
		for (int i = 0; i < numRows; i++) {
			rows[i] = Reader.readLine().replaceAll("\\s+","");
		}
		
		buttons = new char [numRows] [numCols];
		for (int r = 0; r < numRows; r++) {
			for (int c = 0; c < numCols; c++) {
				buttons[r][c] = rows[r].charAt(c);
			}
		}
		
		int count = 0;
		
		for (int r = 0; r < numRows; r++) {
			for (int c = 0; c < numCols; c++) {
				if (startString.indexOf(buttons[r][c]) > -1) {
					count = count + countPhoneNumbers(numLen, r, c);
				}
			}
		}
		
		System.out.println(count);
	}
	
	public static int countPhoneNumbers (int n, int r, int c) {
		if (outOfBounds(r,c)) {
			return 0;
		} else {
			char button = buttons[r][c];
			if (button  == '.' || button  == '*' || button  == '#') {
				// visited
				return 0;
			} else if (n == 1) {
				// count it as a one-digit phone number.
				return 1;
			} else {
				buttons[r][c] = '.'; // record this position so don't revisit.
				// Count all possible phone numbers with one less digit starting
				int result=0;
				if (stepString.equals("knight")){
				result = countPhoneNumbers(n-1,r-2,c-1)
										 + countPhoneNumbers(n-1,r-2,c+1)
										 + countPhoneNumbers(n-1,r+2,c-1)
									 	 + countPhoneNumbers(n-1,r+2,c+1)
									 	 + countPhoneNumbers(n-1,r-1,c-2)
									 	 + countPhoneNumbers(n-1,r-1,c+2)
									 	 + countPhoneNumbers(n-1,r+1,c-2)
									 	 + countPhoneNumbers(n-1,r+1,c+2);
				}
				else if (stepString.equals("bishop")){
				result = countPhoneNumbers(n-1,r-1,c-1)
						 + countPhoneNumbers(n-1,r-1,c+1)
						 + countPhoneNumbers(n-1,r+1,c-1)
					 	 + countPhoneNumbers(n-1,r+1,c+1);
				}
				buttons[r][c] = button; // Remove record from position.
				return result; 
			}
		}
	}

	public static boolean outOfBounds(int row, int col) {
		return (row < 0) || (row >= numRows) || (col < 0) || (col >= numCols);
	}
}

class Reader {
	public static String readLine() {
		try {
			StringBuffer sb = new StringBuffer();
			int i = System.in.read(); // Returns next character from standard input,
		                               // or -1 if no such character.
			while ((i != -1) && (i != (int) '\n')) {
				sb.append((char) i);
				i = System.in.read();
			}
			return sb.toString().trim();
		} catch (IOException e) {
			return "";
		}
	}	
}
