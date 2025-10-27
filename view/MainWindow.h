#pragma once
#include <gtkmm.h>

class MainWindow : public Gtk::Window
{
public:
    MainWindow(void (*func)(std::string *));
    ~MainWindow();
    std::string filename_;
protected:
    void on_button_clicked();
    void on_file_dialog_finish(const Glib::RefPtr<Gio::AsyncResult>& result,
    const Glib::RefPtr<Gtk::FileDialog>& dialog);
    void (*_func)(std::string *);
    int _argc;
    Gtk::ScrolledWindow m_scrollw;
    Gtk::Fixed m_fixed;

    Gtk::Button m_button;
    Gtk::TextView m_textView;
    Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer;
};