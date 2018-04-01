#include "RegistroLlamada.h"
#include "resumen.h"
#include <QtGui>

int get_int(QString num, int inicio, int cantidad);
extern QString pathArchivos;
extern void buscar_llamadas(QString clienteActual, bool llenar);

RegistroLlamada::RegistroLlamada(int vln, QWidget *parent): QDialog(parent,  Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint)
{
    QString fechaStr = QDate::currentDate().toString("yyyyMMdd");
    QString operadorStr = "";
    QString comentariosStr = "";
    QString llamarStr = "";
    bool alta = true;
    QDate fechaAux;

    vl_current = vln;

    if (vc[vl_current].vl.size() > 0)
    {
        if (vc[vl_current].vl[0].rellamar >= "0" && vc[vl_current].vl[0].llamo.isEmpty())
        {
            alta = false;
            fechaStr       = vc[vl_current].vl[0].fecha;
            operadorStr    = vc[vl_current].vl[0].operador;
            comentariosStr = vc[vl_current].vl[0].comentario;
            llamarStr      = vc[vl_current].vl[0].rellamar;
        }
    }

    QLabel *labelFecha = new QLabel("&Fecha");
    fecha = new QLineEdit;
    fecha->setText(fechaStr.mid(6,2) + "-" + fechaStr.mid(4,2) + "-" + fechaStr.mid(2,2));

    fecha->setEnabled(alta);
    labelFecha->setBuddy(fecha);

    QLabel *labelOperador = new QLabel("&Operador");
    operador = new QLineEdit;
    operador->setEnabled(alta);
    operador->setText(operadorStr);
    operador->setMaxLength(3);
    labelOperador->setBuddy(operador);

    QLabel *labelComentarios = new QLabel("&Comentarios");
    comentarios = new QLineEdit;
    comentarios->setEnabled(alta);
    comentarios->setText(comentariosStr);
    comentarios->setMaxLength(100);
    labelComentarios->setBuddy(comentarios);

    QLabel *labelLlamar = new QLabel("&Llamar");
    llamar = new QLineEdit;
    if (!alta)
    {
        llamar->setText(llamarStr.mid(6,2) + "-" + llamarStr.mid(4,2) + "-" + llamarStr.mid(2,2));
    }
    llamar->setEnabled(alta);
    labelLlamar->setBuddy(llamar);

    QLabel *labelLlamo = new QLabel("&Llamó");
    llamo = new QLineEdit;
    llamo->setMaxLength(3);
    labelLlamo->setBuddy(llamo);

    QPushButton *aceptar = new QPushButton("Aceptar");
    aceptar->setDefault(true);

    QPushButton *cancelar = new QPushButton("Cancelar");

    connect(aceptar, SIGNAL(clicked()), this, SLOT(slotAceptar()));
    connect(cancelar, SIGNAL(clicked()), this, SLOT(reject()));

    QGridLayout *gl = new QGridLayout;
    gl->addWidget(labelFecha, 0, 0);
    gl->addWidget(fecha, 0, 1);
    gl->addWidget(labelOperador, 1, 0);
    gl->addWidget(operador, 1, 1);
    gl->addWidget(labelComentarios, 2, 0);
    gl->addWidget(comentarios, 2, 1);
    gl->addWidget(labelLlamar, 3, 0);
    gl->addWidget(llamar, 3, 1);
    gl->addWidget(labelLlamo, 4, 0);
    gl->addWidget(llamo, 4, 1);

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(aceptar);
    hl->addWidget(cancelar);

    QVBoxLayout *vl = new QVBoxLayout;
    vl->addLayout(gl);
    vl->addLayout(hl);

    setLayout(vl);

    resize(QSize(600, 250));
}


QString convertir_fecha(QString str)
{
    if (str.isEmpty() || str < "0")
    {
        return "        ";
    }

    if (str.count() != 8 && str.count() != 10)
    {
        return "Error";
    }

    if ((str.at(2) != '/' && str.at(2) != '-') || (str.at(5) != '/' && str.at(5) != '-'))
    {
        return "Error";
    }

    if (str.count() == 10)
    {
        return "20" + str.mid(8,2) + str.mid(3,2) + str.mid(0,2);
    }

    return "20" + str.mid(6,2) + str.mid(3,2) + str.mid(0,2);
}


QString codificar_fecha(QString str)
{
    if (str.isEmpty() || str < "0")
    {
        return "        ";
    }

     return str.mid(6,2) + "/" + str.mid(4,2) + "/" + str.mid(0,4);
}


void RegistroLlamada::slotAceptar()
{
    QString fechaStr = convertir_fecha(fecha->text());
    if (fechaStr == "Error")
    {
        QMessageBox::warning(this, "Atención!!!", "El formato de la fecha es incorrecto");
        fecha->setFocus(Qt::OtherFocusReason);
        return;
    }

    if (comentarios->text().isEmpty())
    {
        QMessageBox::warning(this, "Atención!!!", "Debe ingresar los comentarios");
        comentarios->setFocus(Qt::OtherFocusReason);
        return;
    }

    if (operador->text().isEmpty())
    {
        QMessageBox::warning(this, "Atención!!!", "Debe ingresar el operador");
        operador->setFocus(Qt::OtherFocusReason);
        return;
    }

    QString llamarStr = convertir_fecha(llamar->text());
    if (llamarStr == "Error")
    {
        QMessageBox::warning(this, "Atención!!!", "El formato de Llamar es incorrecto");
        llamar->setFocus(Qt::OtherFocusReason);
        return;
    }

    llamada ll;

    ll.fecha = fechaStr;
    ll.operador = operador->text();
    ll.comentario = comentarios->text();
    ll.rellamar = llamarStr;
    ll.llamo = llamo->text();

    buscar_llamadas(vc[vl_current].codigo, true);

    if (vc[vl_current].vl.count() > 0)
    {
        if (!vc[vl_current].vl[0].rellamar.isEmpty() && vc[vl_current].vl[0].llamo.isEmpty())
        {
            vc[vl_current].vl[0].llamo = llamo->text();
        }
        else
        {
            if (fechaStr < vc[vl_current].vl[0].fecha)
            {
                QMessageBox::warning(this, "Atención!!!", "La fecha debe ser posterior a la última ingresada");
                fecha->setFocus(Qt::OtherFocusReason);
                return;
            }
            vc[vl_current].vl.append(ll);
        }
    }
    else
    {
        vc[vl_current].vl.append(ll);
    }

    QString fileName = pathArchivos + ("000000" + vc[vl_current].codigo).right(6) + ".csv";

    QFileInfo fi(fileName);

    QFile data(fileName);
    data.open(QIODevice::WriteOnly);
    QTextStream out(&data);
    out << '"' << vc[vl_current].detalle << '"' << ";;" << vc[vl_current].aclaracion << endl;

    for (int i = 0; i < vc[vl_current].vl.count(); i++)
    {
        out << "\"";
        out << codificar_fecha(vc[vl_current].vl[i].fecha);
        out << "\";\"";
        out << vc[vl_current].vl[i].operador;
        out << "\";\"";
        out << vc[vl_current].vl[i].comentario;
        out << "\";\"";
        out << codificar_fecha(vc[vl_current].vl[i].rellamar);
        out << "\";\"";
        out << vc[vl_current].vl[i].llamo;
        out << "\"" << endl;
    }

    accept();
}
