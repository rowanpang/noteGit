package helloApplet;

import java.applet.Applet;
import java.awt.Graphics;

public class HelloApplet extends Applet {
	public String s;

	public void init(){                //applet的初始化方法
		s = new String("Hello World");
	}

	public void paint(Graphics g){
		g.drawString(s,10,25);
	}
}
