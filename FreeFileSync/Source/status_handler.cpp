// *****************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under    *
// * GNU General Public License: https://www.gnu.org/licenses/gpl-3.0          *
// * Copyright (C) Zenju (zenju AT freefilesync DOT org) - All Rights Reserved *
// *****************************************************************************

#include "status_handler.h"
#include <thread>
#include <zen/basic_math.h>

using namespace zen;


namespace
{
std::chrono::steady_clock::time_point lastExec;
}


bool fff::uiUpdateDue(bool force)
{
    const auto now = std::chrono::steady_clock::now();

    if (force || now >= lastExec + UI_UPDATE_INTERVAL)
    {
        lastExec = now;
        return true;
    }
    return false;
}


void fff::delayAndCountDown(std::chrono::nanoseconds delay, const std::function<void(const std::wstring& timeRemMsg)>& notifyStatus)
{
    assert(notifyStatus);
    if (notifyStatus)
        while (delay > std::chrono::nanoseconds(0))
        {
            const auto timeRemMs = std::chrono::duration_cast<std::chrono::milliseconds>(delay).count();
            notifyStatus(_P("1 sec", "%x sec", numeric::intDivCeil(timeRemMs, 1000)));

            std::this_thread::sleep_for(UI_UPDATE_INTERVAL / 2);
            delay -= UI_UPDATE_INTERVAL / 2; //support "Pause" => don't count time spent in notifyStatus()!
        }
    else
        std::this_thread::sleep_for(delay /*may be negative*/);
}
