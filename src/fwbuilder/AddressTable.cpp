/* 

                          Firewall Builder

                 Copyright (C) 2005 NetCitadel, LLC

  Author:  Illiya Yalovoy <yalovoy@gmail.com>

  $Id$

  This program is free software which we release under the GNU General Public
  License. You may redistribute and/or modify this program under the terms
  of that license as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  To get a copy of the GNU General Public License, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include <assert.h>

#include <fwbuilder/libfwbuilder-config.h>

#include <fwbuilder/AddressTable.h>
#include <fwbuilder/FWException.h>
#include <fwbuilder/FWObjectReference.h>
#include <fwbuilder/FWObjectDatabase.h>
#include <fwbuilder/Network.h>
#include <fwbuilder/NetworkIPv6.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace libfwbuilder;
using namespace std;


const char *AddressTable::TYPENAME={"AddressTable"};

AddressTable::AddressTable() : MultiAddress() 
{
    setRunTime(false);
}

AddressTable::AddressTable(const FWObject *root,bool prepopulate) :
    MultiAddress(root,prepopulate) 
{
    setRunTime(false);
}

string AddressTable::getSourceName()
{
    return getStr("filename");
}

void AddressTable::setSourceName(const std::string& source_name)
{
    setStr("filename", source_name);
}

void AddressTable::fromXML(xmlNodePtr root) throw(FWException)
{
    FWObject::fromXML(root);
    const char *n;
    
    n=FROMXMLCAST(xmlGetProp(root,TOXMLCAST("filename")));
    assert(n!=NULL);
    setStr("filename", n);
    FREEXMLBUFF(n);

    n=FROMXMLCAST(xmlGetProp(root,TOXMLCAST("run_time")));
    assert(n!=NULL);
    setStr("run_time", n);
    FREEXMLBUFF(n);
}

xmlNodePtr AddressTable::toXML(xmlNodePtr parent) throw(FWException)
{
    xmlNodePtr me = FWObject::toXML(parent, false);

    xmlNewProp(me, TOXMLCAST("name"), STRTOXMLCAST(getName()));
    xmlNewProp(me, TOXMLCAST("comment"), STRTOXMLCAST(getComment()));
    xmlNewProp(me, TOXMLCAST("ro"), TOXMLCAST(((getRO()) ? "True" : "False")));

    return me;
}

void AddressTable::loadFromSource(bool ipv6) throw(FWException)
{
    ifstream fs(getStr("filename").c_str());
    ostringstream exmess;
    FWObject *root = getParent();
    string buf;
    size_type pos;
    int line = 1;
    int cntr = 0;

    Address *new_addr;

    if(fs)
    {
       while(!fs.eof())
       {
            getline(fs,buf);

            pos = buf.find_first_not_of(" \t");
            if (pos!=string::npos)
            {
                buf = buf.substr(pos);
                pos = buf.find_first_not_of("0123456789abcdef:/.");
                buf = buf.substr(0,pos);
            }
            else
            {
                buf="";
            }
            if (!buf.empty())
            {
                new_addr = NULL;
                if (ipv6 && buf.find(":")!=string::npos)
                {
                    try
                    {
                        InetAddr(AF_INET6, buf); // to test address

                        NetworkIPv6 *net;
                        net = NetworkIPv6::cast(getRoot()->create(
                                                    NetworkIPv6::TYPENAME));
                        net->setAddressNetmask(buf);
                        new_addr = net;
                    } catch (FWException &ex)
                    {
                        exmess << "Invalid address: "
                               << getStr("filename") << ":"
                               << line
                               << " \"" << buf << "\"";
                        throw FWException(exmess.str());
                    }
                }

                if (!ipv6 && buf.find(".")!=string::npos)
                {
                    try
                    {
                        InetAddr(AF_INET, buf); // to test address

                        Network *net;
                        net = Network::cast(getRoot()->create(
                                                Network::TYPENAME));
                        net->setAddressNetmask(buf);
                        new_addr = net;
                    } catch (FWException &ex)
                    {
                        exmess << "Invalid address: "
                               << getStr("filename") << ":"
                               << line
                               << " \"" << buf << "\"";
                        throw FWException(exmess.str());
                    }
                }

                if (new_addr)
                {
                    root->add(new_addr);
                    new_addr->setName(buf);
                    if (validateChild(new_addr))
                    {
                        addRef(new_addr);
                        cntr++;
                    }
                }
            }
            line++;
       }
    }
    else
    {
        exmess << "File not found for Address Table: "
               << getName()
               << " (" << getStr("filename") << ")";
        throw FWException(exmess.str());
    }
}

