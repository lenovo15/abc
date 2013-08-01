/*****************************************************************************/
/*  tank.h - plugin example for LibreCAD                                   */
/*                                                                           */
/*  Copyright (C) 2011 Rallaz, rallazz@gmail.com                             */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 2 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#ifndef tank_H
#define tank_H

#include "qc_plugininterface.h"
#include <QDialog>
#include <QComboBox>
class QLineEdit;

class LC_tank : public QObject, QC_PluginInterface
{
    Q_OBJECT
     Q_INTERFACES(QC_PluginInterface)
#if QT_VERSION >= 0x050000
     Q_PLUGIN_METADATA(IID "org.librecad.tank" FILE  "tank.json")
#endif

 public:
    virtual PluginCapabilities getCapabilities() const;
    virtual QString name() const;
    virtual void execComm(Document_Interface *doc,
                                       QWidget *parent, QString cmd);
};

class lc_tankdlg : public QDialog
{
    Q_OBJECT

public:
    explicit lc_tankdlg(QWidget *parent = 0);
    ~lc_tankdlg();


public slots:
    //void procesAction(QStringList *commandList);
    void procesAction(Document_Interface *doc);
    void checkAccept();

protected:
    void closeEvent(QCloseEvent *event);

private:


    void readSettings();
    void writeSettings();
    bool failGUI(QString *msg);

private:
    QString errmsg;
        QLineEdit *lengthedit;
        QLineEdit *heightedit;
        QLineEdit *widthedit;
        QLineEdit *cycheight;
        QLineEdit *cycradius;
        QLineEdit *startxedit;
        QLineEdit *startyedit;
        QLineEdit *spaceedit;
        QLineEdit *topedit;
        QLineEdit *frontedit;
        QLineEdit *sideedit;
        QComboBox *topcoloredit;
        QComboBox *frontcoloredit;
        QComboBox *sidecoloredit;

       // QString getLayer() { return topedit->text();}
        //void setLayer(QString l) { layedit->setText(l);}
       // QLineEdit *frontedit;
        //QLineEdit *sideedit;
public:
        void topview(double,double,double,Document_Interface *doc);
        void frontview(double,double,double,double,double,double,Document_Interface *doc);
        void sideview(double,double,double,double,double,double,Document_Interface *doc);

        //QString getLayer() { return topedit->text();}
      // void setLayer(QString l) { topedit->setText(l);}

};

#endif // tank_H

