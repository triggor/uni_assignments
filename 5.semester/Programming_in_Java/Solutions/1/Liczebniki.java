package liczebniki;

/**
 * Author: Igor Tryhub, 275235
 */
public class Liczebniki {
    public static String[] jed_glowne = {"zero", "one", "two", "three","four","five",
                                        "six","seven","eight","nine"};
    public static String[] jed_porz = {"zeroth", "first", "second","third",
                                        "fourth","fifth","sixth","seventh",
                                        "eighth","ninth"};
    public static String[] nast_glowne = {"ten", "eleven", "twelve","thirteen",
                                        "fourteen","fifteen","sixteen","seventeen",
                                        "eighteen","nine"};
    public static String[] nast_porz= {"tenth", "eleventh", "twelfth",
                                        "thirteenth","fourteenth","fifteenth",
                                        "sixteenth","seventeenth","eighteenth",
                                        "nineteenth"};
    public static String[] dzies_glowne = {"zero", "ten", "twenty","thirty","forty",
                                        "fifty","sixty","seventy","eighty","ninety"};
    public static String[] dzies_porz = {"zeroth", "tenth", "twentieth",
                                         "thirtieth","fortieth","fiftieth",
                                         "sixtieth","seventieth","eightieth",
                                         "ninetieth"};
    public static String[] setki_glowne = {"zero", "one hundred", "two hundred", 
                                        "three hundred","four hundred",
                                        "five hundred","six hundred","seven hundred",
                                        "eight hundred","nine hundred"};
    public static String[] setki_porz = {"zeroth", "one hundredth",
                                         "two hundredth", "three hundredth",
                                         "four hundredth","five hundredth",
                                         "six hundredth","seven hundredth",
                                         "eight hundredth","nine hundredth"};
    
    public static void main(String[] args) {
        int[] argArray = new int[args.length];
        int count = 0;
        for (String s: args){
            int x = new Integer(s);
            if (!(x>=0))
                throw new IllegalArgumentException("liczba ujemna " + s + " jest błędem");
            argArray[count] = x;
            count++;
        }
        for (int x: argArray){
            String result = convert(x);
            System.out.println(result);
        }
    }
    private static String convert(int x){ //non-static method convert() cannot be referenced from a static context???
        int y = x;
        String res = "";
        if (x==0) return " " + jed_porz[0] + " (" + y + "th)";//zero
        int position = 0;
        int mask = 1;
        while(x>0){
            ++position;
            mask*=10;
            x/=10;
        }
        mask/=10;
        x=y;
        
        while(position>0){
            if(position==1){//jedna cyfra
                res+= " " + jed_porz[x/mask];
                position--;
            }
            else if(position==2 && x/mask==1){//nastki
                res+= " " + nast_porz[x%mask];
                position--;
                position--;
                mask/=10;
            }
            else if(position==2 && x%mask==0){//dziestki
                res += " " + dzies_porz[x/mask];
                position--;
                position--;
                mask/=10;
            }
            else if(position==2 && x%mask!=0){//dziestki+cyfra
                res += " " + dzies_glowne[x/mask];
                position--;
                x=x%mask;
                mask/=10;
            }
            else if(position==3 && x%mask==0){//setki
                res += " " + setki_porz[x/mask];
                position--;
                position--;
                position--;
                x=x%mask;
                mask/=100;
            }
            else if(position==3 && x%mask!=0){//setki + cos
                res += " " + setki_glowne[x/mask];
                position--;
                x=x%mask;
                mask/=10;
            }
            else if(position==4 && x%mask==0){//setki
                res += " " + jed_glowne[x/mask];
                res += " thousandth";
                position--;
                position--;
                position--;
                position--;
                x=x%mask;
                mask/=1000;
            }
            else if(position==4 && x%mask!=0){//setki + cos
                res += " " + jed_glowne[x/mask];
                res += " thousand";
                position--;
                x=x%mask;
                mask/=10;
            }
           
        }
       
        return res + " (" + y + "th)";
    }
    
}
