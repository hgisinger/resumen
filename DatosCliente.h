#ifndef DATOSCLIENTE_H
#define DATOSCLIENTE_H

#include <QDialog>

class DatosCliente: public QWidget
{
    Q_OBJECT

public:
    DatosCliente(int vl_current, QWidget *parent = 0);
};


#endif // DATOSCLIENTE_H
