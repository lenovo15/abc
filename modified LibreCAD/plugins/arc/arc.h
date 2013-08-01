/*****************************************************************************/
/*  arc.h - plugin example for LibreCAD                                   */
/*                                                                           */
/*  Copyright (C) 2011 Rallaz, rallazz@gmail.com                             */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 2 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#ifndef arc_H
#define arc_H

#include "qc_plugininterface.h"
#include <QDialog>
class QLineEdit;

class LC_arc : public QObject, QC_PluginInterface
{
    Q_OBJECT
     Q_INTERFACES(QC_PluginInterface)
#if QT_VERSION >= 0x050000
     Q_PLUGIN_METADATA(IID "org.librecad.arc" FILE  "arc.json")
#endif

 public:
    virtual PluginCapabilities getCapabilities() const;
    virtual QString name() const;
    virtual void execComm(Document_Interface *doc,
                                       QWidget *parent, QString cmd);
};

class lc_arcdlg : public QDialog
{
    Q_OBJECT

public:
    explicit lc_arcdlg(QWidget *parent = 0);
    ~lc_arcdlg();

public slots:
//    void procesAction(QStringList *commandList);
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
    QLineEdit *startxedit;
    QLineEdit *startyedit;
    QLineEdit *radiusedit;
    QLineEdit *a1xedit;
    QLineEdit *a1yedit;
    QLineEdit *a2xedit;
  QLineEdit *a2yedit;



};

#endif // arc_H
