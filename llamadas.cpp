#include <QApplication>
#include <QFont>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>

#include <QVector>
#include <QString>

#include "clientes.h"
#include "resumen.h"
#include "llamadas.h"
#include "RegistroLlamada.h"
#include "DatosCliente.h"

extern QString fecha_archivo;
extern QString formatearFecha(QString aammdd);
extern void buscar_llamadas(QString clienteActual, bool llenar);

using namespace std;


LlamadasWidget::LlamadasWidget(QString titulo, int vl, QWidget *parent): QWidget(parent,  Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint)
{
    vl_current = vl;
    titulo_current = titulo;

    setWindowTitle(titulo);
    setAttribute(Qt::WA_DeleteOnClose);

    table = new QTableWidget(this);
    table->setColumnCount(5);

    QTableWidgetItem *t;

    t = new QTableWidgetItem("Fecha");
    table->setHorizontalHeaderItem(0, t);

    t = new QTableWidgetItem("Operador");
    table->setHorizontalHeaderItem(1, t);

    t = new QTableWidgetItem("Comentarios");
    table->setHorizontalHeaderItem(2, t);

    t = new QTableWidgetItem("Llamar");
    table->setHorizontalHeaderItem(3, t);

    t = new QTableWidgetItem("Llamó");
    table->setHorizontalHeaderItem(4, t);

    const int LEN_FECHA       = 10;
    const int LEN_INICIALES   = 10;
    const int LEN_COMENTARIOS = 80;

    int ancho = fontMetrics().width('x');
    table->setColumnWidth(0, LEN_FECHA * ancho);
    table->setColumnWidth(1, LEN_INICIALES * ancho);
    table->setColumnWidth(2, LEN_COMENTARIOS * ancho);
    table->setColumnWidth(3, LEN_FECHA * ancho);
    table->setColumnWidth(4, LEN_INICIALES * ancho);

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    QPushButton *nuevoButton = new QPushButton("&Registrar llamada");
    connect(nuevoButton, SIGNAL(clicked()), this, SLOT(slotRegistrar()));

    QPushButton *datosButton = new QPushButton("&Ver Contacto");
    connect(datosButton, SIGNAL(clicked()), this, SLOT(slotVerDatos()));

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addStretch();
    hLayout->addWidget(datosButton);
    hLayout->addWidget(nuevoButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(table);
    layout->addLayout(hLayout);
    setLayout(layout);

    ancho = table->columnWidth(0) + table->columnWidth(1) + table->columnWidth(2) +
            table->columnWidth(3) + table->columnWidth(4) + 120;

    resize(QSize(ancho, 400));

    crear_todo();
 }

void LlamadasWidget::crear_todo()
{
    QTableWidgetItem *t;
    int altura = fontMetrics().height() + 4;
    table->setRowCount(vc[vl_current].vl.size());

    for (int i = 0; i < vc[vl_current].vl.count(); i++)
    {
        t = new QTableWidgetItem(formatearFecha(vc[vl_current].vl[i].fecha));
        t->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        table->setItem(i, 0, t);

        t = new QTableWidgetItem(vc[vl_current].vl[i].operador);
        t->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        table->setItem(i, 1, t);

        t = new QTableWidgetItem(vc[vl_current].vl[i].comentario);
        table->setItem(i, 2, t);

        t = new QTableWidgetItem(formatearFecha(vc[vl_current].vl[i].rellamar));
        t->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        table->setItem(i, 3, t);

        t = new QTableWidgetItem(vc[vl_current].vl[i].llamo);
        t->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        table->setItem(i, 4, t);

        table->setRowHeight(i, altura);
    }
}


void LlamadasWidget::changeEvent(QEvent *event)
{
}


void LlamadasWidget::slotRegistrar()
{
    RegistroLlamada rl(vl_current, this);
    if (rl.exec() == QDialog::Accepted)
    {
        buscar_llamadas(vc[vl_current].codigo, true);
        crear_todo();
    }
}

void LlamadasWidget::slotVerDatos()
{
    QString cliente = vc[vl_current].codigo;
    int icod = cliente.toInt();

    DatosCliente *rl = new DatosCliente(icod, 0);
    rl->show();
}
