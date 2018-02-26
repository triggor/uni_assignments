package texteditor;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.*;
import javax.swing.text.BadLocationException;
import javax.swing.undo.UndoManager;


public class InternalFrame extends JInternalFrame {
    public JEditorPane editorPane = new JEditorPane();
    private final JScrollPane scrollPane = new JScrollPane(editorPane);
    private final MenuBar menu = new MenuBar();
    private UndoManager undoManager = new UndoManager();
    private static String textBuffer;
    
    public InternalFrame(String s){
        super(s, true, false, true, true);
        this.setJMenuBar(menu);
        this.add(scrollPane);
        this.setVisible(true);
        this.editorPane.addKeyListener(cutKeyListener);
        this.editorPane.addKeyListener(pasteKeyListener);
        this.editorPane.addKeyListener(copyKeyListener);
        this.menu.copy.addActionListener(copyListener);
        this.menu.paste.addActionListener(pasteListener);
        this.menu.cut.addActionListener(cutListener);        
        this.editorPane.getDocument().addUndoableEditListener(undoManager);
        this.menu.undo.addActionListener(undoListener);
        this.menu.redo.addActionListener(redoListener);   
    }
    
    public ActionListener copyListener = new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
           textBuffer = editorPane.getSelectedText();
        }
    };
    public ActionListener pasteListener=new ActionListener(){
        @Override
        public void actionPerformed(ActionEvent e){
            int pos = editorPane.getCaretPosition();
            try{
                    editorPane.getDocument().insertString(pos, textBuffer, null);
                    editorPane.moveCaretPosition(pos);
            } 
            catch (BadLocationException ex){}
        }
    };
    public ActionListener cutListener = new ActionListener(){
        @Override
        public void actionPerformed(ActionEvent e){
            textBuffer = editorPane.getSelectedText();
            editorPane.cut();
        }
    };
    public KeyListener pasteKeyListener = new KeyAdapter(){
        @Override
        public void keyPressed(KeyEvent e){
            if (e.getKeyCode()==KeyEvent.VK_V && (e.getModifiers()&KeyEvent.ALT_MASK)==KeyEvent.ALT_MASK){
                int caretPos = editorPane.getCaretPosition();
                try {
                    editorPane.getDocument().insertString(caretPos, textBuffer, null);
                } catch (BadLocationException ex) {}
            }
        }
    };
    public KeyListener cutKeyListener = new KeyAdapter(){
        @Override
        public void keyPressed(KeyEvent e) {
            if (e.getKeyCode()==KeyEvent.VK_X && (e.getModifiers()&KeyEvent.ALT_MASK)==KeyEvent.ALT_MASK){
                       textBuffer=editorPane.getSelectedText();
                       editorPane.cut();
            }
        }
    };
    public KeyListener copyKeyListener = new KeyAdapter(){
        @Override
        public void keyPressed(KeyEvent e) {
            if (e.getKeyCode() == KeyEvent.VK_C && (e.getModifiers() & KeyEvent.ALT_MASK) == KeyEvent.ALT_MASK) {
                textBuffer=editorPane.getSelectedText();
            }
        }
    };
    public ActionListener undoListener = new ActionListener(){
        @Override
        public void actionPerformed(ActionEvent a) {
            if (undoManager.canUndo())
            {
               undoManager.undo();
            }
        }
    };
    public ActionListener redoListener = new ActionListener(){
        @Override
        public void actionPerformed(ActionEvent e) {
            if (undoManager.canRedo())
            {
               undoManager.redo();
            }
        }
    };
    
    
}
