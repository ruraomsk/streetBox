#ifndef VIEWAREAL_H
#define VIEWAREAL_H

#include <QWidget>
#include <QAbstractTableModel>
#include <QVariant>
#include <QVBoxLayout>
#include <QTableView>
#include <QGridLayout>
#include <QHeaderView>
#include <QPushButton>

#include "../project/project.h"
#include "xctrl.h"
#include "voronoi.h"
#include "viewvoronoi.h"
class ArealTable;
class ViewAreal : public QWidget
{
    Q_OBJECT
public:
    explicit ViewAreal(Project *project,Xctrl *xctrl,QWidget *parent = nullptr);
    Voronoi *vor;
    ViewVoronoi *voroni;
    void redraw();
signals:
    void updated();
private slots:
    void removeSelected();
    void updateTable();

private:
    Project *project;
    Xctrl *xctrl;
    ArealTable *stable;
    QTableView *m_view;
    QGridLayout *grid;

};
class ArealTable : public QAbstractTableModel{
    Q_OBJECT
public:
    ArealTable(Project *project,Xctrl *xctrl,ViewAreal *parent);
    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;
    void removeSelected(const QModelIndex& index);
signals:
    void updated();
public slots:
    void addRecord();
private:
    Xctrl *xctrl;
    ViewAreal *parent;
    Project *project;
//    QStringList errors;
};

#endif // VIEWAREAL_H
