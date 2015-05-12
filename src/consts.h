// Copyright 2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef CONSTS
#define CONSTS

#include <QtGlobal>


const char gDateTimeFormat[] = "yyyy-MM-dd HH:mm:ss";

namespace Bookmark {
    const int MinRating = 0;
    const int MaxRating = 10;
}

namespace Bookmark {
    enum FilterOption {
        Any             = 0xFFFF,
        ReadItLater     = 0x0001,
        NotReadItLater  = 0x0100,
        Favorite        = 0x0002,
        NotFavorite     = 0x0200,
        Trash           = 0x0004,
        NotTrash        = 0x0400
    };
    Q_DECLARE_FLAGS(FilterOptions, FilterOption)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(Bookmark::FilterOptions)


#endif // CONSTS

