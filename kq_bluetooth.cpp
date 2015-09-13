#include "kq_bluetooth.h"
#include <QBluetoothUuid>

static const QLatin1String serviceUuid("00001101-0000-1000-8000-00805F9B34FB");

Kq_bluetooth::Kq_bluetooth(QObject *parent) : QObject(parent),localDevice(new QBluetoothLocalDevice)
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    localDevice->powerOn();
    discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry);

    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(scanFinished()));
    connect(discoveryAgent,SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),this,SLOT(addDevice(QBluetoothDeviceInfo)));
    discoveryAgent->start();

    QBluetoothServiceInfo serviceinfo;
    serviceinfo.setAttribute(QBluetoothServiceInfo::ServiceId,serviceUuid);

    socket = new QBluetoothSocket(serviceinfo.RfcommProtocol);
    //socket_printer = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);

    success_bz=true;
    //success_bz_printer=true;
    count=0;
}

Kq_bluetooth::~Kq_bluetooth()
{
   // delete discoveryAgent;
}


void Kq_bluetooth::addDevice(const QBluetoothDeviceInfo &info)
{
    QBluetoothLocalDevice::Pairing Status = localDevice->pairingStatus(info.address());
        if (Status == QBluetoothLocalDevice::Paired || Status == QBluetoothLocalDevice::AuthorizedPaired )
        {
            if(info.name()==QString("FBT06") || info.name()==QString("SPP-CA") || info.name()==QString("HC-05"))
            {
                if(success_bz)
                {
                    QBluetoothAddress address;
                    address=info.address();

                    QBluetoothAddress adapterAddress = localDevice->address();

                    service = new QBluetoothServiceDiscoveryAgent(adapterAddress);
                    service->setRemoteAddress(address);
                    connect(service, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
                            this, SLOT(addService(QBluetoothServiceInfo)));

                    service->start();
                }
            }

               /*
            if(info.name()==QString("QSprinter") )
            {
               if(success_bz_printer)
               {
                    QBluetoothAddress address1;
                    address1=info.address();


                    QBluetoothAddress adapterAddress1 = localDevice->address();


                    service_printer = new QBluetoothServiceDiscoveryAgent(adapterAddress1);

                    service_printer->setRemoteAddress(address1);

                    connect(service_printer, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
                            this, SLOT(addService_printer(QBluetoothServiceInfo)));


                    service_printer->start();
                }
             }*/
        }
}

void Kq_bluetooth::scanFinished()
{
    //if(success_bz|success_bz_printer)discoveryAgent->start();
    if(success_bz)
    {
        discoveryAgent->start();
    }
}

/*
void Kq_bluetooth::addService_printer(const QBluetoothServiceInfo &info)
{
    temp_info_printer=info;
    if (info.serviceName().isEmpty())
        return;

    success_bz_printer=true;
    socket_printer->connectToService(info);

    connect(socket_printer, SIGNAL(connected()), this, SLOT(connected_printer()));
    connect(socket_printer, SIGNAL(disconnected()), this, SLOT(disconnected_printer()));
}
*/

void Kq_bluetooth::addService(const QBluetoothServiceInfo &info)
{
    temp_info=info;
    if (info.serviceName().isEmpty())
        return;

    success_bz=true;
    socket->connectToService(info);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

/*
void Kq_bluetooth::connected_printer()
{
    success_bz_printer=false;
    emit this->dyj_state(true);
}
*/

void Kq_bluetooth::connected()
{
    success_bz=false;
    emit this->jly_state(true);
}

/*
void Kq_bluetooth::disconnected_printer()
{
    success_bz_printer=true;
    emit this->dyj_state(false);
}
*/

void Kq_bluetooth::disconnected()
{
    success_bz=true;
    emit this->jly_state(false);
}

void Kq_bluetooth::readSocket()
{

    QByteArray line = socket->readAll();
    wsd_rec.append(line);

    if(wsd_rec.size()==8)
    {
        if(wsd_rec.at(3)==char(0x04))
        {
            //发送过来的数据进行显示
            emit this->wsd_xianshi(wsd_rec);
            //qDebug()<<"wenshidu is         "+wsd_rec.toHex();
            //发送过来一条数据，然后通过网络进行中转
           // emit this->wsd_net_send(wsd_rec);
        }
        else if(wsd_rec.at(3)==char(0x16))
        {
            //刷新数据
           emit this->zigbee_data_shuaxin(wsd_rec);
        }
        else if(wsd_rec.at(3)==char(0x26))
        {
            //刷新数据
           emit this->zigbee_data_shuaxin(wsd_rec);
        }

        wsd_rec.clear();
    }
    else if(wsd_rec.size()>8)
    {
        wsd_rec.clear();
    }

}

void Kq_bluetooth::lianjie_zidong()
{
    if(success_bz) socket->connectToService(temp_info);
    //打印机相关部分  已删除
    //if(success_bz_printer) socket_printer->connectToService(temp_info_printer);
}

/*
void Kq_bluetooth::kq_print_data(QString string)
{
    if(!success_bz_printer)
        socket_printer->write(string.toLocal8Bit().data());

    string.clear();
}
*/

void Kq_bluetooth::Send_temp_query(QByteArray data)
{
    socket->write(data);
}

void Kq_bluetooth::Bluetooth_Close()
{  
    localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
}


