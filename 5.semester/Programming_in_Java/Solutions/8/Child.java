package mikolaj;

import java.awt.Image;
import java.io.File;
import java.io.IOException;
import java.io.Serializable;
import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;


public class Child extends Thread implements Serializable{
    private int x;
    private int y;
    private final Santa mysanta;
    private Boolean happy = false;
    public static Image childImage = null;
    public static Image sleepImage = null;
    public static Image happyImage = null;
    private final Panel panel;
    Random generator = new Random();
  
    public  Child(Panel panel, int i, Santa santa){
        try {
           childImage = ImageIO.read(new File(".\\src\\kid-icon.png"));
           sleepImage = ImageIO.read(new File(".\\src\\kid-sleep.png"));
           happyImage = ImageIO.read(new File(".\\src\\kid-happy.png"));
        } 
        catch (IOException e){System.out.println("Child image not found");}
        this.panel = panel;
        mysanta = santa;
        do{
            x = generator.nextInt(Mikolaj.N);
        }while(Mikolaj.N/2-1<=x && x<=Mikolaj.N/2+1);
        do{
            y = generator.nextInt(Mikolaj.M);
        }while(Mikolaj.M/2-1<=x && x<=Mikolaj.M/2+1);
        panel.frame.mikolaj.setChild(x, y);//Child is standing
    }

    @Override
    public void run() {
        int rested = generator.nextInt(5000)+5000;
        for (int i=0; i<10; i++){
            if(i == 7){
                followSanta();
            }
            lookAround();
            if(happy) return;
            try {
                Child.sleep(rested/10);
            } catch (InterruptedException ex) {
                Logger.getLogger(Child.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        int tired = generator.nextInt(5000)+2000;
        panel.frame.mikolaj.fallAsleepChild(x, y);
        try {
            Child.sleep(tired);//czemu NetBeans krzyczy kiedy pisze mu this.sleep()????????????????
        } catch (InterruptedException ex) {
            Logger.getLogger(Child.class.getName()).log(Level.SEVERE, null, ex);
        }
        panel.frame.mikolaj.wakeChild(x, y);
        run();
    }
    
    private void jump() throws InterruptedException{
        //System.out.println("JUMPING...");
        sleep(100);
        panel.frame.mikolaj.jumpUpChild(x, y);
        sleep(100);
        panel.frame.mikolaj.jumpDownChild(x, y);
    }
    
    private void move(int deltaX, int deltaY, Boolean invertX, Boolean invertY) {
        panel.frame.mikolaj.removeChild(x, y);//Child isn't standing anymore
        if(invertX){
            if(x==0 && deltaX>0) x=Mikolaj.M-1;
            if(x==Mikolaj.M-1 && deltaX<0) x=0;
            else x -= deltaX;
        }
        else{ 
            if(x==Mikolaj.M-1 && deltaX>0) x=0;
            if(x==0 && deltaX<0) x=Mikolaj.M-1;
                x += deltaX;
        }
        if(invertY){
            if(y==0 && deltaY>0) y=Mikolaj.N-1;
            if(y==Mikolaj.N-1 && deltaY<0) y=0;
            else y-=deltaY;
        }
        else {
            if(y==Mikolaj.N-1 && deltaY>0) y=0;
            if(y==0 && deltaY<0) y=Mikolaj.N-1;
            else y += deltaY;
        }
        panel.frame.mikolaj.setChild(x, y);//Child is standing in a new field
    }
    
    private void lookAround(){
        int left = x-1;
        if(x==0) left=Mikolaj.N-1;
        int right = x+1;
        if(x==Mikolaj.N-1) right=0;
        int up = y-1;
        if(y==0) up=Mikolaj.M-1;
        int down = y+1;
        if(y==Mikolaj.M-1) down=0;

        if(panel.frame.mikolaj.checkSanta(left, up) ||
           panel.frame.mikolaj.checkSanta(x, up) ||     
           panel.frame.mikolaj.checkSanta(right, up) ||     
           panel.frame.mikolaj.checkSanta(left, y) ||
           panel.frame.mikolaj.checkSanta(x, y) ||
           panel.frame.mikolaj.checkSanta(right, y) ||
           panel.frame.mikolaj.checkSanta(left, down) ||
           panel.frame.mikolaj.checkSanta(x, down) ||
           panel.frame.mikolaj.checkSanta(right, down)){
            System.out.println("Lost: santa disclosed");
            panel.frame.mikolaj.gameLost = true;
        }

        else if(panel.frame.mikolaj.checkGift(left, up) ||
           panel.frame.mikolaj.checkGift(x, up) ||     
           panel.frame.mikolaj.checkGift(right, up) ||     
           panel.frame.mikolaj.checkGift(left, y) ||
           panel.frame.mikolaj.checkGift(x, y) ||
           panel.frame.mikolaj.checkGift(right, y) ||
           panel.frame.mikolaj.checkGift(left, down) ||
           panel.frame.mikolaj.checkGift(x, down) ||
           panel.frame.mikolaj.checkGift(right, down)){
           for(int j=0; j<5; j++){
               try {
                   jump();
               } catch (InterruptedException ex) {
                   Logger.getLogger(Child.class.getName()).log(Level.SEVERE, null, ex);
               }
           } 
           panel.frame.mikolaj.makeHappyChild(x, y);
           happy = true;
        }
    }
    
    private void followSanta() {
        int xDiff = Math.abs(mysanta.x - x);
        int yDiff = Math.abs(mysanta.y - y);
        Boolean invertX = xDiff>10;
        Boolean invertY = yDiff>10;
        if(xDiff == 0 && mysanta.y > y) move (0, 1, invertX, invertY);
        else if(xDiff == 0 && mysanta.y < y) move (0, -1, invertX, invertY);
        else if(yDiff == 0 && mysanta.x > x) move (1, 0, invertX, invertY);
        else if(yDiff == 0 && mysanta.x < x) move (-1, 0, invertX, invertY);
        else if(xDiff/yDiff >= 3){ //we want to move only along X-axis
            if(mysanta.x > x) move(1, 0, invertX, invertY); //move right
            else move(-1, 0, invertX, invertY); //move left
        }
        else if(yDiff/xDiff >= 3){ //we want to move only along Y-axis
            if(mysanta.y > y) move(0, 1, invertX, invertY); //move down
            else move(0, -1, invertX, invertY); //move up
        }
        else { //we want to move both along X-axis and Y-axis
            if(mysanta.x > x){
                if(mysanta.y > y) move(1, 1, invertX, invertY);
                else move(1, -1, invertX, invertY);
            }
            else{
                if(mysanta.y > y) move(-1, 1, invertX, invertY);
                else move(-1, -1, invertX, invertY);
            }
        }
    }
    
}
