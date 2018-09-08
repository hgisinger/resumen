#include <QApplication>
#include <QFont>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QVector>
#include <QString>
#include <QDate>
#include <QFileInfo>
#include <QTextStream>
#include <QHeaderView>

#include "clientes.h"
#include "resumen.h"
#include "movimientos.h"
#include "llamadas.h"
#include "NuevoCliente.h"

using namespace std;

extern QString fecha_archivo;
extern QString formatearFecha(QString aammdd);
extern void buscar_llamadas(QString clienteActual, bool llenar);
extern void buscar_colores();
extern void leer_todo();
extern QString pathArchivos;
extern int cargar_clientes(void);


int get_int(QString num, int inicio, int cantidad)
{
    QString cp = "";
    int i;

    for (i = 0; i < cantidad; i++)
    {
        cp.append(num.at(inicio + i));
    }

    return cp.toInt();
}


ClientesWidget::ClientesWidget(QWidget *parent): QWidget(parent,  Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint)
{
    table = new QTableWidget(this);
    table->setColumnCount(6);
	
    QTableWidgetItem *t;
	 
    t = new QTableWidgetItem("Código");
    table->setHorizontalHeaderItem(0, t);
	
    t = new QTableWidgetItem("Razón Social");
    table->setHorizontalHeaderItem(1, t);

    t = new QTableWidgetItem("Ult.Compra");
    table->setHorizontalHeaderItem(2, t);

    t = new QTableWidgetItem("Deuda Ant.");
    table->setHorizontalHeaderItem(3, t);

    t = new QTableWidgetItem("Llamar");
    table->setHorizontalHeaderItem(4, t);

    t = new QTableWidgetItem("Saldo");
    table->setHorizontalHeaderItem(5, t);

    const int LEN_CODIGO = 8;
    const int LEN_DETALLE = 35;
    const int LEN_SALDO = 18;
    const int LEN_FECHA = 12;

    int ancho = fontMetrics().width('x');
    table->setColumnWidth(0, LEN_CODIGO * ancho);
    table->setColumnWidth(1, LEN_DETALLE * ancho);
    table->setColumnWidth(2, LEN_FECHA * ancho);
    table->setColumnWidth(3, LEN_FECHA * ancho);
    table->setColumnWidth(4, LEN_FECHA * ancho);
    table->setColumnWidth(5, LEN_SALDO * ancho);

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(table, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(slotDoubleClicked(int, int)));
	
    QLabel *codigoLabel = new QLabel("&Código");
    codigoEdit = new QLineEdit();
    codigoEdit->setMaxLength(6);
    codigoLabel->setBuddy(codigoEdit);
	
    QLabel *razonSocialLabel = new QLabel("&Razón Social");
    razonSocialEdit = new QLineEdit();
    razonSocialEdit->setMaxLength(20);
    razonSocialLabel->setBuddy(razonSocialEdit);
	
    QPushButton *refrescarButton = new QPushButton("&Refrescar");
    connect(refrescarButton, SIGNAL(clicked()), this, SLOT(slotRefrescar()));

    QPushButton *nuevoButton = new QPushButton("&Nuevo");
    connect(nuevoButton, SIGNAL(clicked()), this, SLOT(slotNuevoCliente()));

    QPushButton *llamadasButton = new QPushButton("&Llamadas");
    connect(llamadasButton, SIGNAL(clicked()), this, SLOT(slotLlamadas()));

    buscarButton = new QPushButton("&Buscar");
    buscarButton->setDefault(true);
    connect(buscarButton, SIGNAL(clicked()), this, SLOT(slotSearch()));
	
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addStretch();
    hLayout->addWidget(codigoLabel);
    hLayout->addWidget(codigoEdit);
    hLayout->addWidget(razonSocialLabel);
    hLayout->addWidget(razonSocialEdit);
    hLayout->addWidget(buscarButton);
    hLayout->addWidget(nuevoButton);
    hLayout->addWidget(llamadasButton);
    hLayout->addWidget(refrescarButton);

    QVBoxLayout *layout = new QVBoxLayout;
	
    layout->addWidget(table);
    layout->addLayout(hLayout);
	
    setLayout(layout);
	
    ancho = table->columnWidth(0) + table->columnWidth(1) + table->columnWidth(2)
             + table->columnWidth(3) + table->columnWidth(4) + table->columnWidth(5) + 120;
	
    resize(QSize(ancho, 600));
	
    QString titulo = "Cuenta Corriente ";
    titulo += fecha_archivo;
	
    setWindowTitle(titulo);

    crear_todo();

    md = 0;
    ld = 0;
}

