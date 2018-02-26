package streamprocessor;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;
import java.util.function.Predicate;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

/**
 *
 * @author Igor Tryhub, 275235
 */
public class StreamProcessor {

    public static void main(String[] args) {
        System.out.println("ZADANIE 1:");
        zadanie_1();
        System.out.println("\nZADANIE 2:");
        zadanie_2();
    }
    
    private static void zadanie_1(){
        try (BufferedReader br = new BufferedReader(new FileReader("." + File.separator + "src" + File.separator + "dane11.txt"))) {
            String pattern = "(^\\s*([1-9]\\d{0,8})?\\s*((\\/\\/)+.*)?$)";
            Pattern r = Pattern.compile(pattern);
            Matcher m;
            List<Integer> parsed = new ArrayList<Integer>();

            for (String ln = br.readLine(); ln != null; ln = br.readLine()) {
                m = r.matcher(ln);
                if(m.find()){
                    if(m.group(2)!=null){
                        parsed.add(new Integer(m.group(2)));
                    }
                }
                else throw new Exception("Wrong file data format because of '" + ln + "'");
            }
            for(Integer i : parsed){
                System.out.println(i);
            }

            List<Integer> orderedList = new ArrayList<>();            
            System.out.print("Here are the collection elements in ascending order: ");            
            orderedList = 
                parsed.stream()
                .sorted()
                .collect(Collectors.toList());
            System.out.println(orderedList);
            
            List<Integer> primes = /*new ArrayList<>();
            primes =*/ parsed.stream()
                .filter(b -> isPrime(b) == true )
                .collect(Collectors.toList());
            System.out.println("These are the prime numbers: " + primes);
            
            Predicate<Integer> over1000 = num -> num > 1000;
            int greater_1000 = parsed.stream()
                .filter(over1000)
                .mapToInt(w -> w)
                .sum();
            System.out.println("Sum of numbers greater than 1000: " + greater_1000);
             
        }
        catch (Exception ex) { 
                System.err.println("Error:" + ex);
        }
    }
    
    private static void zadanie_2(){
        try (BufferedReader br = new BufferedReader(new FileReader("." + File.separator + "src" + File.separator + "dane2.txt"))) {
            String pattern = "(^\\s*((([1-9]\\d{0,8})(\\.\\d+)?\\s+)(([1-9]\\d{0,8})(\\.\\d+)?\\s+)(([1-9]\\d{0,8})(\\.\\d+)?\\s*))?((\\/\\/)+(.*))?$)";
            Pattern r = Pattern.compile(pattern);
            Matcher m;
            LinkedList<Triangle> parsed = new LinkedList<>(); //unbalanced read/write with collections????????????????????????????????????????????????????????????

            for (String ln = br.readLine(); ln != null; ln = br.readLine()) {
                m = r.matcher(ln);
                if(m.find()){
                    if(m.group(3)!=null && m.group(6)!=null && m.group(9)!=null){
                        parsed.add(new Triangle(new Double(m.group(3)), new Double(m.group(6)), new Double (m.group(9))));
                    }
                }
                else throw new Exception("Wrong file data format because of '" + ln + "'");
            }
            parsed.forEach((i) -> {
                System.out.println(i);
            });
            
            LinkedList<Triangle> orderedList = new LinkedList<>();
            Comparator<Triangle> byPerimeter = (a,b) -> Double.compare(a.perimeter(), b.perimeter());
            System.out.print("Here are the triangles ordered ascending by perimeter: ");
            orderedList =
                parsed.stream()
                .sorted(byPerimeter)
                .collect(Collectors.toCollection(LinkedList::new));//co tu sie dzieje???????????????????????????????????????????????
            System.out.println(orderedList);

            System.out.print("These are right triangles: ");
            parsed.stream()
                .filter(t -> t.isRight())
                .forEach(System.out::print); //jak dodac delimiters?????????????????????????????????????????????????????
            System.out.println("");
            
            Comparator<Triangle> byArea = (a,b) -> Double.compare(a.area(), b.area());
            System.out.print("These is the triangle with the smallest area: ");
            System.out.println(parsed.stream().min(byArea).get());
            System.out.print("These is the triangle with the biggest area: ");
            System.out.println(parsed.stream().max(byArea).get());
        }
        catch (Exception ex) { 
                System.err.println("Error:" + ex);
        }
    }
    
    public static boolean isPrime(Integer n) {
        if (n < 2) return false;
        if (n == 2) return true;
        for (Integer i = 2; i < n/2; i++)
            if (n%i == 0) return false;
        return true;
    }
    
}
