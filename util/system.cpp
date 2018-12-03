#include "system.h"

const QString System::VERSION = "20181127";
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
