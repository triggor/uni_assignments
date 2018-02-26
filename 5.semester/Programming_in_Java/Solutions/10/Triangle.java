/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package streamprocessor;

/**
 *
 * @author igort
 */
public class Triangle {
    public final boolean exists; //po co nam ta zmienna???????????????????????????????????????????????????????
    private final double b1;
    private final double b2;
    private final double b3;
    
    public Triangle(double a, double b, double c) throws IllegalArgumentException{
        if(a<=0 || b<=0 || c<=0){
            exists = false;
            throw new IllegalArgumentException ("Negative side length: "  + a + ", " + b + ", " + c);
        }
        if(a+b<c || a+c<b || b+c<a){
            exists = false;
            throw new IllegalArgumentException ("Triangle " + a + ", " + b + ", " + c + " doesn't exist");
        }
        exists = true;
        b1=a;
        b2=b;
        b3=c;
    }
    
    public double perimeter(){
        return b1 + b2 + b3;
    }
    
    public boolean isRight(){
        if(b1 == Math.max(b1, Math.max(b2, b3))){
            return b1*b1 == b2*b2 + b3*b3;
        }
        else if(b2 == Math.max(b1, Math.max(b2, b3))){
            return b2*b2 == b1*b1 + b3*b3;
        }
        else {
            return b3*b3 == b1*b1 + b2*b2;
        }
    }
    
    public double area(){
        double p = (b1+b2+b3) / 2;
        return Math.sqrt(p * (p-b1) * (p-b2) * (p-b3));
    }
    
    @Override
    public String toString(){
        return("<"+b1+", "+b2+", "+b3+">");
    }
    
}
