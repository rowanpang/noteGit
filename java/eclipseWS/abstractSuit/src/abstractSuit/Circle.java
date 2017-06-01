package abstractSuit;

public class Circle extends Shape {
    
    private double radius;
    public Circle(double r){
	this.radius = r;
    }

    @Override
    public double area() {
	return Math.PI*radius*radius;
    }

}
