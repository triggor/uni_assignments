package texteditor;

import java.awt.BorderLayout;
import javax.swing.*;

public class MainFrame extends JFrame {
   protected final ToolBar toolBar = new ToolBar();
   protected final DesktopPane desktop = new DesktopPane();
   
    public MainFrame(String s){
        super(s); //musi byc???????????????????????????????????????????????????????????
        this.setSize(800,600); //overridable method call in constructor??????????????????
        this.setLocationRelativeTo(null);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.add(toolBar, BorderLayout.NORTH);
        this.add(desktop);
        this.setVisible(true);
        this.setDefaultLookAndFeelDecorated(true);
        this.setIconImage(new ImageIcon("src/logo.jpg").getImage());
    }
}
