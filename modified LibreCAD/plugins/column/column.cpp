/*****************************************************************************/
/*  column.cpp - plugin example for LibreCAD                                 */
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
#include <math.h>
#include "document_interface.h"
#include "column.h"

QString LC_column::name() const
 {
     return (tr("column"));
 }

PluginCapabilities LC_column::getCapabilities() const
{
    PluginCapabilities pluginCapabilities;
    pluginCapabilities.menuEntryPoints
            << PluginMenuLocation("Help", tr("column"));
    return pluginCapabilities;
}

void LC_column::execComm(Document_Interface *doc,
                             QWidget *parent, QString cmd)
{
    Q_UNUSED(doc);
    Q_UNUSED(cmd);
    lc_columndlg pdt(parent);
    int result =  pdt.exec();
    if (result == QDialog::Accepted)
        pdt.procesAction(doc);
}




/*****************************/
lc_columndlg::lc_columndlg(QWidget *parent) :  QDialog(parent)
{
    setWindowTitle(tr("Views"));
    QLabel *label;

    QDoubleValidator *val = new QDoubleValidator(0);
    QGridLayout *mainLayout = new QGridLayout;

    label = new QLabel(tr("Center x Coordinate:"));
    mainLayout->addWidget(label, 0, 0);
    startxedit = new QLineEdit();
    startxedit->setValidator(val);
    mainLayout->addWidget(startxedit, 0, 1);

    label = new QLabel(tr("Center y Coordinate:"));
    mainLayout->addWidget(label, 1, 0);
    startyedit = new QLineEdit();
    startyedit->setValidator(val);
    mainLayout->addWidget(startyedit, 1, 1);

   /* label = new QLabel(tr("End X:"));
    mainLayout->addWidget(label, 2, 0);
    endxedit = new QLineEdit();
    endxedit->setValidator(val);
    mainLayout->addWidget(endxedit, 3, 0);

    label = new QLabel(tr("End Y:"));
    mainLayout->addWidget(label, 2, 1);
    endyedit = new QLineEdit();
    endyedit->setValidator(val);
    mainLayout->addWidget(endyedit, 3, 1);
*/

    QHBoxLayout *loaccept = new QHBoxLayout;
    QPushButton *acceptbut = new QPushButton(tr("Accept"));
    loaccept->addStretch();
    loaccept->addWidget(acceptbut);
    mainLayout->addLayout(loaccept, 4, 0);

    QPushButton *cancelbut = new QPushButton(tr("Cancel"));
    QHBoxLayout *locancel = new QHBoxLayout;
    locancel->addWidget(cancelbut);
    locancel->addStretch();
    mainLayout->addLayout(locancel, 4, 1);

    setLayout(mainLayout);
    readSettings();

    connect(cancelbut, SIGNAL(clicked()), this, SLOT(reject()));
    connect(acceptbut, SIGNAL(clicked()), this, SLOT(checkAccept()));
}


bool lc_columndlg::failGUI(QString *msg)
{
    if (startxedit->text().isEmpty()) {msg->insert(0, tr("Start X is empty")); return true;}
    if (startyedit->text().isEmpty()) {msg->insert(0, tr("Start Y is empty")); return true;}

    return false;
}


void lc_columndlg::procesAction(Document_Interface *doc)
{
    Q_UNUSED(doc);
    qreal radius1=350;
    qreal radius2= radius1-40;
    qreal radiusin=radius2-8;
    qreal radius3= radius2-16;
    qreal radius4 = 8;
    int angle=360,bars=8; // steel bars
    QPointF Coord_a(20.0,0.0),Coord_b(0,3*radius1),Coord_c(2*radius1,0),Coord_d(0,0.0-3*radius1),mid_1(radius1,0);
    QPointF circum_1,circum_pt,start(0,radius1+40),end_pt;
    QPointF Coord_e, Coord_f,Coord_g,Coord_h;
            Coord_b += Coord_a;
            Coord_c += Coord_b;
            Coord_d +=Coord_c;
            mid_1 += Coord_b;
            start += mid_1;
            end_pt +=Coord_a;
            Coord_e += Coord_a;
            Coord_f +=Coord_d;
            Coord_g +=Coord_a;
            Coord_h +=Coord_d;


          //  QPainter painter;
            //painter.setPen( Qt::white );

            //painter.drawRect( 100, 50, 30, 35 );
            //painter.drawRoundRect( 10, 55, 85, 35 );
            //QRect rect( 105, 10, 85, 35 );

           // painter.drawRoundRect( rect );

    doc->setLayer("lay1");
    doc->setCurrentLayerProperties(QColor(255,0,255, 127),DPI::Width15  , DPI::SolidLine);
    doc->addLine(&Coord_a,&Coord_b);
    doc->addCircle(&start,radius1);
    doc->addLine(&Coord_b,&Coord_c);
    doc->addLine(&Coord_c,&Coord_d);

    doc->setLayer("lay2");
    doc->setCurrentLayerProperties(QColor(0,0,255, 127),DPI::Width20  , DPI::SolidLine);
    doc->addCircle(&start, radius2);
    doc->addCircle(&start, radiusin);

       for (int i=0; i!=angle; i = i + angle/bars)
       {
           circum_1.setY(start.ry()+ radius3*cos(i*.0174));
           circum_1.setX(start.rx()+radius3*sin(i*.0174));
           doc->addCircle(&circum_1,radius4);

           for (int j=90; j<angle; j= j+180)
           {
               circum_pt.setY(3*radius1);
               circum_pt.setX(circum_1.rx() + radius4*sin(j*0.0174));
               end_pt.setX(mid_1.rx()+radius3*sin(i*.0174)+radius4*sin(j*.0174));
               doc->addLine (&circum_pt,&end_pt);
           }
       }
       for (int k=0; k<(3*radius1-3*radius1/(.5*bars)); k= k+3*radius1/(.5*bars+1))
       {
           Coord_e.setY((3*radius1/(.5*bars+1)+k));
           Coord_f.setY((3*radius1/(.5*bars+1)+k));
           Coord_g.setY(Coord_e.ry()+8);
           Coord_h.setY(Coord_f.ry()+8);
           doc->addLine (&Coord_e,&Coord_f);
           doc->addLine (&Coord_g,&Coord_h);

       }
}


void lc_columndlg::checkAccept()
{

    errmsg.clear();
    if (failGUI(&errmsg)) {
        QMessageBox::critical ( this, "column plugin", errmsg );
        errmsg.clear();
        return;
    }
    accept();
}


lc_columndlg::~lc_columndlg()
{
}
void lc_columndlg::closeEvent(QCloseEvent *event)
 {
    writeSettings();
    QWidget::closeEvent(event);
 }


void lc_columndlg::readSettings()
 {
    QString str;
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "column_plugin");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(430,140)).toSize();

    startxedit->setText( settings.value("startx", 0.5).toString() );
    startyedit->setText( settings.value("starty", 0.5).toString() );
    //endxedit->setText( settings.value("endx", 3.5).toString() );
    //endyedit->setText( settings.value("endy", 3.5).toString() );

    resize(size);
    move(pos);
 }

void lc_columndlg::writeSettings()
 {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "column_plugin");
    settings.setValue("pos", pos());
    settings.setValue("size", size());

    settings.setValue("startx", startxedit->text());
    settings.setValue("starty", startyedit->text());
   // settings.setValue("endx", endxedit->text());
   // settings.setValue("endy", endyedit->text());
 }

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(lc_column, LC_column);
#endif
