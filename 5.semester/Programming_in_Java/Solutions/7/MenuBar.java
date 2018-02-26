package texteditor;

import javax.swing.*;

public class MenuBar extends JMenuBar{
    public JButton copy = new JButton("Copy");
    public JButton paste = new JButton("Paste");
    public JButton cut = new JButton("Cut");
    public JButton undo = new JButton("Undo");
    public JButton redo = new JButton("Redo");
    
    public MenuBar(){
        super();
        this.add(copy);
        this.add(paste);
        this.add(cut);
        this.add(undo);
        this.add(redo);
    }
}
