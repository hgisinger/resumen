#ifndef LLAMADAS_H
#define LLAMADAS_H

#include <QFont>
#include <QWidget>
#include <QMessageBox>
#include <QPrinter>
#include <QVector>
#include <QString>
#include <QTableWidget>

#include "resumen.h"

class LlamadasWidget : public QWidget
{
Q_OBJECT

public:
    LlamadasWidget(QString titulo, int vl, QWidget *parent = 0);
    void crear_todo();
private:
    int vl_current;
    QString titulo_current;
    QTableWidget *table;
protected:
    void changeEvent(QEvent * event);
public slots:
    void slotRegistrar();
    void slotVerDatos();
};

#endif // LLAMADAS_H
