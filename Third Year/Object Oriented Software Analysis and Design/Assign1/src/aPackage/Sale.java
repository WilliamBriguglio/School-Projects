package aPackage;
import java.util.*;
import java.io.*;

public class Sale {
	float total;
	float tax;
	float grandTotal;
	float payment;
	float change;
	
	String input;
	int ID;
	
	public Sale() throws IOException{
		File file = new File("receipt.txt");	//create receipt
		file.createNewFile();
		FileWriter writer = new FileWriter(file);
		
		System.out.println("Enter next Item ID.\nType 0000 to end Sale.");

		int more = 1;
		while(more == 1){
			Scanner s = new Scanner(System.in);
			input = s.next();
			ID = Integer.parseInt(input);//get item ID
			if(ID == 0){	//checks if it is the end sale code (0000) or an exitsting item
				more = 0;
			}else{
				if(ID == milk1litre.ID){
					milk1litre aMilk;
					aMilk = new milk1litre();
					total += aMilk.price;
					System.out.println(aMilk.toString() + "\tTotal:\t$" + total); //write to consul
					writer.write(aMilk.toString() + "\tTotal:\t$" + total + "\n"); //write to file
				}
				else if(ID == apple.ID){
					apple aApple;
					aApple = new apple();
					total += aApple.price;
					System.out.println(aApple.toString() + "\tTotal:\t$" + total); //write to consul
					writer.write(aApple.toString() + "\tTotal:\t$" + total + "\n"); //write to file

				}
				else if(ID == banana.ID){
					banana aBanana;
					aBanana = new banana();
					total += aBanana.price;
					System.out.println(aBanana.toString() + "\tTotal:\t$" + total); //write to consul
					writer.write(aBanana.toString() + "\tTotal:\t$" + total + "\n"); //write to file

				}
				else if(ID == orange.ID){
					orange aOrange;
					aOrange = new orange();
					total += aOrange.price;
					System.out.println(aOrange.toString() + "\tTotal:\t$" + total); //write to consul
					writer.write(aOrange.toString() + "\tTotal:\t$" + total + "\n"); //write to file
				}else{
					System.out.println("Item not found!");
				}
			}
			
		}
		tax = total * (float)0.15; //tax assumes to be 15% 
		grandTotal = tax + total;
		System.out.println("Total:\t$" + total + "\ntax:\t$" + tax + "\nGRAND TOTAL:\t$" + grandTotal); //write to file
		writer.write("Total:\t\t\t$" + total + "\ntax:\t\t\t$" + tax + "\nGRAND TOTAL:\t$" + grandTotal + "\n"); //write to file
		register();
		writer.write("payment\t\t\t$" + payment + "\n" + "change\t\t\t$" + change + "\n"); //write to file
		
		writer.flush(); //clean write buffer
	    writer.close(); //close file
	}
	
	public void register(){ //determines customer payment and change
		String inputString;
		
		System.out.println("Enter customer payment. ($x.x)");
		StringTokenizer myTokens;
		Scanner keyboard = new Scanner(System.in);
		inputString = keyboard.nextLine();
		myTokens = new StringTokenizer(inputString, "$ ");	//removes dollar sign and spaces
		payment = Float.parseFloat(myTokens.nextToken());
		System.out.println("payment\t" + payment); //write to file
		change = payment - grandTotal;
		System.out.println("change\t" + change); //write to file
		keyboard.close();

		
	}
}




