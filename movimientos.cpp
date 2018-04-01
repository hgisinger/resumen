#include <QApplication>
#include <QFont>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>

#include <QVector>
#include <QString>
//#include <fstream>

#include "clientes.h"
#include "resumen.h"
#include "movimientos.h"
#include "llamadas.h"

extern QString fecha_archivo;
extern void buscar_llamadas(QString clienteActual, bool llenar);


using namespace std;

MovimientosWidget::MovimientosWidget(QString titulo, int vm, QWidget *parent): QWidget(parent,  Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint)
{
	vm_current = vm;
	titulo_current = titulo;

	setWindowTitle(titulo);
        setAttribute(Qt::WA_DeleteOnClose);

	QTableWidget *table = new QTableWidget(this);
	table->setRowCount(vc[vm_current].vm.size());
	table->setColumnCount(9);
	 
	QTableWidgetItem *t;

	t = new QTableWidgetItem("Tipo");
	table->setHorizontalHeaderItem(0, t);
	
	t = new QTableWidgetItem("Rel. Docum.");
	table->setHorizontalHeaderItem(1, t);
	
	t = new QTableWidgetItem("Recibo");
	table->setHorizontalHeaderItem(2, t);
	
	t = new QTableWidgetItem("Cheque");
	table->setHorizontalHeaderItem(3, t);
	
	t = new QTableWidgetItem("Fecha");
	table->setHorizontalHeaderItem(4, t);
	
	t = new QTableWidgetItem("Debe");
	table->setHorizontalHeaderItem(5, t);
	
	t = new QTableWidgetItem("Haber");
	table->setHorizontalHeaderItem(6, t);
	
	t = new QTableWidgetItem("Saldo");
	table->setHorizontalHeaderItem(7, t);
	
	t = new QTableWidgetItem("Deuda");
	table->setHorizontalHeaderItem(8, t);
	
	int altura = fontMetrics().height() + 4;
	
	for (int i = 0; i < vc[vm_current].vm.count(); i++)
	{
		t = new QTableWidgetItem(vc[vm_current].vm[i].tipo);
		table->setItem(i, 0, t);
		
		t = new QTableWidgetItem(vc[vm_current].vm[i].documento);
		table->setItem(i, 1, t);

		t = new QTableWidgetItem(vc[vm_current].vm[i].recibo);
		table->setItem(i, 2, t);
		
		t = new QTableWidgetItem(vc[vm_current].vm[i].cheque);
		t->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		table->setItem(i, 3, t);
		
		t = new QTableWidgetItem(vc[vm_current].vm[i].fecha);
		t->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
		table->setItem(i, 4, t);
		
		t = new QTableWidgetItem(vc[vm_current].vm[i].debe);
		t->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		table->setItem(i, 5, t);
		
		t = new QTableWidgetItem(vc[vm_current].vm[i].haber);
		t->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		table->setItem(i, 6, t);
		
		t = new QTableWidgetItem(vc[vm_current].vm[i].saldo);
		t->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		table->setItem(i, 7, t);
		
		t = new QTableWidgetItem(vc[vm_current].vm[i].composicion);
		t->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		table->setItem(i, 8, t);
		
		table->setRowHeight(i, altura);
	}
	
	const int LEN_TIPO      = 15;
	const int LEN_DOCUMENTO = 20;
	const int LEN_RECIBO    = 15;
	const int LEN_CHEQUE    = 9;
	const int LEN_FECHA     = 10;
	const int LEN_IMPORTE   = 12;
	
	int ancho = fontMetrics().width('x');
	table->setColumnWidth(0, LEN_TIPO * ancho);
	table->setColumnWidth(1, LEN_DOCUMENTO * ancho);
 	table->setColumnWidth(2, LEN_RECIBO * ancho);
 	table->setColumnWidth(3, LEN_CHEQUE * ancho);
 	table->setColumnWidth(4, LEN_FECHA * ancho);
 	table->setColumnWidth(5, LEN_IMPORTE * ancho);
 	table->setColumnWidth(6, LEN_IMPORTE * ancho);
 	table->setColumnWidth(7, LEN_IMPORTE * ancho);
 	table->setColumnWidth(8, LEN_IMPORTE * ancho);
	 
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);

        QPushButton *llamadasButton = new QPushButton("&Llamadas");
        connect(llamadasButton, SIGNAL(clicked()), this, SLOT(slotLlamadas()));

        QPushButton *printButton = new QPushButton("&Imprimir");
	connect(printButton, SIGNAL(clicked()), this, SLOT(imprimir()));

	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->addStretch();
        hLayout->addWidget(llamadasButton);
        hLayout->addWidget(printButton);
	
	QVBoxLayout *layout = new QVBoxLayout;
	
	layout->addWidget(table);
	layout->addLayout(hLayout);
     
	setLayout(layout);
	
	ancho = table->columnWidth(0) + table->columnWidth(1) + table->columnWidth(2) +
			table->columnWidth(3) + table->columnWidth(4) + table->columnWidth(5) +
			table->columnWidth(6) + table->columnWidth(7) + table->columnWidth(8) + 120;
	
	resize(QSize(ancho, 600));

        ld = 0;
 }

 void MovimientosWidget::imprimir(void)
 {
	QPrintDialog printDialog(&printer, this);
	if (printDialog.exec()) 
	{
		QString html;
		
		html = "<h3 align=center>Cuenta corriente de ";
		html = html + titulo_current + " " + fecha_archivo + "</h3>\n";
		
		html = html + "<table width=\"100%\" border=0 cellspacing=2>\n";
		html = html + "<tr>";
		html = html + "<td bgcolor=\"lightgray\" align=\"CENTER\"><font size=\"+1\">Tipo</td>";
		html = html + "<td bgcolor=\"lightgray\" align=\"CENTER\"><font size=\"+1\">Rel. Docum.</td>";
		html = html + "<td bgcolor=\"lightgray\" align=\"CENTER\"><font size=\"+1\">Recibo</td>";
		html = html + "<td bgcolor=\"lightgray\" align=\"CENTER\"><font size=\"+1\">Cheque</td>";
		html = html + "<td bgcolor=\"lightgray\" align=\"CENTER\"><font size=\"+1\">Fecha</td>";
		html = html + "<td bgcolor=\"lightgray\" align=\"CENTER\"><font size=\"+1\">Debe</td>";
		html = html + "<td bgcolor=\"lightgray\" align=\"CENTER\"><font size=\"+1\">Haber</td>";
		html = html + "<td bgcolor=\"lightgray\" align=\"CENTER\"><font size=\"+1\">Saldo</td>";
		html = html + "<td bgcolor=\"lightgray\" align=\"CENTER\"><font size=\"+1\">Deuda</td>";
		html = html + "<tr>\n";
		
		int i;
		
		for (i = 0; i < vc[vm_current].vm.size(); i++)
		{
			html = html + "<tr>";
			html = html + "<td>" + Qt::escape(vc[vm_current].vm[i].tipo) + "</td>";
			html = html + "<td>" + Qt::escape(vc[vm_current].vm[i].documento) + "</td>";
			html = html + "<td>" + Qt::escape(vc[vm_current].vm[i].recibo) + "</td>";
			html = html + "<td>" + Qt::escape(vc[vm_current].vm[i].cheque) + "</td>";
			html = html + "<td align=center>" + Qt::escape(vc[vm_current].vm[i].fecha) + "</td>";
			html = html + "<td align=right>" + Qt::escape(vc[vm_current].vm[i].debe) + "</td>";
			html = html + "<td align=right>" + Qt::escape(vc[vm_current].vm[i].haber) + "</td>";
			html = html + "<td align=right>" + Qt::escape(vc[vm_current].vm[i].saldo) + "</td>";
			html = html + "<td align=right>" + Qt::escape(vc[vm_current].vm[i].composicion) + "</td>"; 
			html = html + "</tr>\n";
		}
		
		html = html + "</table>\n<br>\n";
		
//		ofstream outfile ("new.html");
//		outfile.write(html.toAscii(), html.size());
//		outfile.close();
		
		QTextDocument textDocument;
		textDocument.setHtml(html);
		textDocument.print(&printer);
	}
}

void MovimientosWidget::changeEvent(QEvent *event)
{
}

void MovimientosWidget::slotLlamadas()
{
    int row = vm_current;
    QString titulo;

    buscar_llamadas(vc[row].codigo, true);

    titulo = vc[row].codigo + " - " + vc[row].detalle;

    if (!vc[row].aclaracion.isEmpty())
    {
        titulo += " (" + vc[row].aclaracion + ")";
    }

    if (ld == 0)
    {
        ld = new LlamadasWidget(titulo, row, NULL);
    }
    ld->show();
}
