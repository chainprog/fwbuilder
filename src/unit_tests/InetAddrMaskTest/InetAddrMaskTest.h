/*

                          Firewall Builder

                 Copyright (C) 2010 NetCitadel, LLC

  Author:  Roman Bovsunivskiy     a2k0001@gmail.com

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

#ifndef INETADDRMASKTEST_H
#define INETADDRMASKTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "fwbuilder/InetAddrMask.h"

#include <vector>



class InetAddrMaskTest : public CppUnit::TestFixture
{
public:
    std::string vectorInetAddrMaskToString(
        std::vector<libfwbuilder::InetAddrMask> vect);
    
    void testIntToInetAddr();
    void testStringToInetAddr();
    void testStringToInetAddrExceptions();
    void testStringToInetAddrMask();
    void testInetAddressOps();
    void testIPv4Overlap();

    CPPUNIT_TEST_SUITE(InetAddrMaskTest);

    CPPUNIT_TEST(testStringToInetAddrExceptions);
    CPPUNIT_TEST(testIntToInetAddr);
    CPPUNIT_TEST(testStringToInetAddr);
    CPPUNIT_TEST(testStringToInetAddrMask);
    CPPUNIT_TEST(testInetAddressOps);
    CPPUNIT_TEST(testIPv4Overlap);
    
    CPPUNIT_TEST_SUITE_END();

};

#endif // INETADDRMASKTEST_H
