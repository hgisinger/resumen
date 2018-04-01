#ifndef _CLIENTES_H_

#define _CLIENTES_H_

#include <QFont>
#include <QWidget>
#include <QMessageBox>
#include <QLineEdit>
#include <QTableWidget>
#include <movimientos.h>
#include <llamadas.h>

class ClientesWidget : public QWidget
{
Q_OBJECT

	QLineEdit *codigoEdit;
	QLineEdit *razonSocialEdit;
	QTableWidget *table;
	QPushButton *buscarButton;
        MovimientosWidget *md;
        LlamadasWidget *ld;

public:
	ClientesWidget(QWidget *parent = 0);
public slots:
    void slotDoubleClicked(int, int);
    void slotSearch();
    void slotLlamadas();
    void slotNuevoCliente();
    void slotRefrescar();
    void crear_todo();
};

#endif
