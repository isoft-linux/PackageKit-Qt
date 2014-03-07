/*                                                                              
 * Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>                      
 *                                                                              
 * This program is free software: you can redistribute it and/or modify         
 * it under the terms of the GNU General Public License as published by         
 * the Free Software Foundation, either version 3 of the License, or            
 * any later version.                                                           
 *                                                                              
 * This program is distributed in the hope that it will be useful,              
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                
 * GNU General Public License for more details.                                 
 *                                                                              
 * You should have received a copy of the GNU General Public License            
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.        
 */

#include <QObject>
#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <QStringList>
#include <Daemon>
#include <signal.h>

using namespace PackageKit;

class MySearch;

static Transaction *m_trans = NULL;
static MySearch *m_mys = NULL;

static void m_cleanup(int sig);

class MySearch : public QObject 
{
    Q_OBJECT

public:
    MySearch(QObject *parent = 0) {}
    ~MySearch() {}

private slots:
    void addPackage(PackageKit::Package package) 
    {
        qDebug() << package.name();
    }
};

static void m_cleanup(int sig) 
{
    qDebug() << "Bye :)";
    QObject::disconnect(m_trans, SIGNAL(package(PackageKit::Package)), 
        m_mys, SLOT(addPackage(PackageKit::Package)));
    if (m_trans) delete m_trans; m_trans = NULL;
    if (m_mys) delete m_mys; m_mys = NULL;
    if (sig == SIGINT) qApp->quit();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    signal(SIGINT, m_cleanup);

    //-------------------------------------------------------------------------
    // Daemon
    //-------------------------------------------------------------------------
    qDebug() << Daemon::backendName();
    qDebug() << Daemon::backendAuthor();
    qDebug() << Daemon::distroId();
    
    Package::Groups groups = Daemon::groups();
    foreach (const Package::Group &value, groups) 
        qDebug() << "Package::Group enum " << value;
    
    QStringList mimeTypes = Daemon::mimeTypes();
    foreach (const QString &value, mimeTypes) 
        qDebug() << value;
    
    qDebug() << "Daemon::Network enum " << Daemon::networkState();

    //-------------------------------------------------------------------------
    // Transaction::search
    //-------------------------------------------------------------------------
    m_trans = new Transaction(Daemon::global());
    MySearch *m_mys = new MySearch;
    QObject::connect(m_trans, SIGNAL(package(PackageKit::Package)), 
        m_mys, SLOT(addPackage(PackageKit::Package)));
    m_trans->searchNames(argv[1] ? argv[1] : "qt5");

    //-------------------------------------------------------------------------
    // Transaction::getCategories GetCategories not supported by alpm backend
    //-------------------------------------------------------------------------

    return a.exec();
}

#include "test1.moc"
