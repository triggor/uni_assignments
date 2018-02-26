package texteditor;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyVetoException;
import javax.swing.*;
import static texteditor.TextEditor.frame;

class ToolBar extends JToolBar {
    private final JButton newFile = new JButton("New file");
    private final JButton writeFile = new JButton("Send content to console");
    private final JButton closeFile = new JButton("Close");
    protected JComboBox comboBox = new JComboBox();
    
    public ToolBar(){
        super();
        this.setFloatable(false);
        newFile.addActionListener(newFileListener);
        writeFile.addActionListener(writeFileListener);
        closeFile.addActionListener(closeFileListener);
        comboBox.addActionListener(comboBoxListener);
        this.add(newFile);
        this.add(writeFile);
        this.add(closeFile);
        this.add(comboBox);
    }
    
    ActionListener newFileListener = new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            if(e.getSource() == newFile) //????????????????????????????????????????????
                frame.desktop.addInternalFrame();
        }
    };// po co srednik?????????????????????????????????????????????????????????????????
    
    ActionListener writeFileListener = new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            JInternalFrame[] frames = frame.desktop.getAllFrames();
            System.out.println(( (InternalFrame) frames[0]).editorPane.getText());
        }
    };
    ActionListener closeFileListener = new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            frame.desktop.closeInternalFrame();
        }
    };
    ActionListener comboBoxListener = new ActionListener(){
        @Override
        public void actionPerformed(ActionEvent e) {
            JComboBox cb = (JComboBox) e.getSource();
            String docName =(String) cb.getSelectedItem();
            JInternalFrame[] frames = frame.desktop.getAllFrames();
            for(JInternalFrame fr: frames){
                if(fr.getTitle().equals(docName)){
                    fr.toFront();
                    try {
                        fr.setSelected(true);
                        fr.setIcon(false);
                    } 
                    catch (PropertyVetoException ex) {}
                }
            }
        }
    };
}
