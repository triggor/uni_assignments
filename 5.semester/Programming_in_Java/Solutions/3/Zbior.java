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
public class Zbior {
    private static int stacktop = 0;
    private final Para[] tablica;
    public Zbior (int n) {
        tablica = new Para[n];
    }
    /** metoda szuka pary z określonym kluczem
     * @param kl - name of the searched key
     * @return the pair with a corresponding key name*/
    public Para szukaj (String kl) {
        for (int i = 0; i < stacktop; i++){
            if (tablica[i].key.equals(kl)) return tablica[i];
        }
        return null;
    }
    /** metoda wstawia do zbioru nową parę
     * @param p - pair to be inserted into tablica[]*/
    public void wstaw (Para p) throws IllegalArgumentException {
        if (szukaj(p.key)!=null) throw new IllegalArgumentException();
        else if (stacktop+1 >= tablica.length) throw new IllegalArgumentException(); //?????????????????
        else {
            tablica[stacktop] = p;
            stacktop++;
        }
    }
    /** metoda odszukuje parę i zwraca wartość związaną z kluczem
     * @param kl - name of the searched key
     * @return value of the searched key*/
    public double czytaj (String kl) throws IllegalArgumentException {
        if (szukaj(kl)==null) throw new IllegalArgumentException();
        else return szukaj(kl).getKey();
    }
    /** metoda wstawia do zbioru nową albo aktualizuje istniejącą parę
     * @param p */
    public void ustal (Para p) throws IllegalArgumentException {
        if (szukaj(p.key)==null) wstaw(p);
        else szukaj(p.key).setKey(p.getKey());
    }
    /** metoda podaje ile par jest przechowywanych w zbiorze
     * @return number of pairs in tablica[] */
    public int ile () {
        return stacktop;
    }
    /** metoda usuwa wszystkie pary ze zbioru */
    public void czysc () {
        stacktop = 0;
    }
}
