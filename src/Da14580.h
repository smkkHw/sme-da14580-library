/*
 * Da14580.h
 *
 * Created: 28/2/2016 10:15:50 PM
 * by Mik (smkk@axelelettronica.com)
 * 
 * License Information
 * -------------------
 *
 * Copyright (c) axelelettronica. All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef DA14580_H_
#define DA14580_H_

#include <stdint-gcc.h>
#include <Arduino.h>

#include "da14580/smeDa14580Model.h"
#include "da14580/smeBleErrorCode.h"


class Da14580{ 
    
public:
    Da14580();
    virtual ~Da14580(){};
    void begin (unsigned long baudRate=19200);

private:
    


    // library API
public:
    bool fwDownload(int size);
};

// external variable used by the sketches
extern Da14580  asmeBle;


#endif /* SMESFX_H_ */