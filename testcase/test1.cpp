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

class MyTransHandler;

static void m_cleanup(int sig);

class MyTransHandler : public QObject 
{
    Q_OBJECT

public:
    MyTransHandler(QObject *parent = 0) {}
    ~MyTransHandler() {}

private slots:
    void package(PackageKit::Package package) 
    {
        QString strUSP = "/usr/share/pixmaps/";
        QString strUSI = "/usr/share/icons/hicolor/256x256/";
        qDebug() << package.id() << package.iconPath() << package.name() << package.summary() << package.version() << package.size();
    }
};

static void m_cleanup(int sig) 
{
    qDebug() << "Bye :)";
    if (sig == SIGINT) qApp->quit();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //signal(SIGINT, m_cleanup);

    //-------------------------------------------------------------------------
    // Daemon
    //-------------------------------------------------------------------------
    qDebug() << Daemon::backendName();
    qDebug() << Daemon::backendAuthor();
    qDebug() << Daemon::distroId();
   
    Transaction::Filters filters = Daemon::filters();
    qDebug() << "Transaction::Filters enum " << filters;
    
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
    Transaction *tranSearch = new Transaction(Daemon::global());
    MyTransHandler *mySearch = new MyTransHandler;
    QObject::connect(tranSearch, SIGNAL(package(PackageKit::Package)), 
        mySearch, SLOT(package(PackageKit::Package)));
    tranSearch->searchNames(argv[1] ? argv[1] : "qt5", Transaction::FilterInstalled);

    return a.exec();

    //-------------------------------------------------------------------------
    // Transaction::get-updates
    //-------------------------------------------------------------------------
    Transaction *tranGetUpdate = new Transaction(Daemon::global());
    MyTransHandler *myGetUpdate = new MyTransHandler;
    QObject::connect(tranGetUpdate, SIGNAL(package(PackageKit::Package)), 
        myGetUpdate, SLOT(package(PackageKit::Package)));
    tranGetUpdate->getUpdates();

    //-------------------------------------------------------------------------
    // Transaction::update 
    // there are percentage, elapsedTime, remainingTime, speed properties
    //-------------------------------------------------------------------------
    Transaction *tranUpdatePackage = new Transaction(Daemon::global());
    Package pkg1("apper;0.7.2-6;x86_64;community");
    qDebug() << pkg1.name() << pkg1.version();
    tranUpdatePackage->updatePackage(pkg1);

    //-------------------------------------------------------------------------
    // Transaction::remove
    //-------------------------------------------------------------------------
    Transaction *tranRemovePackage = new Transaction(Daemon::global());
    Package pkg2("libreoffice-writer;4.1.4-4;x86_64;installed");
    qDebug() << pkg2.name() << pkg2.version();
    tranRemovePackage->removePackage(pkg2);

    return a.exec();
}

#include "test1.moc"
