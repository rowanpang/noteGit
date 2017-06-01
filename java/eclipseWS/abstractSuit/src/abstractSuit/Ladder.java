package abstractSuit;

public class Ladder extends Shape {
    private double a,b,h;

    public Ladder(double a, double b, double h) {
	this.a = a;
	this.b = b;
	this.h = h;
    }

    @Override
    public double area() {
	return ((a+b)*h/2);
    }

}
