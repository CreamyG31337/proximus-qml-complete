#include "dbusiface.h"

DbusIface::DbusIface(Controller *myController)
    : QDBusAbstractAdaptor(myController), controller(myController)
{

}

bool DbusIface::startTimer(int min, int max)
{
    controller->startExternalTimer(min,max);
    return true;
}
