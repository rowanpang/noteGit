package helloApplet;

import java.applet.Applet;
import java.awt.Graphics;

public class HelloApplet extends Applet {

    /**
     * 
     */
    private static final long serialVersionUID = -3362585916510046503L;
    public String s;

    public void init(){                //applet的初始化方法
	s = new String("Hello World");
    }

    public void paint(Graphics g){
	do_paint(g);
    }

    public void do_paint(Graphics g){
	g.drawString(this.s,10,25);
    }
}
