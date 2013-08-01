/*****************************************************************************/
/*  tank.cpp - plugin example for LibreCAD                                 */
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
#include<QColor>
#include <QGroupBox>

#include "document_interface.h"
#include "tank.h"

QString LC_tank::name() const
 {
     return (tr("tank views"));
 }

PluginCapabilities LC_tank::getCapabilities() const
{
    PluginCapabilities pluginCapabilities;
    pluginCapabilities.menuEntryPoints
            << PluginMenuLocation("Help", tr("tank views"));
    return pluginCapabilities;
}

void LC_tank::execComm(Document_Interface *doc,
                             QWidget *parent, QString cmd)
{
    Q_UNUSED(doc);
    Q_UNUSED(cmd);
    lc_tankdlg pdt(parent);
    int result =  pdt.exec();
    if (result == QDialog::Accepted)
        pdt.procesAction(doc);
}




/*****************************/
lc_tankdlg::lc_tankdlg(QWidget *parent) :  QDialog(parent)
{
    setWindowTitle(tr("3 Views of Cuboid having tank"));
    QLabel *label;

    QDoubleValidator *val = new QDoubleValidator(0);
    QGridLayout *mainLayout = new QGridLayout;


         label = new QLabel(tr("Enter Start x Coordinate:"));
         mainLayout->addWidget(label, 0, 0);
         startxedit = new QLineEdit();
         startxedit->setValidator(val);
         mainLayout->addWidget(startxedit, 0, 1);


         label = new QLabel(tr("Enter Start y Coordinate:"));
         mainLayout->addWidget(label, 1, 0);
         startyedit = new QLineEdit();
         startyedit->setValidator(val);
         mainLayout->addWidget(startyedit, 1, 1);

         label = new QLabel(tr("Top View Layer:"));
         mainLayout->addWidget(label, 0, 2);
         topedit = new QLineEdit();
        // startxedit->setValidator(val);
         mainLayout->addWidget(topedit, 0, 3);

         label = new QLabel(tr("Color:"));
         mainLayout->addWidget(label, 0,5);
         topcoloredit = new QComboBox();
         QStringList txtstyles;
         txtstyles << "Red" << "Blue" << "Green"<<"White";
         topcoloredit->addItems(txtstyles);
         mainLayout->addWidget(topcoloredit, 0,6);

         label = new QLabel(tr("Front View Layer:"));
         mainLayout->addWidget(label, 1, 2);
         frontedit = new QLineEdit();
        // startxedit->setValidator(val);
         mainLayout->addWidget(frontedit, 1, 3);

         label = new QLabel(tr("Color:"));
         mainLayout->addWidget(label, 1,5);
         frontcoloredit = new QComboBox();
         frontcoloredit->addItems(txtstyles);
         mainLayout->addWidget(frontcoloredit, 1,6);

         label = new QLabel(tr("Side View Layer"));
         mainLayout->addWidget(label, 2, 2);
         sideedit = new QLineEdit();
        // startxedit->setValidator(val);
         mainLayout->addWidget(sideedit, 2, 3);

         label = new QLabel(tr("Color:"));
         mainLayout->addWidget(label, 2,5);
         sidecoloredit = new QComboBox();
         sidecoloredit->addItems(txtstyles);
         mainLayout->addWidget(sidecoloredit, 2,6);

         label = new QLabel(tr("Length:"));
         mainLayout->addWidget(label, 2, 0);
         lengthedit = new QLineEdit();
         lengthedit->setValidator(val);
         mainLayout->addWidget(lengthedit, 2, 1);

        label = new QLabel(tr("Height:"));
        mainLayout->addWidget(label, 3,0);
        heightedit = new QLineEdit();
        heightedit->setValidator(val);
        mainLayout->addWidget(heightedit, 3, 1);

        label = new QLabel(tr("Width:"));
        mainLayout->addWidget(label, 4, 0);
        widthedit = new QLineEdit();
        widthedit->setValidator(val);
        mainLayout->addWidget(widthedit, 4, 1);


        label = new QLabel(tr("Cylinder Height:"));
        mainLayout->addWidget(label, 5, 0);
        cycheight = new QLineEdit();
        cycheight->setValidator(val);
        mainLayout->addWidget(cycheight,5, 1);

        label = new QLabel(tr("Cylinder Radius:"));
        mainLayout->addWidget(label, 6, 0);
        cycradius = new QLineEdit();
        cycradius->setValidator(val);
        mainLayout->addWidget(cycradius, 6, 1);

        label = new QLabel(tr("Spacing between views"));
        mainLayout->addWidget(label, 7, 0);
        spaceedit = new QLineEdit();
        spaceedit->setValidator(val);
        mainLayout->addWidget(spaceedit, 7, 1);


        QHBoxLayout *loaccept = new QHBoxLayout;
        QPushButton *acceptbut = new QPushButton(tr("Accept"));
        loaccept->addStretch();
        loaccept->addWidget(acceptbut);
        mainLayout->addLayout(loaccept, 8, 0);

        QPushButton *cancelbut = new QPushButton(tr("Cancel"));
        QHBoxLayout *locancel = new QHBoxLayout;
        locancel->addWidget(cancelbut);
        locancel->addStretch();
        mainLayout->addLayout(locancel, 8, 1);

        setLayout(mainLayout);
        readSettings();

        connect(cancelbut, SIGNAL(clicked()), this, SLOT(reject()));
        connect(acceptbut, SIGNAL(clicked()), this, SLOT(checkAccept()));
    }


    bool lc_tankdlg::failGUI(QString *msg)
    {
        if (startxedit->text().isEmpty()) {msg->insert(0, tr("x coordinate field is empty")); return true;}
        if (startyedit->text().isEmpty()) {msg->insert(0, tr("y coordinate is empty")); return true;}
        if (lengthedit->text().isEmpty()) {msg->insert(0, tr("Length is empty")); return true;}
        if (widthedit->text().isEmpty()) {msg->insert(0, tr("Width is empty")); return true;}
        if (heightedit->text().isEmpty()) {msg->insert(0, tr("Height is empty")); return true;}
        if (cycheight->text().isEmpty()) {msg->insert(0, tr("Cylinder Height is empty")); return true;}
        if (cycradius->text().isEmpty()) {msg->insert(0, tr("Cylinder Radius is empty")); return true;}
        if (spaceedit->text().isEmpty()) {msg->insert(0, tr("Space field is empty")); return true;}
        if (topedit->text().isEmpty()) {msg->insert(0, tr("Top view field is empty")); return true;}
        if (frontedit->text().isEmpty()) {msg->insert(0,tr("Front view layer field is empty")); return true;}
         if (sideedit->text().isEmpty()) {msg->insert(0,tr("Side view layer field is empty")); return true;}
        return false;
    }

    void lc_tankdlg::procesAction(Document_Interface *doc)
    {
        Q_UNUSED(doc);
       double leng, breadth,height,r,CH,x; // r=radius, CH=Cylinder Height, x=spacing

        QString layer1,layer2,layer3;

         leng= lengthedit->text().toDouble();
         breadth= widthedit->text().toDouble();
         height= heightedit->text().toDouble();
         r= cycradius->text().toDouble();
         CH= cycheight->text().toDouble();
         x=spaceedit->text().toDouble();

         QColor s,s1,s2;
         switch (topcoloredit->currentIndex()) {
         case 0:
             s= QColor(255, 0, 0, 127);
             break;
         case 1:
             s= QColor(0,0, 255, 127);
              break;
         case 2:
              s= QColor(0, 255,0, 127);
             break;
         case 3:
              s= QColor(255, 255,255, 127);
             break;
         }


         layer1=topedit->text();
         doc->setLayer(layer1);
         doc->setCurrentLayerProperties(s,DPI::Width08  , DPI::SolidLine);
         topview(leng,breadth,r,doc);

         switch (frontcoloredit->currentIndex()) {
         case 0:
             s1= QColor(255, 0, 0, 127);
             break;
         case 1:
             s1= QColor(0,0, 255, 127);
              break;
         case 2:
              s1= QColor(0, 255,0, 127);
             break;
         case 3:
              s1= QColor(255, 255,255, 127);
             break;
         }

         layer2=frontedit->text();
         doc->setLayer(layer2);
         doc->setCurrentLayerProperties(s1,DPI::Width08  , DPI::SolidLine);
         frontview(height,leng,x,CH,r,breadth,doc);

         switch (sidecoloredit->currentIndex()) {
         case 0:
             s2= QColor(255, 0, 0, 127);
             break;
         case 1:
             s2= QColor(0,0, 255, 127);
              break;
         case 2:
              s2= QColor(0, 255,0, 127);
             break;
         case 3:
              s2= QColor(255, 255,255, 127);
             break;
         }
         layer3=sideedit->text();
         doc->setLayer(layer3);
         doc->setCurrentLayerProperties(s2,DPI::Width08  , DPI::SolidLine);

         sideview(leng,x,breadth,height,r,CH,doc);

       }

    void lc_tankdlg::checkAccept()
    {

        errmsg.clear();
        if (failGUI(&errmsg)) {
            QMessageBox::critical ( this, "views", errmsg );
            errmsg.clear();
            return;
        }
        accept();
    }




