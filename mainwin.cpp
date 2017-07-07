#include "mainwin.h"

MainWin::MainWin() : m_layout(new QGridLayout)
{
    this->setLayout(m_layout);
    this->resize(800,400);
}

void MainWin::addWidget(QWidget *wg)
{
    m_layout->addWidget(wg,m_row,m_col);
    if (m_col ==2)
    {
        m_col = 0;
        ++m_row;
    }else{
        ++m_col;
    }
}
