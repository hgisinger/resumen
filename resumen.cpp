#include <QApplication>
#include <QFileInfo>
#include <QDateTime>

#include <QVector>
#include <QSet>
#include <QString>
#include <QFile>
#include <QTextStream>

#include <QDir>
#include <QDirIterator>

#include <QMessageBox>

#include "resumen.h"
#include "clientes.h"

QString fecha_archivo;
QVector<cliente> vc;
QMap<int, QString> vdbf;
QVector<color> colores;
QString pathArchivos;
QSet<QString> csv_files;

using namespace std;

void alert(QString str) {
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.exec();
}

QString formatearFecha(QString aammdd)
{
    QString aux;

    if (aammdd.isEmpty())
    {
        return "        ";
    }

    aux  = aammdd.mid(6, 2);
    aux += "-";
    aux += aammdd.mid(4, 2);
    aux += "-";
    aux += aammdd.mid(2, 2);

    return aux;
}


int string2int(QString num)
{
    QString cp = "";
    int i;
	
    for (i = 0; i < num.count(); i++)
    {
        if (num.at(i) != '.' && num.at(i) != ',')
        {
            cp.append(num.at(i));
        }
    }
	
    return cp.toInt();
}


QString int2string(int num)
{
    QString aux;
    QString signo = "";
	
    aux.setNum(num);
	
    if (aux.left(1) == "-")
    {
        signo = "-";
        aux = aux.mid(1);
    }
	
    int longitud = aux.count();
	
    switch (longitud)
    {
        case 1: aux = "0,0" + aux;
                break;
        case 2:	aux = "0," + aux;
                break;
        case 3: aux = aux.mid(0, 1) + "," + aux.mid(1);
                break;
        case 4: aux = aux.mid(0, 2) + "," + aux.mid(2);
                break;
        case 5: aux = aux.mid(0, 3) + "," + aux.mid(3);
                break;
        case 6: aux = aux.mid(0, 1) + "." + aux.mid(1, 3) + "," + aux.mid(4);
                break;
        case 7: aux = aux.mid(0, 2) + "." + aux.mid(2, 3) + "," + aux.mid(5);
                break;
        case 8: aux = aux.mid(0, 3) + "." + aux.mid(3, 3) + "," + aux.mid(6);
                break;
        case 9:	aux = aux.mid(0, 1) + "." + aux.mid(1, 3) + "." + aux.mid(4, 3) + "," + aux.mid(7);
                break;
        case 10:aux = aux.mid(0, 2) + "." + aux.mid(2, 3) + "." + aux.mid(5, 3) + "," + aux.mid(8);
                break;
        case 11: aux = aux.mid(0, 3) + "." + aux.mid(3, 3) + "." + aux.mid(6, 3) + "," + aux.mid(9);
                break;
    }
	
    return aux;
}

QString sacar_string(QString &buffer)
{
    int i = 0;
    QString aux = "";
    bool adentro = false;

    while (i < buffer.count())
    {
        QChar qc = buffer.at(i);

        if (qc == '"')
        {
            adentro = !adentro;
            i++;
            continue;
        }

        if (qc == ';')
        {
            if (!adentro)
            {
                buffer = buffer.mid(i + 1);
                return aux;
            }
        }

        if (!aux.isEmpty() || qc != ' ')
        {
            aux = aux + qc;
        }
        i++;
    }

    buffer = "";
    return aux;
}

bool colorMayor(const color &c1, const color &c2)
{
    return c1.dias > c2.dias;
}

void buscar_colores()
{
    QFile data("resumen.ini");
    data.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream ifs(&data);

    QString buffer;

    while (!ifs.atEnd())
    {
        buffer = ifs.readLine();

        QString codigo = sacar_string(buffer);
        if (codigo == "path")
        {
            pathArchivos = sacar_string(buffer);
        }
        if (codigo == "color")
        {
            color cl;
            codigo = sacar_string(buffer);
            cl.dias  = codigo.toInt();
            codigo = sacar_string(buffer);
            cl.rojo  = codigo.toInt();
            codigo = sacar_string(buffer);
            cl.verde = codigo.toInt();
            codigo = sacar_string(buffer);
            cl.azul  = codigo.toInt();
            colores.append(cl);
        }
    }
   qSort(colores.begin(), colores.end(), colorMayor);
}


bool llamadaMayor(const llamada &l1, const llamada &l2)
{
    return l1.fecha > l2.fecha;
}

