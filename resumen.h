#ifndef _RESUMEN_H_

#define _RESUMEN_H_

#include <QVector>
#include <QString>
#include <QMap>

extern QString fecha_archivo;

int string2int(QString num);
qint64 string2long(QString num);
QString int2string(int num);
QString long2string(qint64 num);

struct movimiento
{
    QString tipo;
    QString documento;
    QString recibo;
    QString cheque;
    QString fecha;
    QString debe;
    QString haber;
    QString saldo;
    QString estado;
    QString composicion;
};

struct llamada
{
    QString fecha;
    QString operador;
    QString comentario;
    QString rellamar;
    QString llamo;
 };

struct cliente
{
    QString codigo;
    QString detalle;
    QVector<movimiento> vm;
    QVector<llamada> vl;
    QString ultima_compra;
    QString deuda_antigua;
    QString llamar;
    QString aclaracion;
};

struct color
{
    int dias;
    int rojo;
    int verde;
    int azul;
};

extern QVector<cliente> vc;
extern QVector<color> colores;
extern QMap<int, QString> vdbf;


#endif
