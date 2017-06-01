package abstractSuit;

public class Box extends Shape {
    private double length,width;
    
    public Box(double l,double w){
	this.length = l;
	this.width = w;
    }

    public double area(){
	return length*width;
    }

}
