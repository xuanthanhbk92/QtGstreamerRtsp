#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <QGridLayout>
class MainWin : public QWidget
{
    Q_OBJECT
public:
    MainWin();
    void addWidget(QWidget* wg);
private:
    QGridLayout *m_layout;
    int m_col {0};
    int m_row {0};

};


#endif // MAINWIN_H