lc_tankdlg::~lc_tankdlg()
{
}
void lc_tankdlg::closeEvent(QCloseEvent *event)
 {
    writeSettings();
    QWidget::closeEvent(event);
 }

void lc_tankdlg::readSettings()
 {
    QString str;
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "views");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(430,140)).toSize();

        startxedit->setText( settings.value("x coordinate", 0).toString() );
        startyedit->setText( settings.value("y coordinate", 0).toString() );
        lengthedit->setText( settings.value("Length", 0.0).toString() );
        widthedit->setText( settings.value(" Width", 0.0).toString() );
        heightedit->setText( settings.value("Height", 0.0).toString() );
        cycheight->setText( settings.value("Cylinder Height", 0.0).toString() );
        cycradius->setText( settings.value("Cylinder Radius", 0.0).toString() );
        spaceedit->setText( settings.value("Spacing between Views", 0.0).toString() );
        topedit-> setText(settings.value("Top layer name").toString());
        frontedit->setText(settings.value("Front layer name").toString());
        sideedit->setText(settings.value("Side layer name").toString());

        resize(size);
    move(pos);
 }

void lc_tankdlg::writeSettings()
 {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "views");
    settings.setValue("pos", pos());
    settings.setValue("size", size());

        settings.setValue("Start x coordinate",  startxedit->text());
        settings.setValue("Start y coordinate",  startyedit->text());
        settings.setValue("Length",  lengthedit->text());
        settings.setValue("Width",  widthedit->text());
        settings.setValue("Height",heightedit->text());
        settings.setValue("Cylinder Height", cycheight->text());
        settings.setValue("Cylinder Radius", cycradius->text());
        settings.setValue("Top view layer", topedit->text());
        settings.setValue("Front view layer",frontedit->text());
        settings.setValue("Side view layer", sideedit->text());
 }

