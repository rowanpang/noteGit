package helloObject;

public class ObjectHello {
    
    private void paint(){
        System.out.println("helloWrold from hello object");
    }

    private void paint2(){
        System.out.println("helloWrold from hello object");
    }

    private void paint3(){
        System.out.println("helloWrold from hello object");
    }

    public static void main(String[] args){
        ObjectHello oh = new ObjectHello();
        oh.paint();
        oh.paint3();
    }
}