void ClientesWidget::crear_todo()
{
    int saldo_total = 0;

    leer_todo();
    buscar_colores();
    cargar_clientes();

    table->setRowCount(vc.count() + 1);

    int altura = fontMetrics().height() + 4;
    QTableWidgetItem *t;

    table->verticalHeader()->hide();

    for (int i = 0; i < vc.count(); i++)
    {
        t = new QTableWidgetItem(vc[i].codigo);
        t->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        table->setItem(i, 0, t);
		
        t = new QTableWidgetItem(vc[i].detalle);
        table->setItem(i, 1, t);
		
        t = new QTableWidgetItem(vc[i].ultima_compra);
        t->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        table->setItem(i, 2, t);

        t = new QTableWidgetItem(vc[i].deuda_antigua);
        t->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        int dias = 0;

        if (vc[i].deuda_antigua.count() > 7)
        {
            QDate deuda_antigua(2000 + get_int(vc[i].deuda_antigua, 6, 2),
                                get_int(vc[i].deuda_antigua, 3, 2),
                                get_int(vc[i].deuda_antigua, 0, 2));
            dias = deuda_antigua.daysTo(QDate::currentDate());
        }

        int j = 0;
        while (j < colores.count())
        {
            if (dias > colores[j].dias)
            {
                t->setForeground(QBrush(QColor(colores[j].rojo,
                                               colores[j].verde,
                                               colores[j].azul)));
                break;
            }
            j++;
        }

        table->setItem(i, 3, t);

        t = new QTableWidgetItem(formatearFecha(vc[i].llamar));
        t->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        table->setItem(i, 4, t);

        if (vc[i].vm.size() > 0)
        {
            t = new QTableWidgetItem(vc[i].vm[vc[i].vm.size() - 1].saldo);
            saldo_total += string2int(vc[i].vm[vc[i].vm.size() - 1].saldo);
        }
        else
        {
            t = new QTableWidgetItem("");
        }
        t->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        table->setItem(i, 5, t);
		
        table->setRowHeight(i, altura);
    }
	
    t = new QTableWidgetItem("Total");
    t->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    table->setItem(vc.count(), 0, t);
		
    t = new QTableWidgetItem(int2string(saldo_total));
    t->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    table->setItem(vc.count(), 5, t);
 }

void ClientesWidget::slotDoubleClicked(int row, int)
{
    if (vc[row].vm.count () > 0)
    {
        md = new MovimientosWidget(vc[row].codigo + " - " + vc[row].detalle, row, NULL);
        md->show();
    }
    else
    {
        QMessageBox::warning(this, "Atención!!!", "No hay cuenta corriente para este cliente");
    }
}


void ClientesWidget::slotLlamadas()
{
    int row = table->currentRow();
    QString titulo;

    buscar_llamadas(vc[row].codigo, true);

    titulo = vc[row].codigo + " - " + vc[row].detalle;

    if (!vc[row].aclaracion.isEmpty())
    {
        titulo += " (" + vc[row].aclaracion + ")";
    }

    ld = new LlamadasWidget(titulo, row, NULL);
    ld->show();
}

void ClientesWidget::slotNuevoCliente()
{
    NuevoCliente nc;
    if (nc.exec() == QDialog::Accepted)
    {
        QString fileName = pathArchivos + ("000000" + nc.codigo->text()).right(6) + ".csv";

        QFileInfo fi(fileName);

        if (!fi.exists())
        {
            QFile data(fileName);
            data.open(QIODevice::WriteOnly);
            QTextStream out(&data);
            out << nc.detalle->text() << endl;
            crear_todo();
        }
        else
        {
            QMessageBox::warning(this, "Atención!!!", "El cliente ya existe");
        }
    }
}


void ClientesWidget::slotRefrescar()
{
    crear_todo();
}


void ClientesWidget::slotSearch()
{
    QString codigo = codigoEdit->text();
    QString razonSocial = razonSocialEdit->text();
	
    int pos = table->currentRow();
	
    if (codigo.isEmpty() && razonSocial.isEmpty())
    {
        slotDoubleClicked(pos, 0);
        return;
    }
	
    bool enc = false;
	
    for (int i = pos + 1; i < vc.count(); i++)
    {
        if (vc[i].codigo.indexOf(codigo, 0, Qt::CaseInsensitive) > -1 &&        vc[i].detalle.indexOf(razonSocial, 0, Qt::CaseInsensitive) > -1)
        {
            pos = i;
            enc = true;
            break;
        }
    }
    if (!enc)
    {
        for (int i = 0; i < pos; i++)
        {
            if (vc[i].codigo.indexOf(codigo, 0, Qt::CaseInsensitive) > -1 && vc[i].detalle.indexOf(razonSocial, 0, Qt::CaseInsensitive) > -1)
            {
                pos = i;
                enc = true;
                break;
            }
        }
    }
	
    if (!enc)
    {
        QMessageBox::warning(this, "Resultado de la Búsqueda", "No se encontraron registros con los parámetros ingresados");
    }
	
    table->setCurrentCell(pos, 0);
}
