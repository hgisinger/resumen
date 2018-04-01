#ifndef NUEVOCLIENTE_H
#define NUEVOCLIENTE_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class NuevoCliente: public QDialog
{
    Q_OBJECT

public:
    QLineEdit *codigo;
    QLineEdit *detalle;

    QPushButton *aceptar;

    NuevoCliente(QWidget *parent = 0);

private slots:
    void slotAceptar();
    void slotActivarBoton(const QString &);
};

#endif // NUEVOCLIENTE_H
