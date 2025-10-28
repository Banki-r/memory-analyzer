#include "view/MainWindow.h"
#include <cstring>
#include <iostream>

MainWindow::MainWindow(std::string (*func)(std::string *)) {
  _func = func;
  set_default_size(530, 550);
  set_resizable(false);
  const Glib::ustring css_data = R"(
        #main_window{
            background-color: rgba(187, 209, 197, 1);
        }
    )";
  auto css_prov = Gtk::CssProvider::create();
  css_prov->load_from_data(css_data);
  Gtk::StyleContext::add_provider_for_display(
      Gdk::Display::get_default(), css_prov,
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  get_style_context();
  set_name("main_window");

  set_child(m_fixed);
  m_scrollw.set_child(m_textView);
  m_scrollw.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
  m_scrollw.set_size_request(400, 400);
  m_fixed.put(m_scrollw, 50, 80);
  m_fixed.put(m_button, 180, 10);
  m_fixed.put(m_textView, 100, 200);

  m_button.set_label("Browse file...");
  m_button.set_margin(10);
  m_button.signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::on_button_clicked));

  m_refTextBuffer = Gtk::TextBuffer::create();
  m_textView.set_buffer(m_refTextBuffer);
  m_textView.set_editable(false);
  m_refTextBuffer->set_text("Output...");
}

MainWindow::~MainWindow() {}

void MainWindow::on_button_clicked() {
  auto dialog = Gtk::FileDialog::create();

  auto filters = Gio::ListStore<Gtk::FileFilter>::create();
  auto filter_cpp = Gtk::FileFilter::create();
  filter_cpp->set_name(".cpp files");
  filter_cpp->add_pattern("*.cpp");
  filters->append(filter_cpp);

  /*
  auto filter_h = Gtk::FileFilter::create();
  filter_h->set_name(".h files");
  filter_h->add_pattern("*.h");
  filters->append(filter_h);
  */

  dialog->set_filters(filters);
  dialog->set_default_filter(filters->get_item(0));
  dialog->open(sigc::bind(
      sigc::mem_fun(*this, &MainWindow::on_file_dialog_finish), dialog));
}

void MainWindow::on_file_dialog_finish(
    const Glib::RefPtr<Gio::AsyncResult> &result,
    const Glib::RefPtr<Gtk::FileDialog> &dialog) {
  try {
    auto file = dialog->open_finish(result);

    auto filename = file->get_path();
    std::cout << "File selected: " << filename << std::endl;
    filename_ = filename;
    _output = _func(&filename_);
    if (_output.empty()) {
      m_refTextBuffer->set_text("Compilation failed.");
    } else {
      m_refTextBuffer->set_text(_output);
    }
  } catch (const Gtk::DialogError &err) {
    std::cout << "No file selected" << std::endl;
  }
}