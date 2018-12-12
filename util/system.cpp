#include "system.h"

const QString System::VERSION = "20181213";
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
