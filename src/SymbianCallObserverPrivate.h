/*
qgvdial is a cross platform Google Voice Dialer
Copyright (C) 2009-2012  Yuvraaj Kelkar

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Contact: yuvraaj@gmail.com
*/

#ifndef SYMBIANCALLOBSERVERPRIVATE_H
#define SYMBIANCALLOBSERVERPRIVATE_H

#include <QtCore>
#include <Etel3rdParty.h>

class SymbianCallInitiator;
class SymbianCallObserverPrivate : public CActive
{
public:
    static SymbianCallObserverPrivate* NewL(SymbianCallInitiator *parent);
    static SymbianCallObserverPrivate* NewLC(SymbianCallInitiator *parent);
    ~SymbianCallObserverPrivate ();

private:
    SymbianCallObserverPrivate(SymbianCallInitiator *p);
    void RunL();
    void DoCancel();
    void ConstructL();
    void StartListening ();

private:
    TInt                          iState;
    CTelephony::TCallStatusV1     iCurrentStatus;
    CTelephony::TCallStatusV1Pckg iCurrentStatusPckg;

    SymbianCallInitiator *parent;

    bool bUsable;
};

#endif // SYMBIANCALLOBSERVERPRIVATE_H
