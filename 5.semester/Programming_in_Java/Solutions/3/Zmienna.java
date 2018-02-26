/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package exptrees;

/**
 *
 * @author igor
 */
final class Zmienna extends Wyrazenie{
    static protected final Zbior zmienne = new Zbior(100);
    private final Para p;
    public Zmienna (String k, double v){
        p = new Para(k, v);
        zmienne.wstaw(p);
    }
    public Zmienna (String k){
        p = new Para(k, 0);
        zmienne.wstaw(p);
    }
    @Override public double oblicz(){
        return zmienne.czytaj(p.key);
    }
    @Override public String toString(){
        return "(" + p.key + ")";
    }
    @Override public boolean equals(Object o) {
        if (o instanceof Wyrazenie){
            return p.getKey()==((Wyrazenie)o).oblicz();
        }
        else return false;
    }
}
