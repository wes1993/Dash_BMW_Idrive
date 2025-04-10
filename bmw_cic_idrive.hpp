#include <QString>
#include <iostream>
#include <stdlib.h>
#include <QByteArray>
#include <boost/log/trivial.hpp>

#include "plugins/vehicle_plugin.hpp"
#include "app/arbiter.hpp"
#include "openauto/Service/InputService.hpp"
#include "AAHandler.hpp"

#define CIC_LOG(severity) BOOST_LOG_TRIVIAL(severity) << "[CICVehiclePlugin] "

class DebugWindow : public QWidget {
    Q_OBJECT

    public:
        DebugWindow(Arbiter &arbiter, QWidget *parent = nullptr);
        QLabel* inReverse;
        QLabel* KeyLock;
        QLabel* rpm;
        QLabel* rotaryPos;
        QLabel* msgCounter;
        QLabel* lastKey;

};

class BMWCIC : public QObject, VehiclePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID VehiclePlugin_iid)
    Q_INTERFACES(VehiclePlugin)

    public:
        BMWCIC() {};
        ~BMWCIC();
        bool init(ICANBus* canbus) override;

    private:
        QList<QWidget *> widgets() override;

        ICANBus* canbus;
        bool cic_fullscreen = false;
        bool KeyLock = false;
        bool inReverse = false;
        int rotaryPrevPos = -1;
        int rotaryPos = -1;
        unsigned short int msgCounter = 0;
        aasdk::proto::enums::ButtonCode::Enum lastKey = aasdk::proto::enums::ButtonCode::NONE;

        void monitorIdriveRotaryStatus(QByteArray payload);
        void monitorIdriveButtonStatus(QByteArray payload);
        void monitorSteeringWheelButtonStatus(QByteArray payload);
        // void monitorGearStatus(QByteArray payload);
        void monitorEngineRPM(QByteArray payload);
        // void monitorVehicleSpeed(QByteArray payload);
        void monitorCicStatus(QByteArray payload);
        void injectFrame(QByteArray payload);
        void switchTVInput();
        // QString toDebug(const QByteArray & line);

        AAHandler *aa_handler;
        DebugWindow *debug;
};
