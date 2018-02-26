package rationalnumber;

import java.math.BigInteger;
import java.util.AbstractCollection;
import java.util.Collection;
import java.util.Iterator;
import java.util.Spliterator;
import java.util.function.Consumer;
import java.util.function.Predicate;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.stream.Stream;


public class  NumberSet<T extends Number> extends AbstractCollection<T> implements Collection<T>{
    private T[] numset;
    private int size = 10;
    private int lastElem = 0;
    
     public static void main(String[] args) {
        RationalNumber rationalNumber = null;
        RationalNumber rationalNumber1 = null;
        NumberSet myNS;
        myNS = new NumberSet();
        try {
            rationalNumber1 = new RationalNumber(new BigInteger("4"), new BigInteger("-7"));
            rationalNumber = RationalNumber.add(new RationalNumber("0b-10/-1"),new RationalNumber("0x-10/-1"));
            boolean add = myNS.add(rationalNumber1);
            boolean add2 =  myNS.add(rationalNumber);
            System.out.println(add);
            System.out.println(add);
            System.out.println(myNS.size());
            boolean rmv = myNS.remove(new RationalNumber(new BigInteger("4"), new BigInteger("-7")));
            System.out.println(myNS.size());
            System.out.println(rmv);
            System.out.println(myNS);
        } catch (RationalNumber.CannotParseException ex) {
            Logger.getLogger(RationalNumber.class.getName()).log(Level.SEVERE, null, ex);
        }
        System.out.println(rationalNumber);
        System.out.println(rationalNumber1);
    }

    public NumberSet() {
        numset = (T[]) new Number[size];//czemu nie moge napisac `new T[n]`???????????????
    }
    
    @Override
    public Iterator<T> iterator() {
        Iterator<T> it = new Iterator<T>() {

            private int currentIndex = 0;

            @Override
            public boolean hasNext() {
                return currentIndex < lastElem && numset[currentIndex] != null;
            }

            @Override
            public T next() {
                return numset[currentIndex++];
            }

            @Override
            public void remove() {
                throw new UnsupportedOperationException();
            }
        };
        return it;
    }

    @Override
    public int size() {
        return lastElem;
    }

    @Override
    public boolean removeIf(Predicate prdct) {
        return super.removeIf(prdct); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public Spliterator spliterator() {
        return super.spliterator(); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public Stream stream() {
        return super.stream(); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public Stream parallelStream() {
        return super.parallelStream(); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public void forEach(Consumer cnsmr) {
        super.forEach(cnsmr); //To change body of generated methods, choose Tools | Templates.
    }
    
    @Override
    public boolean add(T t){
        if(!this.contains(t)){
            if(lastElem < size){
                lastElem++;
                numset[lastElem-1] = t;
            }
            else{
                size*=2;
                T[] newnumset = (T[]) new Number[size];
                System.arraycopy(numset, 0, newnumset, 0, size);
                numset = newnumset;
            }
            return true;
        }
        return false;
    }
    
    @Override
    public boolean addAll(Collection t){
        T[] ttt = (T[]) t.toArray(); 
        int count = 0;
        for(T val : ttt){
            if(this.add(val)) count++;
        }
        return count>0;
    }
    
    @Override
    public boolean contains(Object o){
        T t = (T) o;
        for(T val : numset){
            if(t==null) return val==null;
            else return t.equals(val);
        }
        return false;
    }
    
    @Override
    public boolean remove (Object o){
        T t = (T) o;
        for(int i=0; i<lastElem; i++){
            if(t.equals(numset[i])){
                numset[i]=numset[lastElem-1];
                lastElem--;
                return true;
            }
        }
        return false;
    }
    
    @Override
    public void clear(){
        for(T val : numset){//variable `val` is not used???????????????????????????????????
            val=null;
        } 
        lastElem=0;
        T[] newnumset = (T[]) new Number[10];
        numset = newnumset;
    }

    @Override
    public String toString(){
        //String res = numset[0].toString() + " ||| " + numset[1].toString();//"";
        String res = "";
        
        for(T val : numset){
            res += val.toString();
            res += " ";
        }
        
        return res;
    }
    
    @Override
    public boolean isEmpty(){
        return lastElem == 0;
    }      
    
}
