#include "prioryty.h"

Prioryty::Prioryty()
{
}

void Prioryty::FromJSON(QJsonArray array)
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            prior[i][j]=array.at(i).toArray().at(j).toInt();
        }
    }
}

QString Prioryty::ToJSON()
{
    QString result;
    //    result.append("{");
    result.append("\"Prioryty\":[");
    for (int i = 0; i < rows; ++i) {
        result.append("[");
        for (int j = 0; j < columns; ++j) {
            result.append(QString::number(prior[i][j]));
            result.append(",");

        }
        result.remove(result.length()-1,1);
        result.append("],");
    }
    result.remove(result.length()-1,1);
    result.append("]");
//    result.append("}");
    return result;
}
