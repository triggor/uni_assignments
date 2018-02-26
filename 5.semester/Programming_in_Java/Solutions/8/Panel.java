package mikolaj;

import java.awt.Graphics;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.JPanel;


public class Panel extends JPanel{
    public static final int FIELDSIZE = 50;
    private Santa santa;
    protected Frame frame;
    final Child[] children;
    
    

    public Panel(Frame frame)
     {
        this.frame = frame;
        this.addKeyListener(upKeyListener);
        this.addKeyListener(downKeyListener);
        this.addKeyListener(leftKeyListener);
        this.addKeyListener(rightKeyListener);
        this.addKeyListener(giftKeyListener);
        this.addKeyListener(exitKeyListener);
        santa = new Santa(this);  
        children = new Child[frame.mikolaj.kidsNum];
        for(int i=0; i<frame.mikolaj.kidsNum; i++){
            children[i] = new Child(this, i, santa);
        }
        
    }

    public KeyListener upKeyListener = new KeyAdapter(){
 
        @Override
        public void keyPressed(KeyEvent e){
            if (e.getKeyCode() == KeyEvent.VK_W){
                santa.move("UP");
            }
        }
    };
    public KeyListener downKeyListener = new KeyAdapter(){
        @Override
        public void keyPressed(KeyEvent e){
            if (e.getKeyCode() == KeyEvent.VK_S){
                santa.move("DOWN");
            }
        }
    };
    public KeyListener leftKeyListener = new KeyAdapter(){
        @Override
        public void keyPressed(KeyEvent e){
            if (e.getKeyCode() == KeyEvent.VK_A){
                santa.move("LEFT");
            }
        }
    };
    public KeyListener rightKeyListener = new KeyAdapter(){
        @Override
        public void keyPressed(KeyEvent e){
            if (e.getKeyCode() == KeyEvent.VK_D){
                santa.move("RIGHT");
            }
        }
    };
    public KeyListener exitKeyListener = new KeyAdapter(){
    @Override
        public void keyPressed(KeyEvent e){
            if (e.getKeyCode() == KeyEvent.VK_ESCAPE){
                System.exit(0);//czemu nie dziala po pojawianiu sie komunikatu JLabel???????????????????????
            }
        }
    };
    public KeyListener giftKeyListener = new KeyAdapter(){
        @Override
        public void keyPressed(KeyEvent e){
            if (e.getKeyCode() == KeyEvent.VK_SPACE){
                frame.mikolaj.setGift(santa.x, santa.y);//Santa standing and Gift is set
                //System.out.println("Gift is set at " + santa.x + ", " + santa.y);
            }
        }
    };
    @Override
    public void paintComponent(Graphics g){
       super.paintComponent(g);
       drawSanta(g);
       drawKids(g);
       drawGifts(g);
    }
    private void drawSanta(Graphics g){
        g.drawImage(santa.santaImage, santa.x*FIELDSIZE, santa.y*FIELDSIZE, FIELDSIZE, FIELDSIZE, this);
    }
    
    private void drawKids(Graphics g){
        for(int i=0; i<Mikolaj.N; i++){
            for(int j=0; j<Mikolaj.M; j++){
                if(frame.mikolaj.checkAwakeChild(j,i)){//Child is standing
                    g.drawImage(Child.childImage, j*FIELDSIZE, i*FIELDSIZE, FIELDSIZE, FIELDSIZE, this);
                }
                else if(frame.mikolaj.checkSleepChild(j,i)){//Child is sleeping
                    g.drawImage(Child.sleepImage, j*FIELDSIZE, i*FIELDSIZE, FIELDSIZE, FIELDSIZE, this);
                }
                else if(frame.mikolaj.checkHappyChild(j,i)){//Child is happy
                    g.drawImage(Child.happyImage, j*FIELDSIZE, i*FIELDSIZE, FIELDSIZE, FIELDSIZE, this);
                }
                else if(frame.mikolaj.checkJumpingChild(j,i)){//Child is happy
                    g.drawImage(Child.childImage, j*FIELDSIZE, i*FIELDSIZE-10, FIELDSIZE, FIELDSIZE, this);
                }
            }
        }
    }

    
    private void drawGifts(Graphics g){
        for(int i=0; i<Mikolaj.N; i++){
            for(int j=0; j<Mikolaj.M; j++){
                if(frame.mikolaj.checkGift(j, i)){//Gift is set
                    g.drawImage(santa.giftImage, j*FIELDSIZE, i*FIELDSIZE, FIELDSIZE, FIELDSIZE, this);
                }
            }
        }
    }
    
}

