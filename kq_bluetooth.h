#ifndef KQ_BLUETOOTH_H
#define KQ_BLUETOOTH_H

#include <QWidget>
#include <QObject>
#include <QBluetoothAddress>
#include <QBluetoothSocket>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothServer>
#include <QBluetoothLocalDevice>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothServiceInfo>

#include <QDebug>
#include <QTimer>

QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)
QT_FORWARD_DECLARE_CLASS(QBluetoothAddress)
QT_FORWARD_DECLARE_CLASS(QBluetoothServiceInfo)
QT_FORWARD_DECLARE_CLASS(QBluetoothServiceDiscoveryAgent)
QT_FORWARD_DECLARE_CLASS(QBluetoothLocalDevice)

QT_USE_NAMESPACE

class Kq_bluetooth : public QObject
{
    Q_OBJECT
public:
    explicit Kq_bluetooth(QObject *parent = 0);
    ~Kq_bluetooth();
    QBluetoothLocalDevice *localDevice;
    int count;
    QByteArray wsd_rec;
public slots:
    void addDevice(const QBluetoothDeviceInfo &info);
    void addService(const QBluetoothServiceInfo &info);
    //void addService_printer(const QBluetoothServiceInfo &info);
    //void connected_printer();
    //void disconnected_printer();
    void readSocket();
    void connected();
    void disconnected();
    void scanFinished();

    //void kq_print_data(QString string);
    void lianjie_zidong();
    void Send_temp_query(QByteArray data);

    //蓝牙关闭
    void Bluetooth_Close();

signals:
    void wsd_xianshi(QByteArray);
    //void dyj_state(bool);
    void jly_state(bool);

    void zigbee_data_shuaxin(QByteArray);

    //网络数据中转
    //void wsd_net_send(QByteArray);

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothServiceDiscoveryAgent *service;//,*service_printer;
    QBluetoothSocket *socket;//,*socket_printer;
    bool success_bz;//,success_bz_printer;
    QBluetoothServiceInfo temp_info;//,temp_info_printer;
};

#endif // KQ_BLUETOOTH_H
