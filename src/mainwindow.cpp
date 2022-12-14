#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDebug>

static const QString namedStarsPath = "/usr/share/kstars/namedstars.dat";
static const QString starsNamedPath = "/usr/share/kstars/starnames.dat";

static const QString namedStarsDefaultPath = "/usr/share/kstars/namedstars-default.dat";
static const QString starsNamedDefaultPath = "/usr/share/kstars/starnames-default.dat";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // make backup
    if (!QFile::exists(namedStarsDefaultPath))
    {
        QFile::copy(namedStarsPath, namedStarsDefaultPath);
    }

    if (!QFile::exists(starsNamedDefaultPath))
    {
        QFile::copy(starsNamedPath, starsNamedDefaultPath);
    }


    ui->tableWidget->setColumnCount(11);
    ui->tableWidget->setHorizontalHeaderLabels({
        "Ra", "Dec", "Mag", "pmRa", "pmDec", "Parallax", "bv_index", "spec_type", "flags", "bayerName", "longName"
    });

    ui->namedStarsDataElement->setColumnCount(4);
    ui->namedStarsDataElement->setHorizontalHeaderLabels({
        "Name", "Size", "Type", "Scale"
    });

    ui->namedStarsEntry->setColumnCount(3);
    ui->namedStarsEntry->setHorizontalHeaderLabels({
        "Id", "Offset", "Count"
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load()
{
    starDatabase.clear();
    starDatabase.load();

    ui->namedStarsDescription->setText(starDatabase.namedStars.description());
    ui->namedStarsVersion->setValue(starDatabase.namedStars.version());

    ui->tableWidget->blockSignals(true);
    ui->tableWidget->setRowCount(starDatabase.starDatas.count());
    int row = 0;
    for (const auto &it: starDatabase.starDatas)
    {

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(it.raAsString(), row));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(it.decAsString(), row));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(it.mag()), row));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(it.pmRa()), row));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(it.pmDec()), row));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(it.parallax()), row));

        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(it.bvIndex()), row));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(it.specType(), row));
        ui->tableWidget->setItem(row, 8, new QTableWidgetItem(QString::number(it.flags()), row));

        ui->tableWidget->setItem(row, 9, new QTableWidgetItem(it.bayerName(), row));
        ui->tableWidget->setItem(row, 10, new QTableWidgetItem(it.longName(), row));
        row++;
    }
    ui->tableWidget->blockSignals(false);

    ui->namedStarsDataElement->setRowCount(starDatabase.namedStars.mDataElements.size());
    row = 0;
    for (const auto &it: starDatabase.namedStars.mDataElements)
    {
        ui->namedStarsDataElement->setItem(row, 0, new QTableWidgetItem(it.name(), row));
        ui->namedStarsDataElement->setItem(row, 1, new QTableWidgetItem(QString::number(it.size()), row));
        ui->namedStarsDataElement->setItem(row, 2, new QTableWidgetItem(QString::number(it.type()), row));
        ui->namedStarsDataElement->setItem(row, 3, new QTableWidgetItem(QString::number(it.scale()), row));
        row++;
    }

    ui->namedStarsEntry->setRowCount(starDatabase.namedStars.mIndexEntries.size());
    row = 0;
    for (const auto &it: starDatabase.namedStars.mIndexEntries)
    {
        ui->namedStarsEntry->setItem(row, 0, new QTableWidgetItem(QString::number(it.id()), row));
        ui->namedStarsEntry->setItem(row, 1, new QTableWidgetItem(QString::number(it.offset()), row));
        ui->namedStarsEntry->setItem(row, 2, new QTableWidgetItem(QString::number(it.count()), row));
        row++;
    }
}

void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    int row = item->type();
    bool changed = false;
    auto &star = starDatabase.starDatas[row];
    switch(item->column())
    {
    case 0:  changed = star.setRa(item->text()); break;
    case 1:  changed = star.setDec(item->text()); break;
    case 2:  changed = star.setMag(item->text().toDouble()); break;
    case 3:  changed = star.setPmRa(item->text().toDouble()); break;
    case 4:  changed = star.setPmDec(item->text().toDouble()); break;
    case 5:  changed = star.setParallax(item->text().toDouble()); break;
    case 6:  changed = star.setBvIndex(item->text().toDouble()); break;
    case 7:  changed = star.setSpecType(item->text().toLatin1()); break;
    case 8:  changed = star.setFlags(item->text().toInt()); break;
    case 9:  changed = star.setBayerName(item->text().toLatin1()); break;
    case 10: changed = star.setLongName(item->text().toLatin1()); break;
    }

    qDebug() << "changed" << changed;
}

void MainWindow::on_saveButton_clicked()
{
    starDatabase.setNamedStarsPath(namedStarsPath);
    starDatabase.setStarsNamesPath(starsNamedPath);
    starDatabase.save();
}

void MainWindow::on_loadButton_clicked()
{
    starDatabase.setNamedStarsPath(namedStarsPath);
    starDatabase.setStarsNamesPath(starsNamedPath);
    load();
}

void MainWindow::on_defaultButton_clicked()
{
    starDatabase.setNamedStarsPath(namedStarsDefaultPath);
    starDatabase.setStarsNamesPath(starsNamedDefaultPath);
    load();
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    for (int i=0; i<starDatabase.starDatas.count(); ++i)
    {
        bool show = ui->tableWidget->item(i, 10)->text().toLower().contains(arg1.toLower());
        if (show)
        {
            ui->tableWidget->showRow(i);
        }
        else
        {
            ui->tableWidget->hideRow(i);
        }

    }
}
