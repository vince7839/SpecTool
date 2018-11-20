#include "system.h"

const QString System::VERSION = "2018-11-13";
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
