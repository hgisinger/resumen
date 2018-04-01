#ifndef MOVIMIENTOS_H
#define MOVIMIENTOS_H

#include <QFont>
#include <QWidget>
#include <QMessageBox>
#include <QPrinter>
#include <QVector>
#include <QString>

#include "resumen.h"
#include "llamadas.h"

class MovimientosWidget : public QWidget
{
Q_OBJECT

public:
	MovimientosWidget(QString titulo, int vm, QWidget *parent = 0);
        LlamadasWidget *ld;
public slots:
	void imprimir(void);
        void slotLlamadas();
private:
	int vm_current;
	QPrinter printer;
	QString titulo_current;
protected:
    void changeEvent(QEvent * event);
};

#endif
