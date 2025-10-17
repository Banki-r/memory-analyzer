#include "view/MainWindow.h"
#include <iostream>

MainWindow::MainWindow() : m_button("Browse files...")
{
    m_button.set_margin(10);

    m_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_clicked));

    set_child(m_button);
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_button_clicked()
{
    std::cout << "Hello World" << std::endl;
}