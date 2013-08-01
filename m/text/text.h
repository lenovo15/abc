/*****************************************************************************/
/*  sample.cpp - plugin example for LibreCAD                                 */
/*                                                                           */
/*  Copyright (C) 2011 Rallaz, rallazz@gmail.com                             */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 2 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSettings>
#include <QMessageBox>
#include <QDoubleValidator>

#include "document_interface.h"
#include "sample.h"
#include <QTextEdit>
QString LC_Sample::name() const
 {
     return (tr("new plugin"));
 }

PluginCapabilities LC_Sample::getCapabilities() const
{
    PluginCapabilities pluginCapabilities;
    pluginCapabilities.menuEntryPoints
            << PluginMenuLocation("Help", tr("new plugin"));
    return pluginCapabilities;
}

void LC_Sample::execComm(Document_Interface *doc,
                             QWidget *parent, QString cmd)
{
    Q_UNUSED(doc);
    Q_UNUSED(cmd);
    lc_Sampledlg pdt(parent);
    int result =  pdt.exec();
    if (result == QDialog::Accepted)
        pdt.procesAction(doc);
}




/*****************************/
lc_Sampledlg::lc_Sampledlg(QWidget *parent) :  QDialog(parent)
{
    setWindowTitle(tr("enter text"));
    QLabel *label;

    QDoubleValidator *val = new QDoubleValidator(0);
    QGridLayout *mainLayout = new QGridLayout;

    label = new QLabel(tr("Start X:"));
    mainLayout->addWidget(label, 0, 0);
    startxedit = new QLineEdit();
    startxedit->setValidator(val);
    mainLayout->addWidget(startxedit, 1, 0);

    label = new QLabel(tr("Start Y:"));
    mainLayout->addWidget(label, 0, 1);
    startyedit = new QLineEdit();
    startyedit->setValidator(val);
    mainLayout->addWidget(startyedit, 1, 1);

    
    QHBoxLayout *loaccept = new QHBoxLayout;
    QPushButton *acceptbut = new QPushButton(tr("Accept"));
    loaccept->addStretch();
    loaccept->addWidget(acceptbut);
    mainLayout->addLayout(loaccept, 2, 0);

    QPushButton *cancelbut = new QPushButton(tr("Cancel"));
    QHBoxLayout *locancel = new QHBoxLayout;
    locancel->addWidget(cancelbut);
    locancel->addStretch();
    mainLayout->addLayout(locancel, 2, 1);

    setLayout(mainLayout);
    readSettings();

    connect(cancelbut, SIGNAL(clicked()), this, SLOT(reject()));
    connect(acceptbut, SIGNAL(clicked()), this, SLOT(checkAccept()));
}


bool lc_Sampledlg::failGUI(QString *msg)
{
    if (startxedit->text().isEmpty()) {msg->insert(0, tr("Start X is empty")); return true;}
    if (startyedit->text().isEmpty()) {msg->insert(0, tr("Start Y is empty")); return true;}
   /* if (endxedit->text().isEmpty()) {msg->insert(0, tr("End X is empty")); return true;}
    if (endyedit->text().isEmpty()) {msg->insert(0, tr("End Y is empty")); return true;}*/
    return false;
}


void lc_Sampledlg::procesAction(Document_Interface *doc)
{
    Q_UNUSED(doc);
    QString txt="hello world";
    QString sty="";
    QPointF start;
    double height=20;
    double angle=0; 
    DPI::HAlign ha=DPI::HAlignLeft;
    DPI::VAlign va=DPI::VAlignTop;
    start.setX(startxedit->text().toDouble());
    start.setY(startyedit->text().toDouble());
   /* end.setX(endxedit->text().toDouble());
    end.setY(endyedit->text().toDouble());*/
	
    doc->addText(txt,sty, &start, height,angle, ha,va );
}

void lc_Sampledlg::checkAccept()
{

    errmsg.clear();
    if (failGUI(&errmsg)) {
        QMessageBox::critical ( this, "new plugin", errmsg );
        errmsg.clear();
        return;
    }
    accept();
}


lc_Sampledlg::~lc_Sampledlg()
{
}
void lc_Sampledlg::closeEvent(QCloseEvent *event)
 {
    writeSettings();
    QWidget::closeEvent(event);
 }


void lc_Sampledlg::readSettings()
 {
    QString str;
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "new_plugin");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(450,150)).toSize();

    startxedit->setText( settings.value("startx", 1).toString() );
    startyedit->setText( settings.value("starty", 1).toString() );
    /*endxedit->setText( settings.value("endx", 3.5).toString() );
    endyedit->setText( settings.value("endy", 3.5).toString() );*/

    resize(size);
    move(pos);
 }

void lc_Sampledlg::writeSettings()
 {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "new_plugin");
    settings.setValue("pos", pos());
    settings.setValue("size", size());

    settings.setValue("startx", startxedit->text());
    settings.setValue("starty", startyedit->text());}
   /* settings.setValue("endx", endxedit->text());
    settings.setValue("endy", endyedit->text());
 }
*/
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(lc_sample, LC_Sample);
#endif
