package aPackage;

public class item{ //generic item class, with description price and ID
	String description;
	float price;
	int ID;
	
	public item(String description, float price, int ID){
		this.description = description;
		this.price = price;
		this.ID = ID;
	}
	
	public String toString(){
		return(ID + "\t" + description + "\t$" + price );
	}
}
