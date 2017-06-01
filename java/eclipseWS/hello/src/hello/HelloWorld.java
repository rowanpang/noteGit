package hello;

public class HelloWorld {
    public static void main(String[] args){
        System.out.print("hello wrold\n");

        System.out.print("args are:\n");
        for(int i=0;i<args.length;i++){
            System.out.println(args[i]);
        }
    }

}
