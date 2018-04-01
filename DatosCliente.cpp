#include "DatosCliente.h"
#include "resumen.h"
#include <QtGui>


DatosCliente::DatosCliente(int vln, QWidget *parent): QWidget(parent,  Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint)
{
    QString html = "";

    html = vdbf[vln];

    QLabel *htmlWidget = new QLabel(html);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(htmlWidget);

    setLayout(hLayout);

    setWindowTitle("Datos de contacto");
    setAttribute(Qt::WA_DeleteOnClose);

}
