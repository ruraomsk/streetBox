#ifndef VIEWSTRATEGY_H
#define VIEWSTRATEGY_H

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
class StrategyTable;
class ViewStrategy : public QWidget
{
    Q_OBJECT
public:
    explicit ViewStrategy(Project *project,Xctrl *xctrl,QWidget *parent = nullptr);

signals:
    void updated();
private slots:
    void removeSelected();
    void updateTable();

private:
    Project *project;
    Xctrl *xctrl;
    StrategyTable *stable;
    QTableView *m_view;


};
class StrategyTable : public QAbstractTableModel{
    Q_OBJECT
public:
    StrategyTable(Project *project,Xctrl *xctrl,ViewStrategy *parent);
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
    ViewStrategy *parent;
    Project *project;
//    QStringList errors;
};

#endif // VIEWSTRATEGY_H
