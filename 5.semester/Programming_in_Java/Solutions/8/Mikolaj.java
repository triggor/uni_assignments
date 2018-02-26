package mikolaj;

import java.io.Serializable;
import java.util.Random;

/**
 * @author Igor Tryhub, 275235
 */
public class Mikolaj implements Serializable{
    final Frame frame;
    protected Boolean gameWon = false;
    protected Boolean gameLost = false; //jak zastopowac wszystkie watki???????????????
    protected static final int N = 20;
    protected static final int M = 20;
    protected int kidsNum;
    protected int happyKidsNum = 0;
    protected int giftsLeft;
    private final int[][] state; //volatile????????????????????????????????????????
    Random generator = new Random(); 
    
    public static void main(String[] args) {
        new Mikolaj();
    }
    public Mikolaj(){
        this.state = new int[N][M];
        kidsNum = generator.nextInt(5) + 5;
        giftsLeft = kidsNum+1;
        System.out.println("KidsNUM == " + kidsNum);
        System.out.println("giftsLeft == " + giftsLeft);
        for(int i=0; i<N; i++){
            for(int j=0; j<M; j++){
                state[i][j] = 0;//Noone is standing
            }
        }
        frame = new Frame(this);
    }
    /*
    public Mikolaj(Mikolaj saved){
        this = saved; //cannot assign a value to final variable this??????????????????????????????
        //moze sklonowac??????????????????????????????????????????????????????????????????????????
    }
    */

    public synchronized int getState(int x, int y){
        return state[x][y];
    }
    public synchronized void setSanta(int x, int y){
        if(state[x][y] < 100) state[x][y] += 100;
    }
    public synchronized void removeSanta(int x, int y){
        if(checkSanta(x ,y)) state[x][y] -= 100;
    }
    public synchronized Boolean checkSanta(int x, int y){
        return (state[x][y] >= 100);
    }
    public synchronized void setChild(int x, int y){
        if((state[x][y]%100)/20==0) state[x][y] += 20;
    }
    public synchronized void fallAsleepChild(int x, int y){
        if(checkAwakeChild(x ,y)) state[x][y] += 20;
    }
    public synchronized void makeHappyChild(int x, int y){
        if(checkAwakeChild(x ,y)) state[x][y] += 40;
        happyKidsNum++;
        if(kidsNum == happyKidsNum){
            System.out.println("Won: all kids are happy");
            gameWon = true;
        }
    }
    public synchronized void jumpUpChild(int x, int y){
        if(checkAwakeChild(x ,y)) state[x][y] += 60;
    }
    public synchronized void jumpDownChild(int x, int y){
        if(checkJumpingChild(x ,y)) state[x][y] -= 60;
    }
    public synchronized void wakeChild(int x, int y){
        if(checkSleepChild(x ,y)) state[x][y] -= 20;
     }
    public synchronized void removeChild(int x, int y){
        if(checkAwakeChild(x ,y)) state[x][y] -= 20;
    }
    public synchronized Boolean checkAwakeChild(int x, int y){
        return ((state[x][y]%100)/20==1);
    }
    public synchronized Boolean checkSleepChild(int x, int y){
        return ((state[x][y]%100)/20==2);
    }
    public synchronized Boolean checkHappyChild(int x, int y){
        return ((state[x][y]%100)/20==3);
    }
    public synchronized Boolean checkJumpingChild(int x, int y){
        return ((state[x][y]%100)/20==4);
    }
    public synchronized void setGift(int x, int y){
        if(giftsLeft>0){
            if((state[x][y]%10)/3==0){
                state[x][y] += 3;
                giftsLeft--;
            }
        }
    }
    public synchronized void removeGift(int x, int y){
        if(checkGift(x, y)) state[x][y] -= 3;
    }
    public synchronized Boolean checkGift(int x, int y){
        return ((state[x][y]%10)/3==1);
    } 
}

