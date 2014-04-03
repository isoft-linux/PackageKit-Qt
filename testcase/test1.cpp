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
        m_pkt = new Transaction;
        connect(m_pkt, &Transaction::package, this, &SearchWorker::package);
        connect(m_pkt, &Transaction::finished, this, &SearchWorker::finished);
        m_pkt->searchNames(pkName);
    }

private slots:
    void errorCode(Transaction::Error error, const QString &details) 
    {
        qDebug() << "DEBUG: " << error << details; 
    }
    
    void finished(Transaction::Exit status, uint runtime) 
    {
        if (!m_pkt) return;
        disconnect(m_pkt, &Transaction::package, this, &SearchWorker::package);
        disconnect(m_pkt, &Transaction::finished, this, &SearchWorker::finished);
        delete m_pkt; m_pkt = NULL;
    }

    void package(Package pk) 
    {
        qDebug() << "DEBUG: " << pk.id() << pk.name() << pk.summary() << 
            pk.version() << pk.iconPath() << pk.size();
    }

private:
    PackageKit::Transaction *m_pkt;
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

    //-------------------------------------------------------------------------
    // Daemon
    //-------------------------------------------------------------------------
    qDebug() << "DEBUG: " << Daemon::backendName();
    qDebug() << "DEBUG: " << Daemon::backendAuthor();
    qDebug() << "DEBUG: " << Daemon::distroId();
   
    Transaction::Filters filters = Daemon::filters();
    qDebug() << "DEBUG: " << "Transaction::Filters enum " << filters;
    
    Package::Groups groups = Daemon::groups();
    foreach (const Package::Group &value, groups) 
        qDebug() << "DEBUG: " << "Package::Group enum " << value;
    
    QStringList mimeTypes = Daemon::mimeTypes();
    foreach (const QString &value, mimeTypes) 
        qDebug() << "DEBUG: " << value;
    
    qDebug() << "DEBUG: " << "Daemon::Network enum " << Daemon::networkState();

    SearchWorker sw("qt5");

    return app.exec();
}

#include "test1.moc"
