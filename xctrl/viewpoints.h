#ifndef VIEWPOINTS_H
#define VIEWPOINTS_H

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
class PointsTable;
class ViewPoints : public QWidget
{
    Q_OBJECT
public:
    explicit ViewPoints(Project *project,Xctrl *xctrl,QWidget *parent=nullptr);

signals:
    void updated();
private slots:
    void removeSelected();
    void updateTable();
private:
    Project *project;
    Xctrl *xctrl;
    PointsTable *ptable;
    QTableView *m_view;
};

class PointsTable : public QAbstractTableModel{
    Q_OBJECT
public:
    PointsTable(Project *project,Xctrl *xctrl,ViewPoints *parent);
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
    ViewPoints *parent;
    Project *project;
    QStringList errors;
};

#endif // VIEWPOINTS_H
