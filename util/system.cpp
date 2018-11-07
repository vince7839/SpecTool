#include "system.h"

System::System()
{

}

QString System::getSeparator()
{
    QString separator = "\n";
    if(QSysInfo::productType() == "windows"){
        separator = "\r\n";
    }
    return separator;
}
