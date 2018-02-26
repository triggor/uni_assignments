package texteditor;

import java.beans.PropertyVetoException;
import javax.swing.*;
import static texteditor.TextEditor.frame;


public class DesktopPane extends JDesktopPane{
    private int nextFrameX;
    private int nextFrameY;
    private final int frameDistance = 50;
    
    public DesktopPane(){
        super(); 
    }
    
    public void addInternalFrame(){
        JOptionPane optionPane = new JOptionPane();
        String s = JOptionPane.showInputDialog("Please enter filename:");
        while(!canUse(s)){
            if(s.equals("")) s = JOptionPane.showInputDialog("Filename cannot be empty.\nPlease try again:");
            else s = JOptionPane.showInputDialog("Filename " + s + " already exists.\nPlease try again:");
        }
        frame.toolBar.comboBox.addItem(s);
        InternalFrame frame = new InternalFrame(s);
        this.add(frame);
        int width = 318;
        int height = this.getHeight() / 3;
        frame.reshape(nextFrameX, nextFrameY, width, height);
        try{
            frame.setSelected(true); //czemu musimy osadzac????????????????????????????
        }
        catch (PropertyVetoException ex) {}
        nextFrameX += frameDistance;
        nextFrameY += frameDistance;
    }
    public void closeInternalFrame(){
        JInternalFrame iframe = this.getSelectedFrame();
        iframe.setVisible(false);
        frame.toolBar.comboBox.removeItem(iframe.getTitle());
        this.remove(iframe);
    }
    private boolean canUse(String name){
        if(name.equals("")) return false;
        JInternalFrame[] frames = this.getAllFrames();
        for(JInternalFrame fr : frames){
            if(fr.getTitle().equals(name)) return false;
        }
        return true;
    }
}

