#include "NuevoCliente.h"
#include <QtGui>

NuevoCliente::NuevoCliente(QWidget *parent): QDialog(parent,  Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint)
{
    QLabel *labelCodigo = new QLabel("&Código");
    codigo = new QLineEdit;
    codigo->setMaxLength(6);
    codigo->setInputMask("000009");
    labelCodigo->setBuddy(codigo);

    QLabel *labelDetalle = new QLabel("&Detalle");
    detalle = new QLineEdit;
    detalle->setMaxLength(50);
    labelDetalle->setBuddy(detalle);

    aceptar = new QPushButton("Aceptar");
    aceptar->setDefault(true);
    aceptar->setEnabled(false);

    QPushButton *cancelar = new QPushButton("Cancelar");

    connect(codigo, SIGNAL(textChanged(const QString&)), this, SLOT(slotActivarBoton(const QString&)));
    connect(aceptar, SIGNAL(clicked()), this, SLOT(slotAceptar()));
    connect(cancelar, SIGNAL(clicked()), this, SLOT(reject()));

    QGridLayout *gl = new QGridLayout;
    gl->addWidget(labelCodigo, 0, 0);
    gl->addWidget(codigo, 0, 1);
    gl->addWidget(labelDetalle, 1, 0);
    gl->addWidget(detalle, 1, 1);

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(aceptar);
    hl->addWidget(cancelar);

    QVBoxLayout *vl = new QVBoxLayout;
    vl->addLayout(gl);
    vl->addLayout(hl);

    setLayout(vl);
}


void NuevoCliente::slotAceptar()
{
    if (detalle->text().isEmpty())
    {
        QMessageBox::warning(this, "Atención!!!", "Debe ingresar la razón social");
        return;
    }
    accept();
}


void NuevoCliente::slotActivarBoton(const QString &str)
{
    aceptar->setEnabled(!str.isEmpty());
}




