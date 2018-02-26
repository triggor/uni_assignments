package mikolaj;

import java.awt.Color;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import javax.swing.AbstractAction;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.Timer;
import javax.swing.event.MenuEvent;
import javax.swing.event.MenuListener;

public class Frame extends JFrame{
    protected Mikolaj mikolaj;
    public static final int DELAY = 5;
    Timer timer = new Timer(DELAY, new TimerActionListener());
    Panel panel;
    JLabel message; 
    JMenuBar menuBar = new JMenuBar();
    JMenu gameMenu = new JMenu("Game");
    JMenuItem newgameMenuItem = new JMenuItem(new AbstractAction("New") {
        @Override
        public void actionPerformed(ActionEvent e) {
            System.out.println("New");
            setVisible(false);
            dispose();//Czy tez zabija wszystkie watki????? Jeżeli nie, to jak ich zabic????????????
            new Mikolaj();
        }
    });
    JMenuItem opengameMenuItem = new JMenuItem(new AbstractAction("Open") {
        @Override
        public void actionPerformed(ActionEvent e) {
            System.out.println("Open");
            Mikolaj m = null;
            try {
               FileInputStream fileIn = new FileInputStream("savedgame.ser");
               ObjectInputStream in = new ObjectInputStream(fileIn);
               m = (Mikolaj) in.readObject();
               in.close();
               fileIn.close();
            }catch(IOException i) {
               i.printStackTrace();//java.io.WriteAbortedException: writing aborted; java.io.NotSerializableException: java.awt.image.BufferedImage??????????
               return;
            }catch(ClassNotFoundException c) {
               System.out.println("Mikolaj class not found");
               c.printStackTrace();
               return;
            }
            setVisible(false);
            dispose();
            Mikolaj NewMikolaj = new Mikolaj();
            NewMikolaj = m;
            //new Mikolaj(m);
        }
        
    });
    JMenuItem savegameMenuItem = new JMenuItem(new AbstractAction("Save") {
        @Override
        public void actionPerformed(ActionEvent e) {
            System.out.println("Save");
            try {
                FileOutputStream fileOut = new FileOutputStream("savedgame.ser");
                ObjectOutputStream out = new ObjectOutputStream(fileOut);
                out.writeObject(mikolaj);
                out.close();
                fileOut.close();
                System.out.printf("Serialized data is saved in savedgame.ser");
            }catch(IOException i) {
                i.printStackTrace(); //java.io.NotSerializableException: java.awt.image.BufferedImage??????????????????????????????????????????????????????????
            }
        }
    });
    JMenuItem exitMenuItem = new JMenuItem(new AbstractAction("Exit") {
        @Override
        public void actionPerformed(ActionEvent e) {
            System.out.println("Exit");
            System.exit(0);
        }
    });
    JMenu aboutMenu = new JMenu("About");

    public Frame(Mikolaj mikolaj){
        
        this.mikolaj = mikolaj;
        panel = new Panel(this);
        for(int i=0; i<mikolaj.kidsNum; i++){
            panel.children[i].start();
        }
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setLocation(300, 50);
        this.setSize(Mikolaj.N*(Panel.FIELDSIZE+1), Mikolaj.M*(Panel.FIELDSIZE+1)+50);
        this.setDefaultLookAndFeelDecorated(true);//czemu nie dziala dla pierwszej instancji????????????????????????????
        menuBar.add(gameMenu);
        gameMenu.add(newgameMenuItem);
        gameMenu.add(opengameMenuItem);
        gameMenu.add(savegameMenuItem);
        gameMenu.add(exitMenuItem);
        menuBar.add(aboutMenu);
        aboutMenu.addMenuListener(new AboutMenuListener());
        this.setJMenuBar(menuBar);
        this.add(panel);
        panel.setFocusable(true);
        panel.requestFocus();
        this.setVisible(true);
        timer.start();
    }
    class TimerActionListener implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent e) {
            panel.repaint();
            if(mikolaj.gameWon){
                panel.setVisible(false);
                message = new JLabel("YOU WON!");
                Font f = new Font("TimesRoman",Font.BOLD,35);
                message.setForeground(Color.green);
                message.setFont(f);
                message.setBounds(Mikolaj.N*(Panel.FIELDSIZE+1)/2-125,Mikolaj.M*(Panel.FIELDSIZE+1)/2-125,400,100);
                setLayout(null);
                mikolaj.frame.add(message);
                return;
            }
            if(mikolaj.gameLost){
                panel.setVisible(false);
                message = new JLabel("YOU LOST!");
                Font f = new Font("TimesRoman",Font.BOLD,35);
                message.setForeground(Color.red);
                message.setFont(f);
                message.setBounds(Mikolaj.N*(Panel.FIELDSIZE+1)/2-125,Mikolaj.M*(Panel.FIELDSIZE+1)/2-125,400,200);
                setLayout(null);
                mikolaj.frame.add(message);
                return;
            }
        }
    }
    class AboutMenuListener implements MenuListener {

        @Override
        public void menuSelected(MenuEvent me) {
            panel.setVisible(false);
            message = new JLabel("<html>The aim of the Mikolaj game is to make all the "
                    + "children happy by giving them gifts while they are sleeping. "
                    + "It is important not to get caught when they wake up.<br><br>"
                    + "The author of the game: Igor Tryhub<br>Released on 19.01.2017</html>");//jak zrobic zeby nie powstawal konflikt z poprzednia zawartoscia `message`?????????????
            Font f = new Font("TimesRoman",Font.BOLD,15);
            message.setForeground(Color.blue);
            message.setFont(f);
            message.setBounds(Mikolaj.N*(Panel.FIELDSIZE+1)/2-200,Mikolaj.M*(Panel.FIELDSIZE+1)/2-300,400,400);
            setLayout(null);
            mikolaj.frame.add(message);
        }

        @Override
        public void menuDeselected(MenuEvent me) {
            panel.setVisible(true);
            panel.requestFocus();//dlaczego nie dziala????????????????????????????????????????????????????????????????????????????????????
        }

        @Override
        public void menuCanceled(MenuEvent me) {
            throw new UnsupportedOperationException("menuCanceled Not supported yet."); //To change body of generated methods, choose Tools | Templates.
        }
    }
}