void buscar_llamadas(QString clienteActual, bool llenar)
{
    QString fileName = pathArchivos + ("000000" + clienteActual).right(6) + ".csv";

    QFile data(fileName);
    if (!data.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream ifs(&data);

    QString buffer;
    int linea = 1, fila;

    for (fila = 0; fila < vc.count(); fila++)
    {
        if (vc[fila].codigo == clienteActual)
        {
            vc[fila].vl.clear();
            break;
        }
    }

    while (!ifs.atEnd())
    {
        buffer = ifs.readLine();

        if (linea == 1)
        {

            if (fila >= vc.count())
            {
                cliente cl;
                cl.codigo = clienteActual;
                cl.detalle = sacar_string(buffer);
                sacar_string(buffer);
                cl.aclaracion = sacar_string(buffer);
                cl.ultima_compra = "";
                cl.deuda_antigua = "";

                vc.append(cl);

                fila = vc.count() - 1;
            }
            else
            {
                sacar_string(buffer);
                sacar_string(buffer);
                vc[fila].aclaracion = sacar_string(buffer);
            }
        }
        else
        {
            llamada ll;
            ll.fecha      = sacar_string(buffer);
            ll.fecha = ll.fecha.mid(6,4) + ll.fecha.mid(3,2) + ll.fecha.mid(0,2);
            ll.operador   = sacar_string(buffer);
            ll.comentario = sacar_string(buffer);
            ll.rellamar   = sacar_string(buffer);
            ll.rellamar = ll.rellamar.mid(6,4) + ll.rellamar.mid(3,2) + ll.rellamar.mid(0,2);
            ll.llamo      = sacar_string(buffer);
            if (ll.llamo.isEmpty())
            {
                vc[fila].llamar = ll.rellamar;
            }
            if (llenar)
            {
                vc[fila].vl.append(ll);
            }
        }
        linea++;
    }
    if (llenar)
    {
        qSort(vc[fila].vl.begin(), vc[fila].vl.end(), llamadaMayor);
    }
}

void buscar_llamadas(QString clienteAnterior, QString clienteActual)
{
    int inicio = clienteAnterior.toInt();
    int fin = clienteActual.toInt() - 1;

    for (int i = inicio; i <= fin; i++)
    {
        QString codigo;
        codigo.setNum(i);
        QString fileNameToSeek = ("000000" + codigo).right(6) + ".csv";
        if (csv_files.contains(fileNameToSeek)) {
            buscar_llamadas(codigo, false);
        }
    }
}

void calcular_composicion(void)
{
    int saldo = 0, debe;

    for (int i = 0; i < vc.count(); i++)
    {
        if (vc[i].vm.count() > 0)
        {
            saldo = string2int(vc[i].vm[vc[i].vm.count() - 1].saldo);
        }
	
        for (int j = vc[i].vm.count() - 1; j >= 0; j--)
        {
            if (saldo > 0)
            {
                debe = string2int(vc[i].vm[j].debe);
                if (debe > 0)
                {
                    saldo = saldo - debe;
                    if (saldo >= 0)
                    {
                        vc[i].vm[j].composicion = vc[i].vm[j].debe;
                    }
                    else
                    {
                        vc[i].vm[j].composicion = int2string(debe + saldo);
                    }
                    vc[i].deuda_antigua = vc[i].vm[j].fecha;
                }
            }
            else
            {
                break;
            }
        }
    }
}


QString copiar_numero(QString buffer, int inicio, int longitud)
{
    QString aux = buffer.mid(inicio, longitud);
	
    aux.replace(',', '$');
    aux.replace('.', ',');
    aux.replace('$', '.');
	
    return aux;
}

void agregar_mov(QString buffer)
{
    if (vc.count() > 0)
    {
        movimiento m;
        m.tipo        = buffer.mid(1, 10);
        m.documento   = buffer.mid(12, 18);
        m.recibo      = buffer.mid(35, 13);
        m.cheque      = buffer.mid(57, 4);
        m.fecha       = buffer.mid(64, 8);
        if (m.fecha == "    -  -")
        {
            m.fecha = "";
        }
        m.debe        = copiar_numero(buffer, 74, 14);
        m.haber       = copiar_numero(buffer, 90, 14);
        m.saldo       = copiar_numero(buffer, 106, 14);
        m.composicion = "";
		
        vc[vc.count() - 1].vm.append(m);
    }
}

void leer_todo()
{
    QFileInfo fi("ctactexx.txt");
    QDateTime dt = fi.lastModified();
    fecha_archivo = dt.toString("(d/M/yyyy h:m)");

    QFile data("ctactexx.txt");
    data.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream ifs(&data);

    QString buffer;
    QString clienteAnterior = "000001";

    vc.clear();

    while (!ifs.atEnd())
    {
        buffer = ifs.readLine();
        if (buffer.size() < 11)
        {
            continue;
        }

        QString tipo_reg = buffer.mid(1, 10);

        if (tipo_reg == "Cliente   ")
        {
            cliente cl;
            int codigo;

            cl.codigo  = buffer.mid(12, 6);
            codigo     = cl.codigo.toInt();
            cl.codigo.setNum(codigo);
            cl.detalle = buffer.mid(20, 35);

            buscar_llamadas(clienteAnterior, cl.codigo);
            clienteAnterior = cl.codigo;

            vc.append(cl);
        }

        if (tipo_reg == "S.Doc.Ant.")
        {
            agregar_mov(buffer);
        }

        if (tipo_reg == "Recibo    ")
        {
            agregar_mov(buffer);
        }

        if (tipo_reg == "Factura   ")
        {
            agregar_mov(buffer);
            if (vc.count() > 0)
            {
                vc[vc.count() - 1].ultima_compra = buffer.mid(64, 8);
            }
        }

        if (tipo_reg == "N. Crédito")
        {
            agregar_mov(buffer);
        }

        if (tipo_reg == "N. Débito ")
        {
            agregar_mov(buffer);
        }

        if (tipo_reg == "Cheque    ")
        {
            agregar_mov(buffer);
        }

        if (tipo_reg == "          ")
        {
            if (vc.count() > 0)
            {
                movimiento m;

                m.tipo = "Saldo";
                m.documento = "";
                m.recibo = "";
                m.cheque = "";
                m.fecha = "";
                m.debe = "";
                m.haber = "";
                m.saldo = copiar_numero(buffer, 110, 10);
                m.composicion = "";

                vc[vc.count() - 1].vm.append(m);
            }
        }
    }

    buscar_llamadas(clienteAnterior, QString("100000"));

    calcular_composicion();
}

QString makeString(char *str, int len) {
    char buffer[512];
    QString tmp;

    memcpy(buffer, str, len);
    buffer[len] = 0;

    tmp = buffer;

    return tmp.trimmed();
}

int cargar_clientes(void)
{
    QFile file(pathArchivos + "cuentas.dbf");
    QString str;
    QString codigo;
    int icod;
    char buffer[512];

    if (!file.open(QIODevice::ReadOnly)) {
        return 1;
    }

    QDataStream in(&file);

    in.skipRawData(641);
    in.readRawData(buffer, 1);
    int delta = 1;
    int len = 391;
    if (buffer[0] == '\0') {
        len = 383;
        delta = 0;
    }

    while (in.readRawData(buffer + delta, len - delta) > 0) {
        delta = 0;
        codigo = makeString(buffer + 1, 6);
        icod = codigo.toInt();

        str  = "<table border='1' cellspacing='0' cellpadding='3'>";
        str += "<tr>";
        str += "<td width=300>";
        str += makeString(buffer + 1, 6);
        str += " ";
        str += makeString(buffer + 7, 35);
        str += "</td>";
        str += "<td width=300>";
        str += makeString(buffer + 42, 35);
        str += "</td>";
        str += "</tr>";
        str += "<tr>";
        str += "<td>";
        str += makeString(buffer + 75, 35);
        str += "</td>";
        str += "<td rowspan='2'>";
        str += "<table>";
        str += "<tr>";
        str += "<td>";
        str += makeString(buffer + 182, 15);
        str += "</td>";
        str += "</tr>";
        str += "<tr>";
        str += "<td>";
        str += makeString(buffer + 197, 15);
        str += "</td>";
        str += "</tr>";
        str += "<tr>";
        str += "<td>";
        str += makeString(buffer + 212, 15);
        str += "</td>";
        str += "</tr>";
        str += "<tr>";
        str += "<td>";
        str += makeString(buffer + 326, 50);
        str += "</td>";
        str += "</tr>";
        str += "</table>";
        str += "<tr>";
        str += "<td>";
        str += makeString(buffer + 112, 30);
        str += "</td>";
        str += "</tr>";
        str += "<tr>";
        str += "</td>";
        str += "</tr>";

        str += "<tr>";
        str += "<td>";
        str += makeString(buffer + 172, 10);
        str += " ";
        str += makeString(buffer + 142, 30);
        str += "</td>";
        str += "<td>Contacto: ";
        str += makeString(buffer + 242, 30);
        str += "</td>";
        str += "</tr>";

        str += "<tr>";
        str += "<td>";
        if (buffer[325] == '1') {
            str += "Inscripto ";
        }
        if (buffer[325] == '3') {
            str += "Consumidor Final ";
        }
        if (buffer[325] == '4') {
            str += "Exento ";
        }
        if (buffer[325] == '6') {
            str += "Monotributo ";
        }
        str += makeString(buffer + 227, 15);
        str += "</td>";
        str += "<td>Observaciones: ";
        str += makeString(buffer + 272, 50);
        str += "</td>";
        str += "</tr>";

        str += "<tr>";
        str += "<td colspan='2'>Ramo: ";
        str += makeString(buffer + 376, 4);
        str += " Transp.: ";
        str += makeString(buffer + 379, 2);
        str += " Zona: ";
        str += makeString(buffer + 322, 3);
        str += "</td>";
        str += "</tr>";

        str += "</table><br>";

        vdbf[icod] = str;
    }

    file.close();

    return 0;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    buscar_colores();

    QDirIterator csv(pathArchivos);
    while (csv.hasNext()) {
        csv.next();
        csv_files.insert(csv.fileName());
    }

    ClientesWidget widget;
	
    widget.show();
     
    return app.exec();
} 


// 281 - 408
