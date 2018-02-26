/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package exptrees;


/**
 *
 * @author Igor Tryhub, 275235
 */
public class ExpTrees {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        
        Para k1 = new Para("key1", 123.45);
        Para k2 = new Para("key2", 123.45);
        Para k3 = new Para("key1", 111.45);
        System.out.println(k1.equals(k3));
        System.out.println(k1.equals(k2));
        k1.setKey(234);
        System.out.println(k1.equals(k3));
        System.out.println(k1.getKey());
        System.out.println(k1.toString());
        Zbior z1 = new Zbior(10);
        System.out.println(z1.ile());
        z1.wstaw(k3);
        System.out.println(z1.ile());
        System.out.println(z1.szukaj("key1"));
        System.out.println(z1.szukaj("heha"));
        z1.ustal(k1);
        System.out.println(z1.szukaj("key1"));
        z1.ustal(k2);
        System.out.println(z1.szukaj("key2"));
        System.out.println(z1.ile());
        z1.czysc();
        System.out.println(z1.ile());
        System.out.println(z1.szukaj("key1"));
        Wyrazenie w1 = new Odwrotnosc(new Odwrotnosc(new Stala(5)));
        System.out.println(w1.oblicz());
        Zmienna zm = new Zmienna("zm", 3.1415/2);
        Wyrazenie w2 = new Odwrotnosc(new Sinus(zm));
        System.out.println(w2.oblicz());
        Zmienna zm2 = new Zmienna("zm2", 0.7);
        System.out.println(zm2.oblicz());
        Wyrazenie w3 = new Dodawanie (w2,w1);
        System.out.println(w3.oblicz());
        Wyrazenie w4 = new Dzielenie (w2,w2);
        w2 = new Mnozenie (w2, w3);
        System.out.println(w4.oblicz());
        System.out.println(w2.toString() + "=" + w2.oblicz());
        System.out.println(Wyrazenie.pomnoz(new Wyrazenie[] {w1,w2,w3}));
        
        /*
        x=-1,618
        3+5
        2+x*7
        (3*11-1)/(7+5)
        arctan(((x+13)*x)/2)
        pow(2,5)+x*log(2,y)
        */
    }
    
}
