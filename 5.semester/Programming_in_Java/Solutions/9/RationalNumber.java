package rationalnumber;

import java.math.BigInteger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 *
 * @author Igor Tryhub, 275235
 */
public class RationalNumber extends Number implements Comparable<RationalNumber>{

    public static enum NumSystem {
        BINARY ("0b", 2), 
        DECIMAL ("", 10), 
        HEXADECIMAL("0x", 16);
        
        public final String prefix;
        public final int base;
        NumSystem(String prefix, int base){
            this.prefix = prefix;
            this.base = base;
        }
    }
    
    public final BigInteger numerator; //licznik
    public final BigInteger denominator; //mianownik
    
    private final NumSystem numSys; 
    
    public RationalNumber(BigInteger numerator, BigInteger denominator){
        numSys = NumSystem.DECIMAL;
        if(denominator.compareTo(BigInteger.ZERO)==0)
            throw new IllegalArgumentException("Division by zero");
        //normalize(numerator, denominator);//jak przekazac przez wskaznik, jak w C++?????????????????????????????
        boolean positive = numerator.compareTo(BigInteger.ZERO)==1 && denominator.compareTo(BigInteger.ZERO)==1
                || numerator.compareTo(BigInteger.ZERO)==-1 && denominator.compareTo(BigInteger.ZERO)==-1;
        this.numerator = positive ? numerator.divide(gcd(numerator, denominator)).abs() : numerator.divide(gcd(numerator, denominator)).abs().negate();
        this.denominator = denominator.divide(gcd(numerator, denominator)).abs();
    }
    
    public RationalNumber(String ratnumStr) throws CannotParseException{
        BigInteger num = null; 
        BigInteger denom = null; 
        NumSystem numsys = null; 
        boolean positive;
        String pattern = "((^(0x)(-?[0-9A-Fa-f]+)/(-?[0-9A-Fa-f]+)$)|(^(0b)(-?[0-1]+)/(-?[0-1]+)$)|(^(-?\\d+)/(-?\\d+)$))";//"(^(0x|0b)?(\\d+)/(\\d+)$)";
        Pattern r = Pattern.compile(pattern);
        Matcher m = r.matcher(ratnumStr);

        if (m.find( )) {
            if(m.group(2)!=null){
                numsys = NumSystem.HEXADECIMAL;
                num = new BigInteger(m.group(4), NumSystem.HEXADECIMAL.base);
                denom = new BigInteger(m.group(5), NumSystem.HEXADECIMAL.base);
            }
            else if(m.group(6)!=null){
                numsys = NumSystem.BINARY;
                num = new BigInteger(m.group(8), NumSystem.BINARY.base);
                denom = new BigInteger(m.group(9), NumSystem.BINARY.base);
            }
            else if(m.group(10)!=null){
                numsys = NumSystem.DECIMAL;
                num = new BigInteger(m.group(11));
                denom = new BigInteger(m.group(12));
            }
        }
        else {
            throw new CannotParseException("ERROR: cannot parse " + ratnumStr);
            //System.err.println("ERROR: cannot parse " + ratnumStr);
            //System.exit(1);
        }
        if(denom.compareTo(BigInteger.ZERO)==0)//dereferencing null pointer possible?????????????????????????????????????????????
            throw new IllegalArgumentException("Division by zero");
        
        positive = num.compareTo(BigInteger.ZERO)==1 && denom.compareTo(BigInteger.ZERO)==1
                || num.compareTo(BigInteger.ZERO)==-1 && denom.compareTo(BigInteger.ZERO)==-1;
        numerator = positive ? num.divide(gcd(num, denom)).abs() : num.divide(gcd(num, denom)).abs().negate();
        denominator = denom.divide(gcd(num, denom)).abs();
        numSys = numsys;
        
        System.out.println("numSys="+numSys);
        System.out.println("numerator="+numerator);
        System.out.println("denominator="+denominator);
    }
    
    @Override
    public int intValue() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public long longValue() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public float floatValue() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public double doubleValue() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public int compareTo(RationalNumber t) {
        return numerator.multiply(t.denominator).compareTo(t.numerator.multiply(denominator));
    }
    
    @Override
    public String toString(){
        switch (numSys) {
            case HEXADECIMAL:
                return "0x" + numerator.toString(NumSystem.HEXADECIMAL.base) + "/" + denominator.toString(NumSystem.HEXADECIMAL.base);
            case BINARY:
                return "0b" + numerator.toString(NumSystem.BINARY.base) + "/" + denominator.toString(NumSystem.BINARY.base);
            case DECIMAL:
                return numerator.toString() + "/" + denominator.toString();
        }
        System.err.println("Something went wrong...");
        return null;
    }
    
    private BigInteger gcd(BigInteger a, BigInteger b){ // NWD
        return a.gcd(b);
    }
    
    public static RationalNumber add(RationalNumber a, RationalNumber b){
        BigInteger denom = a.denominator.multiply(b.denominator);
        BigInteger num = a.numerator.multiply(b.denominator).add(b.numerator.multiply(a.denominator));
        return new RationalNumber(num, denom); 
    }
    
    public static RationalNumber subtract(RationalNumber a, RationalNumber b){
        BigInteger denom = a.denominator.multiply(b.denominator);
        BigInteger num = a.numerator.multiply(b.numerator);
        return new RationalNumber(num, denom); 
    }
    
    public static RationalNumber multiply(RationalNumber a, RationalNumber b){
        BigInteger denom = a.denominator.multiply(b.denominator);
        BigInteger num = a.numerator.multiply(b.numerator);
        return new RationalNumber(num, denom); 
    }
    
    public static RationalNumber reciprocal(RationalNumber a){
        return new RationalNumber(a.denominator, a.numerator); 
    }
    
    public static RationalNumber divide(RationalNumber a, RationalNumber b){
        return multiply(a, reciprocal(b));
    }
    
    public static RationalNumber mod(RationalNumber a, RationalNumber b) throws Exception{
        if(a.denominator.equals(BigInteger.ONE) && b.denominator.equals(BigInteger.ONE)){
            return new RationalNumber(a.numerator.mod(b.numerator), BigInteger.ONE); 
        }
        else{
            throw new NotIntegerException("NotIntegerException");
        }
    }
    
    public static RationalNumber negate(RationalNumber a){
        return new RationalNumber(a.numerator.negate(), a.denominator); 
    }
    
    public static RationalNumber abs(RationalNumber a){
        return new RationalNumber(a.numerator.abs(), a.denominator); 
    }
    
    public static RationalNumber ceiling(RationalNumber a){
        if(a.denominator.compareTo(BigInteger.ONE) == 0){
            return a;
        }
        else return new RationalNumber(a.numerator.divide(a.denominator).add(BigInteger.ONE), BigInteger.ONE);
    }
    
    public static RationalNumber floor(RationalNumber a){
        if(a.denominator.compareTo(BigInteger.ONE) == 0){
            return a;
        }
        else return new RationalNumber(a.numerator.divide(a.denominator).subtract(BigInteger.ONE), BigInteger.ONE);
    }
    
    public static class NotIntegerException extends Exception{
        public NotIntegerException(String message) { super(message); }
    }
    
    public static class CannotParseException extends Exception{
        public CannotParseException(String message) { super(message); }
    }

}
