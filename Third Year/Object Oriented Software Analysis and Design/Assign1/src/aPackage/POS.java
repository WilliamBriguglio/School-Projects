package aPackage;
import java.io.IOException;
import java.util.*;

public class POS {
	public static void main(String[] args) throws IOException{
	

		
		String ans;	//input form users
		char ans1;	//first letter from input
		int log = 1;
		
		while(log == 1){
			Scanner s = new Scanner(System.in);
			System.out.println("Would you like to start a new sale?\nType Yes or Exit.");
			ans = s.next();
			ans1 = ans.charAt(0);
			
			if( (ans1 == 69) || (ans1 == 101 )){ //first letter equals e or E
				System.out.println("Logging Off...");
				log = 0;
			}else if( (ans1 == 89) || (ans1 == 121 ) ){ //first letter equals y or Y
				System.out.println("Starting New Sale...");
				Sale aSale;
				aSale = new Sale();	//start a sale
				log = 0;
			}
			s.close();
		}

		
		
	}
}
