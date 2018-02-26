package mikolaj;

import java.awt.Image;
import java.io.File;
import java.io.IOException;
import java.io.Serializable;
import javax.imageio.ImageIO;


public class Santa extends Thread implements Serializable{
    public int x;
    public int y;
    public Image santaImage;
    public Image giftImage;
    private Panel panel;
            
    public Santa(Panel panel){
        this.panel = panel;
        x = Mikolaj.N/2;
        y = Mikolaj.M/2;
        panel.frame.mikolaj.setSanta(x, y);//Santa is standing
        try {
            santaImage=ImageIO.read(new File(".\\src\\santa-claus.png"));
            giftImage=ImageIO.read(new File(".\\src\\gift.png"));
        } 
        catch (IOException e){}
    }
    public void move(String s){
        panel.frame.mikolaj.removeSanta(x, y);//Santa isn't standing anymore
        switch(s){
            case "UP":
                if(y==0) y=Mikolaj.N-1;
                else y--;
                break;
            case "DOWN":
                if(y==Mikolaj.N-1) y=0;
                else y++;
                break;
            case "LEFT":
                if(x==0) x=Mikolaj.M-1;
                else x--;
                break;
            case "RIGHT":
                if(x==Mikolaj.M-1) x=0;
                else x++;
                break;
        }
        panel.frame.mikolaj.setSanta(x, y);//Santa is standing
    }
}
