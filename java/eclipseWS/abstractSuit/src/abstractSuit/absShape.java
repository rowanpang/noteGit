package abstractSuit;

public class absShape {
    public static void main(String[] args){
	Box b = new Box(1.2,1.2);
	System.out.println("Box area: " + b.area());

	Circle c = new Circle(2);
	System.out.println("Box area: " + c.area());

	Ladder l = new Ladder(1,2,4);
	System.out.println("Box area: " + l.area());
    }

}
