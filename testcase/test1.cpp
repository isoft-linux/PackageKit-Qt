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
#include <QDebug>
#include <QStringList>
#include <QThread>
#include <Daemon>

using namespace PackageKit;

class SearchWorker : public QObject
{
    Q_OBJECT

public:
    SearchWorker(QString pkName, QObject *parent = 0) : QObject(parent), m_pkt(NULL) 
    {
        m_pkt = new Transaction(this);
        connect(m_pkt, 
            SIGNAL(package(PackageKit::Transaction::Info, QString, QString)), 
            this, 
            SLOT(package(PackageKit::Transaction::Info, QString, QString)));
        connect(m_pkt, 
            SIGNAL(finished(PackageKit::Transaction::Exit, uint)), 
            this, 
            SLOT(finished(PackageKit::Transaction::Exit, uint)));
        connect(m_pkt, 
            SIGNAL(errorCode(PackageKit::Transaction::Error, QString)), 
            this, 
            SLOT(errorCode(PackageKit::Transaction::Error, QString)));
        m_pkt->searchNames(pkName);
    }
    ~SearchWorker() 
    {
        if (!m_pkt) return;
        disconnect(m_pkt, 
            SIGNAL(package(PackageKit::Transaction::Info, QString, QString)), 
            this, 
            SLOT(package(PackageKit::Transaction::Info, QString, QString)));
        disconnect(m_pkt, 
            SIGNAL(finished(PackageKit::Transaction::Exit, uint)),   
            this, 
            SLOT(finished(PackageKit::Transaction::Exit, uint)));            
        disconnect(m_pkt, 
            SIGNAL(errorCode(PackageKit::Transaction::Error, QString)), 
            this, 
            SLOT(errorCode(PackageKit::Transaction::Error, QString)));
        delete m_pkt; m_pkt = NULL;
    }

private slots:
    void errorCode(PackageKit::Transaction::Error error, const QString &details) 
    {
        qDebug() << "DEBUG: " << __PRETTY_FUNCTION__ << error << details; 
    }
    
    void finished(PackageKit::Transaction::Exit status, uint runtime) 
    {
        qDebug() << "DEBUG: " << __PRETTY_FUNCTION__ << status << runtime;
    }

    void package(PackageKit::Transaction::Info info, 
                 const QString &packageID, 
                 const QString &summary) 
    {
        qDebug() << "DEBUG: " << __PRETTY_FUNCTION__ << packageID << 
            Transaction::packageName(packageID) << summary << 
            Transaction::packageVersion(packageID) << 
            Transaction::packageIcon(packageID);
    }

private:
    Transaction *m_pkt;
};

class SearchThread : public QThread 
{
    Q_OBJECT

public:
    SearchThread(QString pkName) : m_sw(NULL), m_pkName(pkName) {}
    ~SearchThread() { if (m_sw) delete m_sw; m_sw = NULL; }

protected:
    void run() { m_sw = new SearchWorker(m_pkName); }

private:
    SearchWorker *m_sw;
    QString m_pkName;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "DEBUG: " << Daemon::global()->backendName();
    qDebug() << "DEBUG: " << Daemon::global()->backendAuthor();
    qDebug() << "DEBUG: " << Daemon::global()->distroID();

    SearchWorker sw("qt5");

    return app.exec();
}

#include "test1.moc"
