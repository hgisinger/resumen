#ifndef REGISTROLLAMADA_H
#define REGISTROLLAMADA_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class QDateEdit;

class RegistroLlamada: public QDialog
{
    Q_OBJECT

    int vl_current;
public:
    QLineEdit *fecha;
    QLineEdit *operador;
    QLineEdit *comentarios;
    QLineEdit *llamar;
    QLineEdit *llamo;

    RegistroLlamada(int vl_current, QWidget *parent = 0);

private slots:
    void slotAceptar();
};


#endif // REGISTROLLAMADA_H