void lc_tankdlg::topview(double leng, double breadth, double r,Document_Interface *doc)
{
    QPointF Coord_a,Coord_b(0.0,0.0+breadth),Coord_c(leng,breadth),Coord_d(0.0+leng,0);// Top View= L*B  L=30,B=20
    Coord_a.setX(startxedit->text().toDouble());
    Coord_a.setY(startyedit->text().toDouble());
    Coord_b +=Coord_a;
    Coord_c +=Coord_a;
    Coord_d +=Coord_a;
    QPointF c_center(leng/2,breadth/2);
    c_center += Coord_a;


    doc->addLine(&Coord_a, &Coord_b); // coord_a to Coord_d -> for TOP view
    doc->addLine(&Coord_b, &Coord_c);
    doc->addLine(&Coord_c, &Coord_d);
    doc->addLine(&Coord_d, &Coord_a);
    doc-> addCircle(&c_center,r);

}

void lc_tankdlg::frontview(double height, double leng, double x, double CH,double r,double breadth,Document_Interface *doc)
{

    QPointF Coord_a,Coord_e(0.0,breadth+x), Coord_f(0.0,height);//front view
    Coord_a.setX(startxedit->text().toDouble());
    Coord_a.setY(startyedit->text().toDouble());
    Coord_e += Coord_a, Coord_f+= Coord_e;
    QPointF Coord_g(leng,0.0), Coord_h(0.0,0.0-height);
    Coord_g += Coord_f,Coord_h += Coord_g;

    QPointF mid1_l(leng/2-r,0.0), mid2_l(leng/2+r,0.0); //tank  front view
    mid1_l += Coord_f, mid2_l += Coord_f;

    QPointF ch1(0.0,CH), ch2(0.0,CH);
    ch1 += mid1_l, ch2 +=mid2_l;

    doc->addLine(&Coord_e, &Coord_f);//coord_e to coor_h for Front view
    doc->addLine(&Coord_f, &Coord_g );
    doc->addLine(&Coord_g, &Coord_h);
    doc->addLine(&Coord_h, & Coord_e);

    doc->addLine(&mid1_l,&ch1); // tank of front view
    doc->addLine(&mid2_l,&ch2);
    doc->addLine(&ch1,&ch2);
}

void lc_tankdlg::sideview(double leng, double x,double breadth,double height,double r, double CH,Document_Interface *doc)
{

    QPointF Coord_a,Coord_i(0.0+leng+x,0.0+breadth+height+x), Coord_j(breadth,0.0); // side view
    Coord_a.setX(startxedit->text().toDouble());
    Coord_a.setY(startyedit->text().toDouble());
    Coord_i += Coord_a, Coord_j += Coord_i;
    QPointF Coord_k(0.0,height),Coord_l(0.0,0.0-height);
    Coord_k = Coord_j-Coord_k, Coord_l += Coord_i;

    QPointF mid3_b(breadth/2-r,0.0), mid4_b(breadth/2+r,0.0); //tank side view
    mid3_b += Coord_i, mid4_b += Coord_i;
    QPointF ch3(0.0,CH), ch4(0.0,CH);
    ch3 += mid3_b, ch4 +=mid4_b;

    doc->addLine(&Coord_l, &Coord_i); //Coord_i to Coord_l for side view
    doc->addLine(&Coord_i, &Coord_j);
    doc->addLine(&Coord_j, &Coord_k);
    doc->addLine(&Coord_k, &Coord_l);

    doc->addLine(&mid3_b, &ch3);// side view of tank
    doc->addLine(&mid4_b,&ch4);
    doc->addLine(&ch3, &ch4);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(lc_tank, LC_tank);
#endif

